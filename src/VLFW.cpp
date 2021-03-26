#include "VLFW/VLFW.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>

#include "GLFW/glfw3.h"
#include "ValkyrieEngine/ValkyrieEngine.hpp"

using namespace vlk;
using namespace vlfw;

namespace
{
	std::mutex mtx;
	std::vector<Monitor*> monitorRegistry;
}

void ErrorCallback(Int errorCode, const char* what)
{
	SendEvent(vlfw::ErrorEvent{errorCode, std::string(what)});
}

void MonitorConnectedCallback(GLFWmonitor* monitor, Int event)
{
	if (event == GLFW_CONNECTED)
	{
		Monitor* monitorObject = new Monitor(monitor);
		SendEvent(Monitor::ConnectEvent{monitorObject});

		// Add monitor to registry
		monitorRegistry.push_back(monitorObject);
	}
	else if (event == GLFW_DISCONNECTED)
	{
		Monitor* monitorObject = reinterpret_cast<Monitor*>(glfwGetMonitorUserPointer(monitor));
		SendEvent(Monitor::DisconnectEvent{monitorObject});

		// Remove monitor from registry
		monitorRegistry.erase(
			std::find(monitorRegistry.begin(), monitorRegistry.end(), monitorObject));

		// Delete monitor object
		delete monitorObject;
	}
}

void WindowMoveCallback(GLFWwindow* window, Int x, Int y)
{
	Window* obj = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	SendEvent(Window::MoveEvent({obj, Point<Int>(x, y)}));
}

VLFWMain::VLFWMain(const VLFWMainArgs& args) :
	lock(mtx, std::try_to_lock)
{
	if (!lock.owns_lock())
	{
		throw std::runtime_error("Multiple concurrent instances of VLFWMain is disallowed.");
		return;
	}
	
	glfwSetErrorCallback(ErrorCallback);

	glfwInit();
	glfwSetMonitorCallback(MonitorConnectedCallback);

	{ // Set Up monitors
		Int monitorCount;
		GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
		
		monitorRegistry.clear();
		monitorRegistry.reserve(monitorCount);

		for (Int i = 0; i < monitorCount; i++)
		{
			Monitor* monitor = new Monitor(monitors[i]);
			monitorRegistry.push_back(monitor);
		}
	}

	waitMode = args.waitMode;
	waitTimeout = args.waitTimeout;
}

VLFWMain::~VLFWMain()
{
	monitorRegistry.clear();

	glfwTerminate();
}

void VLFWMain::OnEvent(const vlk::PostUpdateEvent& ev)
{
	// Process Events
	if (waitMode == WaitMode::Poll)
	{
		glfwPollEvents();
	}
	else if (waitTimeout > 0)
	{
		glfwWaitEventsTimeout(waitTimeout);
	}
	else
	{
		glfwWaitEvents();
	}
	

	// Check close flags
	std::vector<Component<Window>*> toClose;

	Component<Window>::ForEach([&toClose](Component<Window>* c)
	{
		c->SwapBuffers();
		if (c->GetCloseFlag()) toClose.push_back(c);
	});

	for (auto it = toClose.begin(); it != toClose.end(); it++)
	{
		(*it)->Delete();
	}
}

void VLFWMain::SendEmptyEvent()
{
	glfwPostEmptyEvent();
}

void VLFWMain::SetSwapInterval(Int interval)
{
	glfwSwapInterval(interval);
}

Monitor::Monitor(MonitorHandle handle) :
	nativeHandle(handle)
{
	GLFWmonitor* mhdl = reinterpret_cast<GLFWmonitor*>(handle);
	glfwSetMonitorUserPointer(mhdl, this);

	Int numVideoModes;
	const GLFWvidmode* modes = glfwGetVideoModes(mhdl, &numVideoModes);

	supportedModes.reserve(numVideoModes);
	for (Int i = 0; i < numVideoModes; i++)
	{
		const GLFWvidmode& m(modes[i]);

		supportedModes.push_back(VideoMode{
			Point<Int>(m.width, m.height),
			m.redBits,
			m.greenBits,
			m.blueBits,
			m.refreshRate
		});
	}

	const GLFWgammaramp* ramp = glfwGetGammaRamp(mhdl);

	gammaRamp.reserve(ramp->size);
	for (Int i = 0; i < ramp->size; i++)
	{
		gammaRamp.emplace_back(
			ramp->red[i] / 255.f,
			ramp->green[i] / 255.0f,
			ramp->blue[i] / 255.0f,
			1.f);
	}
}

Monitor* Monitor::GetPrimaryMonitor()
{
	return reinterpret_cast<Monitor*>(
		glfwGetMonitorUserPointer(glfwGetPrimaryMonitor()));

}

Int Monitor::GetMonitorCount()
{
	return monitorRegistry.size();
}

const std::vector<Monitor*>& Monitor::GetMonitors()
{
	return monitorRegistry;
}

Point<Int> Monitor::GetPosition() const
{
	Point<Int> p;
	glfwGetMonitorPos(reinterpret_cast<GLFWmonitor*>(nativeHandle), &p.X(), &p.Y());
	return p;
}

Point<Int> Monitor::GetPhysicalSize() const
{
	Point<Int> p;
	glfwGetMonitorPhysicalSize(reinterpret_cast<GLFWmonitor*>(nativeHandle), &p.X(), &p.Y());
	return p;
}

Vector2 Monitor::GetContentScale() const
{
	Vector2 v;
	glfwGetMonitorContentScale(reinterpret_cast<GLFWmonitor*>(nativeHandle), &v.X(), &v.Y());
	return v;
}

Area<Int> Monitor::GetWorkingArea() const
{
	Area<Int> a;
	glfwGetMonitorWorkarea(reinterpret_cast<GLFWmonitor*>(nativeHandle),
		&a.location.X(), &a.location.Y(), &a.size.X(), &a.size.Y());
	return a;
}

std::string Monitor::GetName() const
{
	return std::string(glfwGetMonitorName(reinterpret_cast<GLFWmonitor*>(nativeHandle)));
}

void Monitor::SetGamma(double gamma)
{
	GLFWmonitor* monitor = reinterpret_cast<GLFWmonitor*>(nativeHandle);
	glfwSetGamma(monitor, gamma);
	const GLFWgammaramp* ramp = glfwGetGammaRamp(monitor);

	gammaRamp.clear();
	gammaRamp.reserve(ramp->size);
	for (Int i = 0; i < ramp->size; i++)
	{
		gammaRamp.emplace_back(
			ramp->red[i] / 255.f,
			ramp->green[i] / 255.0f,
			ramp->blue[i] / 255.0f,
			1.f);
	}
}

void Monitor::SetGammaRamp(const std::vector<Color>& ramp)
{
	GLFWgammaramp gamma;
	gamma.size = ramp.size();
	std::vector<unsigned short> red, green, blue;
	red.reserve(gamma.size);
	green.reserve(gamma.size);
	blue.resize(gamma.size);

	for (Int i = 0; i < gamma.size; i++)
	{
		red.push_back(ramp[i].R() * 255.0f);
		green.push_back(ramp[i].G() * 255.0f);
		blue.push_back(ramp[i].B() * 255.0f);
	}

	gamma.red = red.data();
	gamma.green = green.data();
	gamma.blue = blue.data();

	glfwSetGammaRamp(reinterpret_cast<GLFWmonitor*>(nativeHandle), &gamma);
	gammaRamp = ramp;
}

VideoMode Monitor::GetVideoMode() const
{
	const GLFWvidmode* mode = glfwGetVideoMode(reinterpret_cast<GLFWmonitor*>(nativeHandle));
	return VideoMode {
		Point<Int>(mode->width, mode->height),
		mode->redBits,
		mode->greenBits,
		mode->blueBits,
		mode->refreshRate
	};
}

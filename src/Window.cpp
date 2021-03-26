#include "VLFW/VLFW.hpp"

#include "GLFW/glfw3.h"
#include "ValkyrieEngine/ValkyrieEngine.hpp"
#include <stdexcept>

using namespace vlk;
using namespace vlfw;

void CloseCallback(GLFWwindow* window)
{
	Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	SendEvent(Window::CloseEvent{w});
}

void ResizeCallback(GLFWwindow* window, Int x, Int y)
{
	Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	SendEvent(Window::ResizeEvent{w, Point<Int>(x, y)});
}

void FramebufferResizeCallback(GLFWwindow* window, Int x, Int y)
{
	Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	SendEvent(Window::FramebufferResizeEvent{w, Point<Int>(x, y)});
}

void ContentScaleCallback(GLFWwindow* window, Float x, Float y)
{
	Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	SendEvent(Window::ContentScaleChangeEvent{w, Vector2(x, y)});
}

void MoveCallback(GLFWwindow* window, Int x, Int y)
{
	Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	SendEvent(Window::MoveEvent{w, Point<Int>(x, y)});
}

void MinimizeCallback(GLFWwindow* window, Int action)
{
	Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	SendEvent(Window::MinimizeEvent{w, static_cast<bool>(action)});
}

void MaximizeCallback(GLFWwindow* window, Int action)
{
	Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	SendEvent(Window::MaximizeEvent{w, static_cast<bool>(action)});
}

void FocusCallback(GLFWwindow* window, Int action)
{
	Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	SendEvent(Window::FocusEvent{w, static_cast<bool>(action)});
}

void RefreshCallback(GLFWwindow* window)
{
	Window* w = reinterpret_cast<Window*>(glfwGetWindowUserPointer(window));
	SendEvent(Window::RefreshEvent{w});
}

Window::Window(const WindowHints& hints)
{
	// Set window hints
	{
		glfwDefaultWindowHints();

		// Cosmetic
		glfwWindowHint(GLFW_RESIZABLE,        hints.resizable);
		glfwWindowHint(GLFW_VISIBLE,          hints.visible);
		glfwWindowHint(GLFW_DECORATED,        hints.decorated);
		glfwWindowHint(GLFW_SCALE_TO_MONITOR, hints.obeyContentScale);

		// Focus Control

		glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, hints.transparent);
		glfwWindowHint(GLFW_FLOATING,                hints.topMost);
		glfwWindowHint(GLFW_AUTO_ICONIFY,            hints.minimizeOnFocusLoss);
		glfwWindowHint(GLFW_FOCUS_ON_SHOW,           hints.focusOnShow);

		// Create Behaviour
		glfwWindowHint(GLFW_FOCUSED,       hints.focusOnCreate);
		glfwWindowHint(GLFW_MAXIMIZED,     hints.maximizeOnCreate);
		glfwWindowHint(GLFW_CENTER_CURSOR, hints.centerCursorOnCreate);

		// Framebuffer
		glfwWindowHint(GLFW_RED_BITS,     hints.framebufferRedBits);
		glfwWindowHint(GLFW_GREEN_BITS,   hints.framebufferGreenBits);
		glfwWindowHint(GLFW_BLUE_BITS,    hints.framebufferBlueBits);
		glfwWindowHint(GLFW_ALPHA_BITS,   hints.framebufferAlphaBits);
		glfwWindowHint(GLFW_DEPTH_BITS,   hints.framebufferDepthBits);
		glfwWindowHint(GLFW_STENCIL_BITS, hints.framebufferStencilBits);
		glfwWindowHint(GLFW_SAMPLES,      hints.framebufferSamples);
		glfwWindowHint(GLFW_SRGB_CAPABLE, hints.enableSRGB);
		glfwWindowHint(GLFW_DOUBLEBUFFER, hints.enableDoubleBuffered);

		// Monitor
		glfwWindowHint(GLFW_REFRESH_RATE, hints.fullscreenRefreshRate);

		// Context
		glfwWindowHint(GLFW_CLIENT_API,            static_cast<Int>(hints.clientAPI));
		glfwWindowHint(GLFW_CONTEXT_CREATION_API,  static_cast<Int>(hints.contextAPI));
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, hints.contextVersionMajor);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, hints.contextVersionMinor);
		glfwWindowHint(GLFW_CONTEXT_NO_ERROR,      hints.noErrorContext);

		// OpenGL
		glfwWindowHint(GLFW_STEREO,                hints.enableStereoscopy);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, hints.openglForwardCompatible);
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,  hints.openglDebugContext);
		glfwWindowHint(GLFW_OPENGL_PROFILE,        static_cast<Int>(hints.openglProfile));

		// MacOS
		glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, hints.macosRetinaFramebuffer);
		glfwWindowHint(GLFW_COCOA_GRAPHICS_SWITCHING, hints.macosAutoGraphicsSwitch);
		glfwWindowHintString(GLFW_COCOA_FRAME_NAME,   hints.macosFrameName.data());

		// X11
		glfwWindowHintString(GLFW_X11_CLASS_NAME,    hints.x11ClassName.data());
		glfwWindowHintString(GLFW_X11_INSTANCE_NAME, hints.x11InstanceName.data());
	}

	GLFWmonitor* mon;
	if (hints.monitor == nullptr) mon = nullptr;
	else mon = reinterpret_cast<GLFWmonitor*>(hints.monitor->GetHandle());

	// Create Window
	GLFWwindow* window = glfwCreateWindow(
		hints.size.X(),
		hints.size.Y(),
		hints.title.data(),
		mon,
		nullptr);

	// Window creation failed, abort.
	if (window == nullptr) throw std::runtime_error("Window creation failed.");

	// Set user pointer and handle association
	glfwSetWindowUserPointer(window, this);
	handle = window;

	// Set other members
	raiseStopOnClose = hints.raiseStopOnClose;
	
	// Setup callbacks
	{
		glfwSetWindowCloseCallback(window,        CloseCallback);
		glfwSetWindowSizeCallback(window,         ResizeCallback);
		glfwSetFramebufferSizeCallback(window,    FramebufferResizeCallback);
		glfwSetWindowContentScaleCallback(window, ContentScaleCallback);
		glfwSetWindowPosCallback(window,          MoveCallback);
		glfwSetWindowIconifyCallback(window,      MinimizeCallback);
		glfwSetWindowMaximizeCallback(window,     MaximizeCallback);
		glfwSetWindowFocusCallback(window,        FocusCallback);
		glfwSetWindowRefreshCallback(window,      RefreshCallback);
	}
}

Window::~Window()
{
	glfwDestroyWindow(reinterpret_cast<GLFWwindow*>(handle));

	if (raiseStopOnClose) vlk::Application::Stop();
}

bool Window::GetCloseFlag() const
{
	return glfwWindowShouldClose(reinterpret_cast<GLFWwindow*>(handle));
}

void Window::SetCloseFlag(bool value)
{
	glfwSetWindowShouldClose(reinterpret_cast<GLFWwindow*>(handle), value);
}

Point<Int> Window::GetSize() const
{
	Point<Int> p;
	glfwGetWindowSize(reinterpret_cast<GLFWwindow*>(handle), &p.X(), &p.Y());
	return p;
}

Point<Int> Window::GetFramebufferSize() const
{
	Point<Int> p;
	glfwGetFramebufferSize(reinterpret_cast<GLFWwindow*>(handle), &p.X(), &p.Y());
	return p;
}

void Window::GetDecoratedSize(Int* left, Int* top, Int* right, Int* bottom) const
{
	glfwGetWindowFrameSize(reinterpret_cast<GLFWwindow*>(handle), left, top, right, bottom);
}

void Window::SetSize(const Point<Int>& size)
{
	glfwSetWindowSize(reinterpret_cast<GLFWwindow*>(handle), size.X(), size.Y());
}

Vector2 Window::GetContentScale() const
{
	Vector2 v;
	glfwGetWindowContentScale(reinterpret_cast<GLFWwindow*>(handle), &v.X(), &v.Y());
	return v;
}

void Window::SetSizeLimit(const Point<Int>& min, const Point<Int>& max)
{
	glfwSetWindowSizeLimits(reinterpret_cast<GLFWwindow*>(handle), min.X(), min.Y(), max.X(), max.Y());
}

void Window::SetAspectRatio(const Point<Int>& ratio)
{
	glfwSetWindowAspectRatio(reinterpret_cast<GLFWwindow*>(handle), ratio.X(), ratio.Y());
}

Point<Int> Window::GetPosition() const
{
	Point<Int> p;
	glfwGetWindowPos(reinterpret_cast<GLFWwindow*>(handle), &p.X(), &p.Y());
	return p;
}

void Window::SetPosition(const Point<Int>& pos)
{
	glfwSetWindowPos(reinterpret_cast<GLFWwindow*>(handle), pos.X(), pos.Y());
}

void Window::SetTitle(const std::string& title)
{
	glfwSetWindowTitle(reinterpret_cast<GLFWwindow*>(handle), title.data());
}

Monitor* Window::GetMonitor() const
{
	return reinterpret_cast<Monitor*>(
		glfwGetMonitorUserPointer(
			glfwGetWindowMonitor(
				reinterpret_cast<GLFWwindow*>(handle))));
}

void Window::SetMonitor(Monitor* monitor)
{
	VideoMode mode = monitor->GetVideoMode();

	glfwSetWindowMonitor(
		reinterpret_cast<GLFWwindow*>(handle),
		reinterpret_cast<GLFWmonitor*>(monitor->GetHandle()),
		0,
		0,
		mode.size.X(),
		mode.size.Y(),
		mode.refreshRate
	);
}

void Window::SetMonitor(Monitor* monitor, const Area<Int>& area, Int refreshRate)
{
	glfwSetWindowMonitor(
		reinterpret_cast<GLFWwindow*>(handle),
		reinterpret_cast<GLFWmonitor*>(monitor->GetHandle()),
		area.location.X(),
		area.location.Y(),
		area.size.X(),
		area.size.Y(),
		refreshRate
	);
}

bool Window::IsMaximized() const
{
	return glfwGetWindowAttrib(reinterpret_cast<GLFWwindow*>(handle), GLFW_MAXIMIZED);
}

bool Window::IsMinimized() const
{
	return glfwGetWindowAttrib(reinterpret_cast<GLFWwindow*>(handle), GLFW_ICONIFIED);
}

void Window::Minimize()
{
	glfwIconifyWindow(reinterpret_cast<GLFWwindow*>(handle));
}

void Window::Restore()
{
	glfwRestoreWindow(reinterpret_cast<GLFWwindow*>(handle));
}

void Window::Maximize()
{
	glfwMaximizeWindow(reinterpret_cast<GLFWwindow*>(handle));
}

bool Window::IsVisible() const
{
	return glfwGetWindowAttrib(reinterpret_cast<GLFWwindow*>(handle), GLFW_VISIBLE);
}

void Window::Show()
{
	glfwShowWindow(reinterpret_cast<GLFWwindow*>(handle));
}

void Window::Hide()
{
	glfwHideWindow(reinterpret_cast<GLFWwindow*>(handle));
}

bool Window::IsFocused() const
{
	return glfwGetWindowAttrib(reinterpret_cast<GLFWwindow*>(handle), GLFW_FOCUSED);
}

void Window::Focus()
{
	glfwFocusWindow(reinterpret_cast<GLFWwindow*>(handle));
}

void Window::Notify()
{
	glfwRequestWindowAttention(reinterpret_cast<GLFWwindow*>(handle));
}

bool Window::IsTransparent() const
{
	return glfwGetWindowAttrib(reinterpret_cast<GLFWwindow*>(handle), GLFW_TRANSPARENT_FRAMEBUFFER);
}

Float Window::GetOpacity() const
{
	return glfwGetWindowOpacity(reinterpret_cast<GLFWwindow*>(handle));
}

void Window::SetOpacity(Float opacity)
{
	glfwSetWindowOpacity(reinterpret_cast<GLFWwindow*>(handle), opacity);
}

bool Window::IsResizable() const
{
	return glfwGetWindowAttrib(reinterpret_cast<GLFWwindow*>(handle), GLFW_RESIZABLE);
}

bool Window::IsHovered() const
{
	return glfwGetWindowAttrib(reinterpret_cast<GLFWwindow*>(handle), GLFW_HOVERED);
}

bool Window::IsDecorated() const
{
	return glfwGetWindowAttrib(reinterpret_cast<GLFWwindow*>(handle), GLFW_DECORATED);
}

bool Window::IsMinimizeOnFocusLoss() const
{
	return glfwGetWindowAttrib(reinterpret_cast<GLFWwindow*>(handle), GLFW_AUTO_ICONIFY);
}

bool Window::IsTopMost() const
{
	return glfwGetWindowAttrib(reinterpret_cast<GLFWwindow*>(handle), GLFW_FLOATING);
}

bool Window::IsFocusOnShow() const
{
	return glfwGetWindowAttrib(reinterpret_cast<GLFWwindow*>(handle), GLFW_FOCUS_ON_SHOW);
}

ClientAPIType Window::GetClientAPI() const
{
	switch (glfwGetWindowAttrib(reinterpret_cast<GLFWwindow*>(handle), GLFW_CLIENT_API))
	{
		case GLFW_OPENGL_API:
			return ClientAPIType::OpenGL;
		case GLFW_OPENGL_ES_API:
			return ClientAPIType::OpenGLES;
		default:
			return ClientAPIType::None;
	}
}

ContextAPIType Window::GetContextAPI() const
{
	switch (glfwGetWindowAttrib(reinterpret_cast<GLFWwindow*>(handle), GLFW_CONTEXT_CREATION_API))
	{
		case GLFW_OSMESA_CONTEXT_API:
			return ContextAPIType::OSMesa;
		case GLFW_EGL_CONTEXT_API:
			return ContextAPIType::EGL;
		default:
			return ContextAPIType::Native;
	}
}

OpenGLProfileType Window::GetOpenGLProfile() const
{
	switch (glfwGetWindowAttrib(reinterpret_cast<GLFWwindow*>(handle), GLFW_OPENGL_PROFILE))
	{
		case GLFW_OPENGL_CORE_PROFILE:
			return OpenGLProfileType::Core;
		case GLFW_OPENGL_COMPAT_PROFILE:
			return OpenGLProfileType::Compatability;
		default:
			return OpenGLProfileType::Any;
	}
}

void Window::GetContextVersion(Int* major, Int* minor, Int* revision) const
{
	*major = glfwGetWindowAttrib(
		reinterpret_cast<GLFWwindow*>(handle),
		GLFW_CONTEXT_VERSION_MAJOR);

	*minor = glfwGetWindowAttrib(
		reinterpret_cast<GLFWwindow*>(handle),
		GLFW_CONTEXT_VERSION_MINOR);

	*revision = glfwGetWindowAttrib(
		reinterpret_cast<GLFWwindow*>(handle),
		GLFW_CONTEXT_REVISION);
}

bool Window::IsOpenGLForwardCompatible() const
{
	return glfwGetWindowAttrib(reinterpret_cast<GLFWwindow*>(handle), GLFW_OPENGL_FORWARD_COMPAT);
}

bool Window::IsOpenGLDebug() const
{
	return glfwGetWindowAttrib(reinterpret_cast<GLFWwindow*>(handle), GLFW_OPENGL_DEBUG_CONTEXT);
}

bool Window::IsNoErrorContext() const
{
	return glfwGetWindowAttrib(reinterpret_cast<GLFWwindow*>(handle), GLFW_CONTEXT_NO_ERROR);
}

void Window::SwapBuffers()
{
	glfwSwapBuffers(reinterpret_cast<GLFWwindow*>(handle));
}

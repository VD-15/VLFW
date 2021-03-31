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

void KeyCallback(GLFWwindow* window, Int key, Int scancode, Int action, Int mods)
{
	switch (action)
	{
		case GLFW_PRESS:
			SendEvent(Window::KeyDownEvent{
				reinterpret_cast<Window*>(glfwGetWindowUserPointer(window)),
				static_cast<Key>(key),
				scancode});
		case GLFW_RELEASE:
			SendEvent(Window::KeyUpEvent{
				reinterpret_cast<Window*>(glfwGetWindowUserPointer(window)),
				static_cast<Key>(key),
				scancode});
		case GLFW_REPEAT:
			SendEvent(Window::KeyRepeatEvent{
				reinterpret_cast<Window*>(glfwGetWindowUserPointer(window)),
				static_cast<Key>(key),
				scancode});
		default:
			break;
	}
}

void CharCallback(GLFWwindow* window, UInt codepoint)
{
	SendEvent(Window::CharTypeEvent{
		reinterpret_cast<Window*>(glfwGetWindowUserPointer(window)),
		codepoint
		});
}

void CursorEnterCallback(GLFWwindow* window, Int entered)
{
	if (entered)
	{
		SendEvent(Window::CursorEnterEvent{
			reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))
		});
	}
	else
	{
		SendEvent(Window::CursorLeaveEvent{
			reinterpret_cast<Window*>(glfwGetWindowUserPointer(window))
		});
	}
}

void CursorPosCallback(GLFWwindow* window, double xpos, double ypos)
{
	SendEvent(Window::MouseMoveEvent{
		reinterpret_cast<Window*>(glfwGetWindowUserPointer(window)),
		Vector2(static_cast<Float>(xpos), static_cast<Float>(ypos))
	});
}

void MouseButtonCallback(GLFWwindow* window, Int button, Int action, Int mods)
{
	if (action == GLFW_PRESS)
	{
		SendEvent(Window::MouseButtonDownEvent{
			reinterpret_cast<Window*>(glfwGetWindowUserPointer(window)),
			static_cast<MouseButton>(button)
		});
	}
	else
	{
		SendEvent(Window::MouseButtonUpEvent{
			reinterpret_cast<Window*>(glfwGetWindowUserPointer(window)),
			static_cast<MouseButton>(button)
		});
	}
}

void ScrollCallback(GLFWwindow* window, double x, double y)
{
	SendEvent(Window::ScrollEvent{
		reinterpret_cast<Window*>(glfwGetWindowUserPointer(window)),
		Vector2(static_cast<Float>(x), static_cast<Float>(y))
	});
}

void Window::PollEvents()
{
	glfwPollEvents();
}

void Window::WaitEvents(double timeout)
{
	if (timeout > 0.0)
	{
		glfwWaitEventsTimeout(timeout);
	}
	else
	{
		glfwWaitEvents();
	}
}

Window* Window::GetCurrentContext()
{
	GLFWwindow* context = glfwGetCurrentContext();

	if (context)
	{
		return reinterpret_cast<Window*>(glfwGetWindowUserPointer(context));
	}
	else
	{
		return nullptr;
	}
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
		glfwSetKeyCallback(window,                KeyCallback);
		glfwSetCharCallback(window,               CharCallback);
		glfwSetCursorEnterCallback(window,        CursorEnterCallback);
		glfwSetMouseButtonCallback(window,        MouseButtonCallback);
		glfwSetCursorPosCallback(window,          CursorPosCallback);
		glfwSetScrollCallback(window,             ScrollCallback);
	}
}

Window::~Window()
{
	glfwDestroyWindow(reinterpret_cast<GLFWwindow*>(handle));

	if (raiseStopOnClose) vlk::Application::Stop();
}

void Window::MakeContextCurrent()
{
	glfwMakeContextCurrent(reinterpret_cast<GLFWwindow*>(handle));
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
	return static_cast<ClientAPIType>(
		glfwGetWindowAttrib(reinterpret_cast<GLFWwindow*>(handle), GLFW_CLIENT_API));
}

ContextAPIType Window::GetContextAPI() const
{
	return static_cast<ContextAPIType>(
		glfwGetWindowAttrib(reinterpret_cast<GLFWwindow*>(handle), GLFW_CONTEXT_CREATION_API));
}

OpenGLProfileType Window::GetOpenGLProfile() const
{
	return static_cast<OpenGLProfileType>(
		glfwGetWindowAttrib(reinterpret_cast<GLFWwindow*>(handle), GLFW_OPENGL_PROFILE));
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

bool Window::IsRawMouseInputSupported() const
{
	return glfwRawMouseMotionSupported();
}

bool Window::IsRawMouseInput() const
{
	return static_cast<bool>(
		glfwGetInputMode(
			reinterpret_cast<GLFWwindow*>(handle),
			GLFW_RAW_MOUSE_MOTION));
}

void Window::SetRawMouseInput(bool rawMouseInput)
{
	if (glfwRawMouseMotionSupported())
	{
		glfwSetInputMode(
			reinterpret_cast<GLFWwindow*>(handle),
			GLFW_RAW_MOUSE_MOTION,
			rawMouseInput);
	}
}

CursorMode Window::GetCursorMode() const
{
	return static_cast<CursorMode>(
		glfwGetInputMode(
			reinterpret_cast<GLFWwindow*>(handle),
			GLFW_CURSOR));
}

void Window::SetCursorMode(CursorMode mode)
{
	glfwSetInputMode(
		reinterpret_cast<GLFWwindow*>(handle),
		GLFW_CURSOR,
		static_cast<Int>(mode));
}

void Window::ResetCursor()
{
	glfwSetCursor(reinterpret_cast<GLFWwindow*>(handle), nullptr);
}

void Window::SetCursor(Cursor& cursor)
{
	glfwSetCursor(
		reinterpret_cast<GLFWwindow*>(handle),
		reinterpret_cast<GLFWcursor*>(cursor.GetHandle()));
}

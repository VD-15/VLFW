#ifndef VLFW_WINDOW_HPP
#define VLFW_WINDOW_HPP

#include "Monitor.hpp"

namespace vlk
{
	namespace vlfw
	{
		enum class ClientAPIType
		{
			//! Don't create a client context
			None =           0x00000000,

			//! Create an OpenGL context
			OpenGL =         0x00030001,

			//! Create an OpenGL ES context
			OpenGLES =       0x00030002,
		};

		enum class ContextAPIType
		{
			//! Use the system's native API to create a context
			Native =         0x00036001,

			//! Use EGL to create a context
			EGL =            0x00036002,

			//! Use OSMesa to create a context
			OSMesa =         0x00036003
		};

		enum class OpenGLProfileType
		{
			//! Use any available OpenGL profile
			Any =            0x00000000,

			//! Use an OpenGL Core profile
			Core =           0x00032001,

			//! Use an OpenGL Compatibility profile
			Compatability =  0x00032002
		};

		struct WindowHints
		{
			//! Desired size of the window, in screen coordinates
			Point<Int> size =               Point<Int>(640, 480);

			//! Initial title of the window (UTF-8 encoded)
			std::string title =             u8"ValkyrieEngine Application";

			//Window* share =                 nullptr;

			////////////////////////
			//// Cosmetic flags ////
			////////////////////////
			
			//! Whether the window should be resizable by the user
			bool resizable =                true;

			//! Whether the window should be visible when created
			bool visible =                  true;

			//! Whether the window should have decorations such as a border and widgets.
			bool decorated =                true;

			//! Whether the window should obey the content scale of the monitor it's on.
			bool obeyContentScale =         true;

			/////////////////////////////
			//// Focus control flags ////
			/////////////////////////////

			//! Whether the window should have a transparent framebuffer
			bool transparent =              false;

			//! Whether the window should be rendered on top of any other windows
			bool topMost =                  false;

			//! Whether the window should be minimized when focus is lost
			bool minimizeOnFocusLoss =      false;

			//! Whether the window should regain focus when Window::Show() is called
			bool focusOnShow =              true;

			////////////////////////////////
			//// Create behaviour flags ////
			////////////////////////////////

			//! Whether the window should be given focus when created
			bool focusOnCreate =            true;

			//! Whether the window should be maximized when created
			bool maximizeOnCreate =         false;

			//! Whether the cursoe should be centered on the window when created
			bool centerCursorOnCreate =     false;

			/////////////////////////////
			//// Framebuffer options ////
			/////////////////////////////

			//! Desired bit depth of the red channel of the default framebuffer or -1 for auto
			Int framebufferRedBits =        8;

			//! Desired bit depth of the green channel of the default framebuffer or -1 for auto
			Int framebufferGreenBits =      8;

			//! Desired bit depth of the blue channel of the default framebuffer or -1 for auto
			Int framebufferBlueBits =       8;

			//! Desired bit depth of the alpha channel of the default framebuffer or -1 for auto
			Int framebufferAlphaBits =      8;

			//! Desired bit depth of the depth channel of the default framebuffer or -1 for auto
			Int framebufferDepthBits =      24;

			//! Desired bit depth of the stencil channel of the default framebuffer or -1 for auto
			Int framebufferStencilBits =    8;

			//! Desired number of samples per pizel for the default framebuffer or -1 for auto
			Int framebufferSamples =        0;

			//! Whether the default framebuffer should be sRGB capable
			bool enableSRGB =               false;

			//! Whether the default framebuffer should be double-buffered
			bool enableDoubleBuffered =     true;

			///////////////////////
			//// Monitor options //
			///////////////////////

			//! The monitor to use for fullscreen mode, or <tt>nullptr</tt> for windowed mode
			Monitor* monitor =              nullptr;

			//! The refresh rate of the window in fullscreen mode, or -1 to match the monitor
			Int fullscreenRefreshRate =     -1;

			/////////////////////////
			//// Context options ////
			/////////////////////////

			//! The client API to create a context for
			ClientAPIType clientAPI =       ClientAPIType::OpenGL;

			//! The context creation API used to generate the context
			ContextAPIType contextAPI =     ContextAPIType::Native;

			//! The minimum required major version the context must conform to
			Int contextVersionMajor =       1;

			//! The minimum required minor version the context must conform to
			Int contextVersionMinor =       0;

			//! Whether the context should not generate any errors
			bool noErrorContext =           false;

			/////////////////////////////////
			//// OpenGL-specific options ////
			/////////////////////////////////

			//! Whether to enable OpenGL stereoscopic rendering
			bool enableStereoscopy =              false;

			//! Whether to request a forward compatible OpenGL context
			bool openglForwardCompatible =        false;

			//! Whether to request a debug OpenGL context
			bool openglDebugContext =             false;

			//! What OpenGL profile to create the context for
			OpenGLProfileType openglProfile =     OpenGLProfileType::Any;

			////////////////////////////////
			//// macOS-specific options ////
			////////////////////////////////

			//! Whether to use full resolution framebuffers on retina displays
			bool macosRetinaFramebuffer =   true;

			//! Whether to allow the system to automatically switch graphics devices to save battery
			bool macosAutoGraphicsSwitch =  false;

			//! UTF-8 encoded name to use for autosaving the window frame, empty string disables
			std::string macosFrameName =    "";

			//////////////////////////////
			//// X11-specific options ////
			//////////////////////////////

			//! ASCII-encoded name for this window class
			std::string x11ClassName =      "";

			//! ASCII-encoded name for this window instance
			std::string x11InstanceName =   "";

			/////////////////////////////////////////
			//// ValkyrieEngine-specific options ////
			/////////////////////////////////////////

			//! Raise ValkyrieEngine's stop flag when destroyed
			bool raiseStopOnClose =         true;
		};

		class Window
		{
			public:
			typedef void* WindowHandle;

			//! Sent when the user attempts to close a window
			struct CloseEvent
			{
				const Window* window;
			};

			//! Sent when a window is resized
			struct ResizeEvent
			{
				const Window* window;
				Point<Int> newSize;
			};

			//! Sent when a window's default framebuffer is resized
			struct FramebufferResizeEvent
			{
				const Window* window;
				Point<Int> newSize;
			};

			//! Sent when the content scale of a window is changed
			struct ContentScaleChangeEvent
			{
				const Window* window;
				Vector2 newScale;
			};

			//! Sent when a window is moved
			struct MoveEvent
			{
				const Window* window;
				Point<Int> newPosition;
			};

			//! Sent when a window is minimized
			struct MinimizeEvent
			{
				const Window* window;
				bool minimized;
			};

			//! Sent when a window is maximized
			struct MaximizeEvent
			{
				const Window* window;
				bool maximized;
			};

			//! Sent when a window is focused
			struct FocusEvent
			{
				const Window* window;
				bool IsFocused;
			};

			//! Sent when a window is refreshed
			struct RefreshEvent
			{
				const Window* window;
			};

			private:
			WindowHandle handle;
			bool raiseStopOnClose;

			public:
			Window(const WindowHints& hints);

			//TODO: Might be able to implement move constructor by updating the user pointer of the window

			inline Window() : Window(WindowHints{}) {}
			Window(const Window&&) = delete;
			Window(Window&&) = delete;
			Window& operator=(const Window&) = delete;
			Window& operator=(Window&&) = delete;
			~Window();

			inline WindowHandle GetHandle() { return handle; }

			bool GetCloseFlag() const;
			void SetCloseFlag(bool value = true);

			Point<Int> GetSize() const;
			Point<Int> GetFramebufferSize() const;
			void GetDecoratedSize(Int* left, Int* top, Int* right, Int* bottom) const;
			void SetSize(const Point<Int>& size);

			Vector2 GetContentScale() const;

			void SetSizeLimit(const Point<Int>& min, const Point<Int>& max);
			void SetAspectRatio(const Point<Int>& ratio);

			Point<Int> GetPosition() const;
			void SetPosition(const Point<Int>& pos);

			void SetTitle(const std::string& title);

			Monitor* GetMonitor() const;
			void SetMonitor(Monitor* monitor);
			void SetMonitor(Monitor* monitor, const Area<Int>& area, Int refreshRate = -1);

			bool IsMaximized() const;
			bool IsMinimized() const;
			void Minimize();
			void Restore();
			void Maximize();

			bool IsVisible() const;
			void Show();
			void Hide();

			bool IsFocused() const;
			void Focus();

			void Notify();

			bool IsTransparent() const;
			Float GetOpacity() const;
			void SetOpacity(Float opacity);

			bool IsResizable() const;
			bool IsHovered() const;
			bool IsDecorated() const;
			bool IsMinimizeOnFocusLoss() const;
			bool IsTopMost() const;
			bool IsFocusOnShow() const;

			ClientAPIType GetClientAPI() const;
			ContextAPIType GetContextAPI() const;
			OpenGLProfileType GetOpenGLProfile() const;
			void GetContextVersion(Int* major, Int* minor, Int* revision) const;
			bool IsOpenGLForwardCompatible() const;
			bool IsOpenGLDebug() const;
			bool IsNoErrorContext() const;

			void SwapBuffers();
		};
	}
}

#endif

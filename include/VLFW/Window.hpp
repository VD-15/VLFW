#ifndef VLFW_WINDOW_HPP
#define VLFW_WINDOW_HPP

#include "Input.hpp"
#include "Monitor.hpp"
#include "Cursor.hpp"

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

		enum class CursorMode
		{
			//! Cursor behaves normally
			Normal =         0x00034001,

			//! Cursor is invisible while over the window
			Hidden =         0x00034002,

			//! Cursor will be invisible and locked to the center of the window
			Locked =         0x00034003
		};

		/*!
		 * \brief Struct containing various hints to influence the creation and
		 * initial state of a window object.
		 */
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

		/*!
		 * \brief Window wrapper class
		 *
		 * Certain functions of this class require an instance of VLFWMain to
		 * be constructed prior in order to function properly.
		 */
		class Window
		{
			public:
			typedef void* WindowHandle;

			/*!
			 * \brief Sent when the user attempts to close a window
			 *
			 * This event does not necessarily mean the window is going to
			 * close nor does it mean the application is about to exit.
			 */
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

			//! Sent when a window enters or leaves a maximized state
			struct MaximizeEvent
			{
				const Window* window;
				bool maximized;
			};

			//! Sent when a window is focused or unfocused
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

			//! Sent when the cursor enters the area of a window
			struct CursorEnterEvent
			{
				const Window* window;
			};

			//! Sent when the cursor enters the area of a window
			struct CursorLeaveEvent
			{
				const Window* window;
			};

			//! Sent when a mouse button is pressed
			struct MouseButtonDownEvent
			{
				const Window* window;
				MouseButton button;
			};

			//! Sent when a mouse button is released
			struct MouseButtonUpEvent
			{
				const Window* window;
				MouseButton button;
			};

			//! Sent when the mouse is moved over a window
			struct MouseMoveEvent
			{
				const Window* window;
				Vector2 position;
			};

			//! Sent when the user scrolls inside a window
			struct ScrollEvent
			{
				const Window* window;
				Vector2 scrollAmount;
			};

			/*!
			 * \brief Sent when a key is pressed
			 */
			struct KeyDownEvent
			{
				//! Window that recieved the event
				const Window* window;

				//! Key that was pressed, may be Key::Unknown
				Key key;

				/*!
				 * \brief Scancode of the key that was pressed
				 *
				 * \par From GLFW's Documentation:
				 * The scancode is unique for every key, regardless of whether
				 * it has a key token. Scancodes are platform-specific but
				 * consistent over time, so keys will have different scancodes
				 * depending on the platform but they are safe to save to disk.
				 */
				Int scancode;
			};

			/*!
			 * \brief Sent when a key is released
			 */
			struct KeyUpEvent
			{
				//! Window that recieved the event
				const Window* window;

				//! Key that was pressed, may be Key::Unknown
				Key key;

				/*!
				 * \brief Scancode of the key that was pressed
				 *
				 * \par From GLFW's Documentation:
				 * The scancode is unique for every key, regardless of whether
				 * it has a key token. Scancodes are platform-specific but
				 * consistent over time, so keys will have different scancodes
				 * depending on the platform but they are safe to save to disk.
				 */
				Int scancode;
			};

			/*!
			 * \brief Sent when a pressed key is repeated by the system
			 */
			struct KeyRepeatEvent
			{
				//! Window that recieved the event
				const Window* window;

				//! Key that was pressed, may be Key::Unknown
				Key key;

				/*!
				 * \brief Scancode of the key that was pressed
				 *
				 * \par From GLFW's Documentation:
				 * The scancode is unique for every key, regardless of whether
				 * it has a key token. Scancodes are platform-specific but
				 * consistent over time, so keys will have different scancodes
				 * depending on the platform but they are safe to save to disk.
				 */
				Int scancode;
			};

			/*!
			 * \brief Sent when a printable character is typed.
			 */
			struct CharTypeEvent
			{
				/*!
				 * \brief The window object that recieved this event.
				 */
				const Window* window;

				/*!
				 * \brief UTF-32 codepoint for the typed character.
				 */
				char32_t codepoint;
			};

			private:
			WindowHandle handle;
			bool raiseStopOnClose;

			public:

			/*!
			 * \brief Polls for any window events, then update the window
			 *
			 * \ts
			 * This function may only be called from the main thread.<br>
			 * This function will not block the calling thread.<br>
			 */
			static void PollEvents();

			/*!
			 *  \brief Waits for a window event to be posted, then update the window
			 *
			 *  \ts
			 *  This function may only be called from the main thread.<br>
			 *  This function will block the calling thread.<br>
			 */
			static void WaitEvents(double timeout);

			/*!
			 * \brief Gets the window whose OpenGL or OpenGL ES context is
			 * current on the calling thread.
			 *
			 * \ts
			 * This function may only be called from the main thread.<br>
			 * This function will not block the calling thread.<br>
			 */
			static Window* GetCurrentContext();

			Window(const WindowHints& hints);

			//TODO: Might be able to implement move constructor by updating the user pointer of the window

			inline Window() : Window(WindowHints{}) {}
			Window(const Window&&) = delete;
			Window(Window&&) = delete;
			Window& operator=(const Window&) = delete;
			Window& operator=(Window&&) = delete;
			~Window();

			inline WindowHandle GetHandle() { return handle; }

			/*!
			 * \brief Makes the OpenGL or OpenGL ES context of the
			 * window current on the calling thread.
			 *
			 * \ts
			 * This function may be called from any thread.<br>
			 * This function will not block the calling thread.<br>
			 */
			void MakeContextCurrent();

			/*!
			 * \brief Returns whether the close flag for this window has been raised.
			 * 
			 * The close flag is an indicator that the window should close at
			 * the nearest available oppertunity. VLFWMain will automatically
			 * close any windows with this flag raised at the beginning of the
			 * next frame if an instance has been constructed.
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 *
			 * \sa SetCloseFlag()
			 */
			bool GetCloseFlag() const;

			/*!
			 * \brief Raises or lowers the close flag for this window.
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 *
			 * \sa GetCloseFlag()
			 */
			void SetCloseFlag(bool value = true);

			/*!
			 * \brief Gets the size of this window in screen units
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 *
			 * \sa GetSize()
			 */
			Point<Int> GetSize() const;

			/*!
			 * \brief Gets the size of the window's framebuffer in pixels
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			Point<Int> GetFramebufferSize() const;

			/*!
			 * \brief Gets the size of each edge of this window's frame.
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			void GetDecoratedSize(Int* left, Int* top, Int* right, Int* bottom) const;

			/*!
			 * \brief Sets the size of the window.
			 *
			 * This function does not take into account the size of any window
			 * decorations.
			 *
			 * \sa GetSize()
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			void SetSize(const Point<Int>& size);

			/*!
			 * \brief Gets the content scale of this window
			 *
			 * \glfw
			 * This function retrieves the content scale for the specified
			 * window. The content scale is the ratio between the current DPI
			 * and the platform's default DPI. This is especially important for
			 * text and any UI elements. If the pixel dimensions of your UI
			 * scaled by this look appropriate on your machine then it should
			 * appear at a reasonable size on other machines regardless of
			 * their DPI and scaling settings. This relies on the system DPI
			 * and scaling settings being somewhat correct. On systems where
			 * each monitors can have its own content scale, the window content
			 * scale will depend on which monitor the system considers the
			 * window to be on.
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			Vector2 GetContentScale() const;

			/*!
			 * \brief Sets the minimum and maximum size limits of the window
			 *
			 * This function does not take into account the size of any window
			 * decorations.
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			void SetSizeLimit(const Point<Int>& min, const Point<Int>& max);
			void SetAspectRatio(const Point<Int>& ratio);

			/*!
			 * \brief Gets the position of the window in screen coordinates
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			Point<Int> GetPosition() const;

			/*!
			 * \brief Sets the position of the window
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			void SetPosition(const Point<Int>& pos);

			/*!
			 * \brief Sets the title of the window.
			 *
			 * \param title A UTF-8 encoded string
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			void SetTitle(const std::string& title);

			/*!
			 * \brief Gets the monitor the window is fullscreen on
			 *
			 * Returns <tt>nullptr</tt> if the window is not fullscreen
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			Monitor* GetMonitor() const;

			/*!
			 * \brief Makes the window fullscreen on a monitor.
			 */
			void SetMonitor(Monitor* monitor);

			/*!
			 * \copydoc SetMonitor(Monitor*)
			 */
			void SetMonitor(Monitor* monitor, const Area<Int>& area, Int refreshRate = -1);

			/*!
			 *  Returns true if the window is maximized
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 *
			 * \sa Maximize()
			 */
			bool IsMaximized() const;

			/*!
			 * Returns true if the window is minimized
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 *
			 * \sa Minimize()
			 */
			bool IsMinimized() const;

			/*!
			 * \brief Minimizes the window
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 *
			 * \sa IsMinimized()
			 * \sa Restore()
			 */
			void Minimize();

			/*!
			 * \brief Restores the window from a minimized or maximized state.
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 *
			 * \sa IsMinimized()
			 * \sa IsMaximized()
			 * \sa Minimize()
			 * \sa Maximize()
			 */
			void Restore();

			/*!
			 * \brief Maximizes the window.
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 *
			 * \sa IsMaximized()
			 * \sa Restore()
			 */
			void Maximize();

			/*!
			 * \brief Returns true if the window is visible.
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 *
			 * \sa Show()
			 * \sa Hide()
			 */
			bool IsVisible() const;

			/*!
			 * \brief Makes an invisible window visible
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 *
			 * \sa IsVisible()
			 * \sa Hide()
			 */
			void Show();

			/*!
			 * \brief Makes a visible window invisible
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			void Hide();
 
			/*!
			 * \brief Returns true if the window has focus.
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 *
			 * \sa Focus()
			 */
			bool IsFocused() const;

			/*!
			 * \brief Focus the window.
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			void Focus();

			/*!
			 * \brief Request user attention to the window
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			void Notify();

			/*!
			 * \brief Returns true if the window is transparent
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 *
			 * \sa WindowHints
			 * \sa GetOpacity()
			 * \sa SetOpacity(Float)
			 */
			bool IsTransparent() const;

			/*!
			 * \brief Gets the opacity of the window
			 *
			 * ALways returns <tt>1.0f</tt> if the window is not transparent
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 *
			 * \sa IsTransparent()
			 * \sa SetOpacity(Float)
			 */
			Float GetOpacity() const;

			/*!
			 * \brief Sets the opacity of the window
			 *
			 * Does nothing if the window is not transparent
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 *
			 * \sa IsTransparent()
			 * \sa GetOpacity()
			 */
			void SetOpacity(Float opacity);

			/*!
			 * \brief Returns true if the window can be resized by the user
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			bool IsResizable() const;

			/*!
			 * \brief Returns true if the mouse is directly over the content area of the window
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			bool IsHovered() const;

			/*!
			 * \brief Returns true if the window has border decorations
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			bool IsDecorated() const;

			/*!
			 * \brief Returns true if the window minimuzes when it loses focus
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			bool IsMinimizeOnFocusLoss() const;

			/*!
			 * \brief Returns true if the window is always on top of other windows
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			bool IsTopMost() const;

			/*!
			 * \brief Returns true if the window regains focus when Show() is called
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			bool IsFocusOnShow() const;

			/*!
			 * \brief Gets the client API that was created for the window
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 *
			 * \sa WindowHints
			 */
			ClientAPIType GetClientAPI() const;

			/*!
			 * \brief Gets the API that was used to create the window's context
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			ContextAPIType GetContextAPI() const;

			/*!
			 * \brief Gets the OpenGL profile used by the context
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			OpenGLProfileType GetOpenGLProfile() const;

			/*!
			 * \brief Gets the version of the window's context
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			void GetContextVersion(Int* major, Int* minor, Int* revision) const;

			/*!
			 * \brief Returns true if the window's OpenGL context is forward compatible
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			bool IsOpenGLForwardCompatible() const;

			/*!
			 * \brief Returns true if the window's OpenGL context is a debug context
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			bool IsOpenGLDebug() const;

			/*!
			 * \brief Returns true if the window's context does not raise errors
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			bool IsNoErrorContext() const;

			/*!
			 * \brief Swaps the front and back buffers of the window
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			void SwapBuffers();

			/*!
			 * \brief Returns true if the window can recieve raw mouse movement
			 *
			 * \glfw
			 * Raw mouse motion is closer to the actual motion of the mouse
			 * across a surface. It is not affected by the scaling and
			 * acceleration applied to the motion of the desktop cursor. That
			 * processing is suitable for a cursor while raw motion is better
			 * for controlling for example a 3D camera. Because of this, raw
			 * mouse motion is only provided when the cursor is disabled.
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 *
			 * \sa IsRawMouseInput()
			 * \sa SetRawMouseInput(bool)
			 */
			bool IsRawMouseInputSupported() const;

			/*!
			 * \brief Returns true if the window is recieving raw mouse movement
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 *
			 * \sa IsRawMouseInputSupported()
			 * \sa SetRawMouseInput(bool)
			 */
			bool IsRawMouseInput() const;

			/*!
			 * \brief Sets whether the window should recieve raw mouse movement
			 *
			 * This function does nothing if raw mouse input is not supported
			 * by the window.
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 *
			 * \sa IsRawMouseInput()
			 * \sa IsRawMouseInputSupported()
			 */
			void SetRawMouseInput(bool rawMouseInput);

			/*!
			 * \brief Sets how the cursor should behave with the window
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			CursorMode GetCursorMode() const;

			/*!
			 * \brief Gets the cursor mode of the window
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			void SetCursorMode(CursorMode mode);

			/*!
			 * \brief Sets the window's cursor to the platform default
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			void ResetCursor();

			/*!
			 * \brief Sets the window's cursor
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			void SetCursor(Cursor& cursor);
		};
	}
}

#endif

#ifndef VLFW_WINDOW_HPP
#define VLFW_WINDOW_HPP

#include "Input.hpp"
#include "Monitor.hpp"
#include "Cursor.hpp"
#include <type_traits>

namespace vlk
{
	namespace vlfw
	{
		enum class ContextAPI
		{
			//! Don't create a client context
			None =           0x00000000,

			//! Create an OpenGL context
			OpenGL =         0x00030001,

			//! Create an OpenGL ES context
			OpenGLES =       0x00030002,

			//! Create a vulkan context
			Vulkan =         0x10000000,
		};

		enum class ContextCreationAPI
		{
			//! Use the system's native API to create a context
			Native =         0x00036001,

			//! Use EGL to create a context
			EGL =            0x00036002,

			//! Use OSMesa to create a context
			OSMesa =         0x00036003
		};

		/*!
		 * Specifies a profile for an OpenGL context to conform to
		 */
		enum class OpenGLProfileType
		{
			//! Use any available OpenGL profile
			Any =            0x00000000,

			//! Use an OpenGL Core profile
			Core =           0x00032001,

			//! Use an OpenGL Compatibility profile
			Compatability =  0x00032002
		};

		/*!
		 * \brief Describes the robustness strategy to employ in the event of
		 * a hardware fault or driver failure.
		 *
		 * Only affects OpenGL contexts.
		 */
		enum class ContextRobustness
		{
			/*!
			 * \brief No robustness strategy
			 */
			None        = 0,

			/*!
			 * \brief The driver will never deliver a reset notification to the
			 * context. This will effectively disallow loss of context state.
			 */
			NoResetNotif = 0x00031001,

			/*!
			 * \brief A reset will result in the loss of all context state,
			 * requiring the recreation of all associated objects.
			 */
			LoseOnReset  = 0x00031002,
		};

		/*!
		 * \brief Behavior to employ when switching OpenGL contexts.
		 *
		 * Only affects OpenGL contexts.
		 */
		enum class ContextReleaseBehavior
		{
			/*!
			 * \brief Use the context creation API's default behavior
			 */
			Any = 0,

			/*!
			 * \brief Flush and pending commands in the graphics pipeline
			 */
			Flush = 0x00035001,

			/*!
			 * \brief Do nothing
			 */
			None = 0x00035002
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
			ContextAPI contextAPI                  = ContextAPI::OpenGL;

			//! The context creation API used to generate the context
			ContextCreationAPI contextCreationAPI  = ContextCreationAPI::Native;

			//! The context robustness strategy to employ
			ContextRobustness robustness           = ContextRobustness::None;

			//! The behaviour to employ when switching OpenGL contexts
			ContextReleaseBehavior releaseBehavior = ContextReleaseBehavior::Any;

			//! The minimum required major version the context must conform to
			Int contextVersionMajor                = 1;

			//! The minimum required minor version the context must conform to
			Int contextVersionMinor                = 0;

			//! Whether the context should not generate any errors
			bool noErrorContext                    = false;

			/*!
			 * \brief A list of OpenGL, OpenGL ES or Vulkan instance extensions
			 * that will be required by the application.
			 *
			 * If using an OpenGL or OpenGL ES context, support will be queried
			 * for each of the extensions in the list and a std::runtime_error
			 * will be thrown if any are not supported.
			 *
			 * If using a Vulkan context, this list will be passed to the
			 * <tt>VkInstanceCreateInfo</tt> struct alone with any extensions
			 * that VLFW requires. A std::runtime_error will be thrown if
			 * any of the extensions are not supported.
			 */
			std::vector<const char*> requiredExtensions = {};

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

			/////////////////////////////////
			//// VULKAN-SPECIFIC OPTIONS ////
			/////////////////////////////////

			/*!
			 * \brief Pointer to a <tt>VkAllocationCallbacks</tt> object to use
			 * for the vulkan instance and surface, if using a vulkan context.
			 */
			const void* allocationCallbacks = nullptr;

			//! Name of the application to pass into
			std::string applicationName     = "";

			//! Major version of the application
			UInt applicationVersionMajor    = 1;

			//! Minor version of the application
			UInt applicationVersionMinor    = 0;

			//! Patch version of the application
			UInt applicationVersionPatch    = 0;

			/*!
			 * \brief A list of names of validation layers to enable.
			 */
			std::vector<const char*> requiredValidationLayers;

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
				Window* window;
			};

			//! Sent when a window is resized
			struct ResizeEvent
			{
				Window* window;
				Point<Int> newSize;
			};

			//! Sent when a window's default framebuffer is resized
			struct FramebufferResizeEvent
			{
				Window* window;
				Point<Int> newSize;
			};

			//! Sent when the content scale of a window is changed
			struct ContentScaleChangeEvent
			{
				Window* window;
				Vector2 newScale;
			};

			//! Sent when a window is moved
			struct MoveEvent
			{
				Window* window;
				Point<Int> newPosition;
			};

			//! Sent when a window is minimized
			struct MinimizeEvent
			{
				Window* window;
				bool minimized;
			};

			//! Sent when a window enters or leaves a maximized state
			struct MaximizeEvent
			{
				Window* window;
				bool maximized;
			};

			//! Sent when a window is focused or unfocused
			struct FocusEvent
			{
				Window* window;
				bool IsFocused;
			};

			//! Sent when a window is refreshed
			struct RefreshEvent
			{
				Window* window;
			};

			//! Sent when the cursor enters the area of a window
			struct CursorEnterEvent
			{
				Window* window;
			};

			//! Sent when the cursor enters the area of a window
			struct CursorLeaveEvent
			{
				Window* window;
			};

			//! Sent when a mouse button is pressed
			struct MouseButtonDownEvent
			{
				Window* window;
				MouseButton button;
			};

			//! Sent when a mouse button is released
			struct MouseButtonUpEvent
			{
				Window* window;
				MouseButton button;
			};

			//! Sent when the mouse is moved over a window
			struct MouseMoveEvent
			{
				Window* window;
				Vector2 position;
			};

			//! Sent when the user scrolls inside a window
			struct ScrollEvent
			{
				Window* window;
				Vector2 scrollAmount;
			};

			/*!
			 * \brief Sent when a key is pressed
			 */
			struct KeyDownEvent
			{
				//! Window that recieved the event
				Window* window;

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
				Window* window;

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
				Window* window;

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
				Window* window;

				/*!
				 * \brief UTF-32 codepoint for the typed character.
				 */
				char32_t codepoint;
			};

			/*!
			 * \brief Sent when a file or directory is 'dropped' onto the window.
			 */
			struct FileDropEvent
			{
				/*!
				 * \brief The window object that recieved this event.
				 */
				Window* window;

				/*!
				 * \brief UTF-8 encoded paths that were dropped on the window.
				 */
				std::vector<std::string> paths;
			};

			private:
			WindowHandle handle;
			bool raiseStopOnClose;
			ContextAPI contextAPI;

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

			/*!
			 * \brief Constructs a window using the provided args
			 *
			 * \param share The window to share a context with.<br>This context
			 * will be destroyed when all windows that share the context have
			 * been closed. Do any necessary cleanup during a CloseEvent
			 */
			Window(const WindowHints& hints, Window* share = nullptr);

			//TODO: Might be able to implement move constructor by updating the user pointer of the window

			inline Window() : Window(WindowHints{}) {}
			Window(const Window&&) = delete;
			Window(Window&&) = delete;
			Window& operator=(const Window&) = delete;
			Window& operator=(Window&&) = delete;
			~Window();

			/*!
			 * \brief Returns the underlying GLFW handle to this window object
			 */
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
			inline ContextAPI GetContextAPI() const { return contextAPI; }

			/*!
			 * \brief Gets the API that was used to create the window's context
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			ContextCreationAPI GetContextCreationAPI() const;

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
			 * \brief Gets the robustness strategy used by this window's OpenGL
			 * context
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			ContextRobustness GetContextRobustness() const;

			/*!
			 * \brief Gets the release behavior used by this window's OpenGL
			 * context
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			ContextReleaseBehavior GetContextReleaseBehavior() const;

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
			 * \brief Returns true if the window's OpenGL context is forward
			 * compatible
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			bool IsOpenGLForwardCompatible() const;

			/*!
			 * \brief Returns true if the window's OpenGL context is a debug
			 * context
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			bool IsOpenGLDebug() const;

			/*!
			 * \brief Returns true if the window's OpenGl context does not
			 * raise errors
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			bool IsNoErrorContext() const;

			/*!
			 * \brief Returns true if the given OpenGL extension is supported
			 * by the implementation
			 *
			 * \ts
			 * This function may be called from any thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			bool IsOpenGLExtensionSupported(const char* extensionName) const;

			/*!
			 * \brief Returns true if the given Vulkan instance extension is
			 * supported by the implementation
			 *
			 * \ts
			 * This function may be called from any thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			static bool IsVulkanExtensionSupported(const char* extensionName);

			/*!
			 * \brief Generic function pointer typedef for loaded OpenGL or
			 * Vulkan extensions.
			 */
			typedef void(* ExtensionProc)(void);

			/*!
			 * \brief Gets the address of a core or extension function of the
			 * context API the window was created with.
			 *
			 * Will return <tt>nullptr</tt> if the function is not supported by
			 * the implementation. If using an OpenGL or OpenGL ES context, it
			 * must be current on the calling thread in order for this to work
			 * properly.
			 *
			 * \param name The name of the process to retrieve
			 *
			 * \sa MakeContextCurrent()
			 */
			ExtensionProc GetProcessAddress(const char* name) const;

			/*!
			 * \brief Typedef for OpenGL loader functions
			 *
			 * One should be able to use this to cast to a GLADloadproc
			 */
			typedef void* (* OpenGLProcessLoader)(const char* name);

			/*!
			 * \brief Gets the address of the OpenGL process loader
			 *
			 * The OpenGL or OpenGL ES context of this window must be current
			 * on the calling thread in order for this function to work.
			 *
			 * The returned value may be passed directly into gladLoadGLLoader:
			 *
			 * \code
			 * Component<Window>* window = Component<Window>::Create(WindowArgs{});
			 * window->MakeContextCurrent();
			 *
			 * // Initialize glad
			 * if (!gladLoadGLLoader((GLADloadproc)window->GetOpenGLProcessLoader()))
			 * {
			 *     throw std::runtime_error("Failed to initialize glad!");
			 * }
			 * \endcode
			 *
			 * \sa MakeContextCurrent()
			 */
			OpenGLProcessLoader GetOpenGLProcessLoader() const;

			/*!
			 * \brief Swaps the front and back buffers of the window
			 *
			 * This function only has an effect if the window was created with
			 * an OpenGL or OpenGL ES context
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			void SwapBuffers();

			/*!
			 * \brief Returns true if the specified queue family of the
			 * specified physical device supports presentation to the window's
			 * surface
			 *
			 * \param instance The <tt>VkInstance</tt> the device belongs to
			 * \param device The <tt>VkPhysicalDevice</tt> the queue family
			 * belongs to
			 * \param queueFamily The index of the queue family to query
			 *
			 * \sa CreateVulkanSurface()
			 */
			bool GetVulkanPresentationSupport(void* physicalDevice, UInt queueFamily) const;

			/*!
			 * \brief Returns the handle to this window's vulkan instance
			 *
			 * The lifetime of the instance is handled by this window object
			 * and is destroyed when the window closes. Do any necessary
			 * cleanup during a CloseEvent
			 *
			 * \returns <tt>nullptr</tt> if the Context API of this window is
			 * anything other than ContextAPI::Vulkan, otherwise returns a
			 * handle to a <tt>VkInstance</tt> object.
			 */
			void* GetVulkanInstance() const;

			/*!
			 * \brief Returns the handle to this window's vulkan surface
			 *
			 * The lifetime of the surface is handled by this window object and
			 * is destroyed when the window closes. Do any necessary cleanup
			 * during a CloseEvent
			 *
			 * \returns <tt>nullptr</tt> if the Context API of this window is
			 * anything other than ContextAPI::Vulkan, otherwise returns a
			 * handle to a <tt>VkSurfaceKHR</tt> object.
			 */
			void* GetVulkanSurface() const;

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

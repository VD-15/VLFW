#ifndef VLFW_MOUSE_HPP
#define VLFW_MOUSE_HPP

#include "ValkyrieEngine/ValkyrieEngine.hpp"

#include "Window.hpp"

namespace vlk
{
	namespace vlfw
	{
		/*!
		 * \brief Static mouse class
		 *
		 * Stores the states of mouse buttons, the position of the mouse and
		 * how far the mouse has scrolled. Buttons that are pressed are
		 * guaranteed to have IsButtonPressed return true for exactly one 
		 * frame, same with released buttons and IsButtonReleased. Buttons may
		 * be pressed and released on the same frame, but IsButtonDown and
		 * IsButtonUp will always report the last known state of the button.
		 * To use this class, one must first construct an instance of
		 * MouseMain.
		 */
		class Mouse
		{
			public:
			static std::string GetButtonName(MouseButton button);

			/*!
			 * \brief Returns true if the given mouse button was held down at
			 * the start of the current frame.
			 *
			 * \ts
			 * This function may be called from any thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			static bool IsButtonDown(MouseButton button);

			/*!
			 * \brief Returns true if the given mouse button was not held down
			 * at the start of the current frame
			 *
			 * \ts
			 * This function may be called from any thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			static bool IsButtonUp(MouseButton button);

			/*!
			 * \brief Returns true if the given mouse button was pressed some
			 * time during the last frame
			 *
			 * \ts
			 * This function may be called from any thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			static bool IsButtonPressed(MouseButton button);

			/*!
			 * \brief Returns true if the given mouse button was released some
			 * time during the last frame
			 *
			 * \ts
			 * This function may be called from any thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			static bool IsButtonReleased(MouseButton button);

			/*!
			 * \brief Gets the position of the mouse
			 *
			 * The returned vector will be measured in screen units relative to
			 * the top-left corner of the window the mouse is currently over.
			 *
			 * \ts
			 * This function may be called from any thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			static Vector2 GetMousePos();

			/*!
			 * \brief Gets the distance the mouse has moved since the last frame
			 *
			 * The returned vector will be measured in screen units. If the
			 * mouse leaves the boundaries of the window, this figure will
			 * not be reported accurately.
			 *
			 * \ts
			 * This function may be called from any thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 *
			 * \sa Window::SetCursorMode(CursorMode)
			 */
			static Vector2 GetMouseDelta();

			/*!
			 * \brief Gets the amount the user has scrolled since the last frame
			 *
			 * \ts
			 * This function may be called from any thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			static Vector2 GetScrollDelta();
		};

		//! Arguments for mouse main class
		struct MouseMainArgs
		{ };

		//! Implements functionality for the Mouse class
		class MouseMain :
			public EventListener<Window::MouseButtonUpEvent>,
			public EventListener<Window::MouseButtonDownEvent>,
			public EventListener<Window::MouseMoveEvent>,
			public EventListener<Window::ScrollEvent>,
			public EventListener<PostUpdateEvent>
		{
			std::unique_lock<std::mutex> lock;

			public:
			inline MouseMain() : MouseMain(MouseMainArgs{}) {};
			MouseMain(const MouseMainArgs&);

			MouseMain(const MouseMain&) = delete;
			MouseMain(MouseMain&&) = delete;
			MouseMain& operator=(const MouseMain&) = delete;
			MouseMain& operator=(MouseMain&&) = delete;
			~MouseMain() = default;

			void OnEvent(const Window::MouseButtonUpEvent&) override;
			void OnEvent(const Window::MouseButtonDownEvent&) override;
			void OnEvent(const Window::MouseMoveEvent&) override;
			void OnEvent(const Window::ScrollEvent&) override;
			void OnEvent(const PostUpdateEvent&) override;
		};
	}
}

#endif

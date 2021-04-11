#ifndef VLFW_KEYBOARD_HPP
#define VLFW_KEYBOARD_HPP

#include "ValkyrieEngine/ValkyrieEngine.hpp"

#include "Window.hpp"

namespace vlk
{
	namespace vlfw
	{
		/*!
		 * \brief Static keyboard class
		 *
		 * Stores the states of keyboard keys. Keys that are pressed are
		 * guaranteed to have IsKeyPressed return true for exactly one frame,
		 * same with released keys and IsKeyReleased. Keys may be pressed and
		 * released on the same frame, but IsKeyDown and IsKeyUp will always
		 * accurately report the latest known state of the key. To use this
		 * class, one must first construct an instance of KeyboardMain.
		 */
		class Keyboard
		{
			public:

			/*!
			 * \brief Gets the scancode of the given key
			 *
			 * \glfw The scancode is unique for every key, regardless of
			 * whether it has a key token. Scancodes are platform-specific but
			 * consistent over time, so keys will have different scancodes
			 * depending on the platform but they are safe to save to disk.
			 *
			 * \ts
			 * This function may be called from any thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			static Int GetKeyScancode(Key key);

			/*!
			 * \brief Gets the human-readable name of the given key
			 *
			 * \ts
			 * This function may be called from any thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			static const std::string& GetKeyName(Key key);

			/*!
			 * \brief Returns true if the given key was held down at the start
			 * of the current frame
			 *
			 * \ts
			 * This function may be called from any thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			static bool IsKeyDown(Int scancode);

			/*!
			 * \copydoc IsKeyDown(Int)
			 */
			inline static bool IsKeyDown(Key key) { return IsKeyDown(GetKeyScancode(key)); }

			/*!
			 * \brief Returns true if the given key was not held down at the
			 * start of the current frame
			 *
			 * \ts
			 * This function may be called from any thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			static bool IsKeyUp(Int scancode);

			/*!
			 * \copydoc IsKeyUp(Int)
			 */
			inline static bool IsKeyUp(Key key) { return IsKeyUp(GetKeyScancode(key)); }

			/*!
			 * \brief Returns true if the given key was pressed some time
			 * during the last frame
			 *
			 * \ts
			 * This function may be called from any thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			static bool IsKeyPressed(Int scancode);

			/*!
			 * \copydoc IsKeyPressed(Int)
			 */
			inline static bool IsKeyPressed(Key key) { return IsKeyPressed(GetKeyScancode(key)); }

			/*!
			 * \brief Returns true if the given key was released some time
			 * during the last frame
			 *
			 * \ts
			 * This function may be called from any thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			static bool IsKeyReleased(Int scancode);

			/*!
			 * \copydoc IsKeyReleased(Int)
			 */
			inline static bool IsKeyReleased(Key key) { return IsKeyReleased(GetKeyScancode(key)); }
		};

		//! Arguments for keyboard main class
		struct KeyboardMainArgs
		{ };

		/*!
		 * \brief Implements functionality for keyboard class
		 */	
		class KeyboardMain final :
			public EventListener<Window::KeyDownEvent>,
			public EventListener<Window::KeyUpEvent>,
			public EventListener<PostUpdateEvent>
		{
			std::unique_lock<std::mutex> lock;

			public:
			inline KeyboardMain() : KeyboardMain(KeyboardMainArgs{}) {};
			KeyboardMain(const KeyboardMainArgs&);

			KeyboardMain(const KeyboardMain&) = delete;
			KeyboardMain(KeyboardMain&&) = delete;
			KeyboardMain& operator=(const KeyboardMain&) = delete;
			KeyboardMain& operator=(KeyboardMain&&) = delete;
			~KeyboardMain() = default;

			void OnEvent(const Window::KeyDownEvent& ev) override;
			void OnEvent(const Window::KeyUpEvent& ev) override;
			void OnEvent(const PostUpdateEvent& ev) override;
		};
	}
}

#endif

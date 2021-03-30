#ifndef VLFW_CURSOR_HPP
#define VLFW_CURSOR_HPP

#include "Input.hpp"

#include "ValkyrieEngineCommon/Vector.hpp"

namespace vlk
{
	namespace vlfw
	{
		enum class DefaultCursorType
		{
			Arrow =      0x00036001,
			IBeam =      0x00036002,
			Crosshair =  0x00036003,
			Hand =       0x00036004,
			HorzResize = 0x00036005,
			VertResize = 0x00036006
		};

		/*!
		 * \brief Hardware cursor wrapper class
		 */
		class Cursor
		{
			void* handle;

			public:

			/*!
			 * \brief Creates a cursor object using the system's cursor theme
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			Cursor(DefaultCursorType type);

			/*!
			 * \brief Creates a cursor with a custom image.
			 *
			 * \param data An array of non-premultiplied RGBA color values
			 * \param size The size, in pixels of the image
			 * \param hotspot The center of the image, measured from the top-left
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			Cursor(unsigned char* data, Point<Int> size, Point<Int> hotspot);

			Cursor() = delete;
			Cursor(const Cursor&) = delete;
			Cursor(Cursor&&) = delete;
			Cursor& operator=(const Cursor&) = delete;
			Cursor& operator=(Cursor&&) = delete;
			~Cursor();

			/*!
			 * \brief Gets the handle of the cursor object.
			 *
			 * This handle is owned by GLFW and functions as a valid
			 * <tt>GLFWcursor*</tt> object should you want to use the library
			 * directly.
			 *
			 * \ts
			 * This function must only be called from the main thread.<br>
			 * Access to this class is not synchronized.<br>
			 * This function will not block the calling thread.<br>
			 */
			inline void* GetHandle() { return handle; }
		};
	}
}

#endif

#include "VLFW/Mouse.hpp"

#include "GLFW/glfw3.h"

using namespace vlk;
using namespace vlfw;

Cursor::Cursor(DefaultCursorType type) :
	handle(glfwCreateStandardCursor(static_cast<Int>(type)))
{ }

Cursor::Cursor(unsigned char* data, Point<Int> size, Point<Int> hotspot)
{
	GLFWimage image;
	image.pixels = data;
	image.height = size.Y();
	image.width = size.X();
	handle = glfwCreateCursor(&image, hotspot.X(), hotspot.Y());
}

Cursor::~Cursor()
{
	glfwDestroyCursor(reinterpret_cast<GLFWcursor*>(handle));
}

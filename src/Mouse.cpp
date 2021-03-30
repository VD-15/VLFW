#include "VLFW/Mouse.hpp"

#include <unordered_set>

using namespace vlk;
using namespace vlfw;

namespace
{
	//! Buttons pressed this frame
	std::unordered_set<MouseButton> pressedSet;
	
	//! Buttons released this frame
	std::unordered_set<MouseButton> releasedSet;

	//! Buttons that are currently down
	std::unordered_set<MouseButton> downSet;

	// Current mouse pos
	Vector2 mousePos;

	// Mouse pos last frame
	Vector2 lastMouse;

	// Scroll performed this frame
	Vector2 scrollDelta;

	std::mutex mtx;
}

std::string Mouse::GetButtonName(MouseButton button)
{
	return std::string("Mouse Button ") + std::to_string(static_cast<Int>(button));
}

MouseMain::MouseMain(const MouseMainArgs&) :
	lock(mtx, std::try_to_lock)
{
	if (!lock.owns_lock())
	{
		throw std::runtime_error("Multiple concurrent instances of MouseMain are disallowed.");
		return;
	}

	pressedSet.clear();
	releasedSet.clear();
	downSet.clear();
	mousePos = Vector2();
	lastMouse = Vector2();
	scrollDelta = Vector2();
}

bool Mouse::IsButtonDown(MouseButton button)
{
	return downSet.find(button) != downSet.end();
}

bool Mouse::IsButtonUp(MouseButton button)
{
	return downSet.find(button) == downSet.end();
}

bool Mouse::IsButtonPressed(MouseButton button)
{
	return pressedSet.find(button) != pressedSet.end();
}

bool Mouse::IsButtonReleased(MouseButton button)
{
	return releasedSet.find(button) != releasedSet.end();
}

Vector2 Mouse::GetMousePos()
{
	return mousePos;
}

Vector2 Mouse::GetMouseDelta()
{
	return lastMouse - mousePos;
}

Vector2 Mouse::GetScrollDelta()
{
	return scrollDelta;
}

void MouseMain::OnEvent(const Window::MouseButtonDownEvent& ev)
{
	pressedSet.insert(ev.button);
	downSet.insert(ev.button);
}

void MouseMain::OnEvent(const Window::MouseButtonUpEvent& ev)
{
	releasedSet.insert(ev.button);
	downSet.erase(ev.button);
}

void MouseMain::OnEvent(const Window::MouseMoveEvent& ev)
{
	mousePos = ev.position;
}

void MouseMain::OnEvent(const Window::ScrollEvent& ev)
{
	scrollDelta += ev.scrollAmount;
}

void MouseMain::OnEvent(const PostUpdateEvent&)
{
	pressedSet.clear();
	releasedSet.clear();
	lastMouse = mousePos;
	scrollDelta = Vector2();
}

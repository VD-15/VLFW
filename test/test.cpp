#include "VLFW/VLFW.hpp"
#include "ValkyrieEngine/ValkyrieEngine.hpp"

#include <ios>
#include <iostream>

using namespace vlk;
using namespace vlk::vlfw;

class Logger :
	public EventListener<vlfw::ErrorEvent>,
	public EventListener<Window::CloseEvent>,
	public EventListener<Window::ResizeEvent>,
	public EventListener<Window::FramebufferResizeEvent>,
	public EventListener<Window::ContentScaleChangeEvent>,
	public EventListener<Window::MoveEvent>,
	public EventListener<Window::MinimizeEvent>,
	public EventListener<Window::MaximizeEvent>,
	public EventListener<Window::FocusEvent>,
	public EventListener<Window::RefreshEvent>,
	public EventListener<Window::KeyUpEvent>,
	public EventListener<Window::KeyDownEvent>,
	public EventListener<Window::MouseButtonUpEvent>,
	public EventListener<Window::MouseButtonDownEvent>,
	public EventListener<Window::MouseMoveEvent>,
	public EventListener<Window::ScrollEvent>,
	public EventListener<UpdateEvent>
{
	public:
	void OnEvent(const vlfw::ErrorEvent& ev) override
	{
		std::cout << ev.what << '\n';
	}

	void OnEvent(const Window::CloseEvent&) override
	{
		std::cout << "CloseEvent\n";
	}

	void OnEvent(const Window::ResizeEvent&) override
	{
		std::cout << "ResizeEvent\n";
	}

	void OnEvent(const Window::FramebufferResizeEvent&) override
	{
		std::cout << "FramebufferResizeEvent\n";
	}

	void OnEvent(const Window::ContentScaleChangeEvent&) override
	{
		std::cout << "ContentScaleChangeEvent\n";
	}

	void OnEvent(const Window::MoveEvent&) override
	{
		std::cout << "MoveEvent\n";
	}

	void OnEvent(const Window::MinimizeEvent&) override
	{
		std::cout << "MinimizeEvent\n";
	}

	void OnEvent(const Window::MaximizeEvent&) override
	{
		std::cout << "MaximizeEvent\n";
	}

	void OnEvent(const Window::FocusEvent&) override
	{
		std::cout << "FocusEvent\n";
	}

	void OnEvent(const Window::RefreshEvent&) override
	{
		std::cout << "RefreshEvent\n";
	}

	void OnEvent(const Window::KeyDownEvent& ev) override
	{
		std::cout << "KeyDownEvent "
		          << Keyboard::GetKeyName(ev.key)
				  << std::endl;
	}

	void OnEvent(const Window::KeyUpEvent& ev) override
	{
		std::cout << "KeyUpEvent "
		          << Keyboard::GetKeyName(ev.key)
				  << std::endl;
	}

	void OnEvent(const Window::MouseMoveEvent& ev) override
	{
		std::cout << "MouseMoveEvent "
		          << std::to_string(ev.position.X())
		          << ", "
		          << std::to_string(ev.position.Y())
		          << std::endl;
	}

	void OnEvent(const UpdateEvent&) override
	{
		if (Keyboard::IsKeyPressed(Key::Space)) std::cout << "Space pressed\n";
		if (Keyboard::IsKeyReleased(Key::Space)) std::cout << "Space released\n";
		if (Mouse::IsButtonPressed(MouseButton::Button0)) std::cout << "LMB pressed\n";
		if (Mouse::IsButtonReleased(MouseButton::Button0)) std::cout << "LMB released\n";
	}

	void OnEvent(const Window::MouseButtonUpEvent& ev) override
	{
		std::cout << "MouseButtonDownEvent "
		          << Mouse::GetButtonName(ev.button)
				  << std::endl;
	}

	void OnEvent(const Window::MouseButtonDownEvent& ev) override
	{
		std::cout << "MouseButtonUpEvent "
		          << Mouse::GetButtonName(ev.button)
				  << std::endl;
	}

	void OnEvent(const Window::ScrollEvent& ev) override
	{
		std::cout << "ScrollEvent "
		          << std::to_string(ev.scrollAmount.X())
		          << ", "
		          << std::to_string(ev.scrollAmount.Y())
		          << std::endl;
	}
};

int main()
{
	Logger l;
	VLFWMain* vlfwMain = new VLFWMain();
	KeyboardMain* keyboardMain = new KeyboardMain();

	auto* w = Component<Window>::Create(0);

	vlk::Application::Start(ApplicationArgs{});

	delete keyboardMain;
	delete vlfwMain;
}

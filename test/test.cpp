#include "VLFW/VLFW.hpp"
#include "ValkyrieEngine/ValkyrieEngine.hpp"

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
	public EventListener<Window::RefreshEvent>
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
};

int main()
{
	Logger l;
	VLFWMain* vlfwMain = new VLFWMain();

	auto* w = Component<Window>::Create(0);

	vlk::Application::Start(ApplicationArgs{});

	delete vlfwMain;
}

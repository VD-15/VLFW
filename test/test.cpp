#include "VLFW/VLFW.hpp"
#include "ValkyrieEngine/ValkyrieEngine.hpp"

#include "glad/glad.h"
#include "vulkan/vulkan.h"

#include <chrono>
#include <exception>
#include <ios>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <set>

using namespace vlk;
using namespace vlk::vlfw;

std::string GLSourceToString(UInt source)
{
	switch (source)
	{
		case 0x8246:
			return "API";
		case 0x8247:
			return "Window System";
		case 0x8248:
			return "Shader Compiler";
		case 0x8249:
			return "Third Party";
		case 0x824A:
			return "Application";
		case 0x824B:
		default:
			return "Other";
	}
}

std::string GLTypeToString(UInt type)
{
	switch (type)
	{
		case 0x824C:
			return "Error";
		case 0x824D:
			return "Deprecated";
		case 0x824E:
			return "Undefined";
		case 0x824F:
			return "Portability";
		case 0x8250:
			return "Performance";
		case 0x8268:
			return "Marker";
		case 0x8269:
			return "Push group";
		case 0x826A:
			return "Pop group";
		case 0x8251:
		default:
			return "Other";
	}
}

std::string GLSeverityToString(UInt severity)
{
	switch (severity)
	{
		case 0x9146:
			return "High";
		case 0x9147:
			return "Medium";
		case 0x9148:
			return "Low";
		case 0x826B:
			return "Notification";
		default:
			return "Unknown";
	}
}

void GLErrorCallback(UInt source, UInt type, UInt id, UInt severity, Int length, const char* message, const void* _)
{
	std::cout << "OpenGL Message:" <<
	" Severity: " << GLSeverityToString(severity) <<
	" Source: " << GLSourceToString(source) <<
	" Type: " << GLTypeToString(type) << "\n" <<
	"Message:\n" << message << "\n\n";
}

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

		if (ev.key == Key::Escape)
		{
			ev.window->SetCloseFlag();
		}
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

class OpenGLTester :
	public EventListener<PostUpdateEvent>,
	public EventListener<VLFWMain::RenderWaitEvent>,
	public EventListener<Window::FramebufferResizeEvent> 
{
	std::chrono::steady_clock::time_point last;
	// Formatted xyz-rgb
	std::vector<float> verts;
	float time;

	void UpdateVerts()
	{
		verts =
		{
			Sin(time + 0.f * HalfPi), Cos(time + 0.f * HalfPi), 0.f, 1.f, 0.f, 0.f,
			Sin(time + 1.f * HalfPi), Cos(time + 1.f * HalfPi), 0.f, 0.f, 1.f, 0.f,
			Sin(time + 2.f * HalfPi), Cos(time + 2.f * HalfPi), 0.f, 0.f, 0.f, 1.f,
			Sin(time + 3.f * HalfPi), Cos(time + 3.f * HalfPi), 0.f, 1.f, 1.f, 0.f,
		};
	}

	void CheckShader(UInt shader)
	{
		int success;
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

		if (!success)
		{
			int bufLength;
			glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &bufLength);

			char* buffer = new char[bufLength];
			glGetShaderInfoLog(shader, bufLength, nullptr, buffer);
			std::cout << "Shader compilation failed:\n" << buffer << "\n";
			delete[] buffer;

			glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &bufLength);
			buffer = new char[bufLength];
			glGetShaderSource(shader, bufLength, nullptr, buffer);
			std::cout << "Shader Source:" << buffer << "{end}\n";
			delete[] buffer;

			throw std::runtime_error("Shader compilation failed.");
		}
		else
		{
			std::cout << "Shader compilation successful\n";
		}
	}

	public:
	OpenGLTester(const VLFWMain* vlfw)
	{
		if (!gladLoadGLLoader((GLADloadproc)vlfw->GetOpenGLProcessLoader()))
		{
			throw std::runtime_error("Failed to initialize glad");
		}

		// Debug
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(GLErrorCallback, nullptr);

		// Setup
		time = 0.f;
		UpdateVerts();
		glViewport(0, 0, 640, 480);
		UInt indices[] = { 0, 1, 2, 0, 2, 3 };

		// Vertex array object
		UInt vao = 0;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		// Array buffer
		UInt vbo = 0;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_DYNAMIC_DRAW);

		// Element Buffer
		UInt ebo = 0;
		glGenBuffers(1, &ebo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * 6, indices, GL_STATIC_DRAW);

		// Shader sources
		const char* vertexSource =
R"source(
#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inCol;

out vec3 moveCol;

void main()
{
    gl_Position = vec4(inPos, 1.0);
    moveCol = inCol;
}
)source";

		const char* fragSource =
R"source(
#version 330 core
in vec3 moveCol;

layout (location = 0) out vec4 outCol;

void main()
{
    outCol = vec4(moveCol, 1.0);
}
)source";

		// Vertex Shader
		UInt vertShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertShader, 1, &vertexSource, nullptr);
		glCompileShader(vertShader);
		CheckShader(vertShader);

		// Fragment Shader
		UInt fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragShader, 1, &fragSource, nullptr);
		glCompileShader(fragShader);
		CheckShader(fragShader);

		// Shader Program
		UInt program = glCreateProgram();
		glAttachShader(program, vertShader);
		glAttachShader(program, fragShader);
		glLinkProgram(program);

		Int success = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			Int bufLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);

			char* buffer = new char[bufLength];
			glGetProgramInfoLog(program, bufLength, nullptr, buffer);
			std::cout << "Program link failed:\n" << buffer << "\n";
			delete[] buffer;

			throw std::runtime_error("Program link failed.");
		}
		else
		{
			std::cout << "Program link successful\n";
		}

		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
		glUseProgram(program);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), nullptr);
		glEnableVertexAttribArray(0);

		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * 4));
		glEnableVertexAttribArray(1);

		last = std::chrono::steady_clock::now();
	}

	void OnEvent(const Window::FramebufferResizeEvent& ev) override
	{
		glViewport(0, 0, ev.newSize.X(), ev.newSize.Y());
	}

	void OnEvent(const VLFWMain::RenderWaitEvent& ev) override
	{
		glFinish();
	}

	void OnEvent(const PostUpdateEvent& ev) override
	{
		auto now = std::chrono::steady_clock::now();
		float diff = std::chrono::duration_cast<std::chrono::microseconds>(now - last).count() / 1000000.f;
		last = now;
		time += diff;
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT);

		UpdateVerts();
		glBufferSubData(GL_ARRAY_BUFFER, 0, verts.size() * sizeof(float), verts.data());
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
	}
};

int main()
{
	Logger l;
	VLFWMainArgs args {};
	args.waitForRenderer = true;
	VLFWMain* vlfwMain = new VLFWMain();
	KeyboardMain* keyboardMain = new KeyboardMain();

	WindowHints hints{};
	auto* w = Component<Window>::Create(0, hints);

	w->SetTitle("OpenGL test");
	w->MakeContextCurrent();
	vlfwMain->SetSwapInterval(0);
	auto* opengltest = new OpenGLTester(vlfwMain);

	vlk::Application::Start(ApplicationArgs{});

	delete opengltest;
	delete keyboardMain;
	delete vlfwMain;
}

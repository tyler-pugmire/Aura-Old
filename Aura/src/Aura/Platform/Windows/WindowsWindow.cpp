#include "pch.h"
#include "WindowsWindow.h"
#include "Aura/Core/Logger.h"
#include "Aura/Core/Events/WindowEvent.h"
#include "Aura/Core/Input/Input.h"
#include "Aura/Core/Events/KeyEvent.h"
#include "Aura/Core/Events/MouseEvent.h"

#include <GLFW/glfw3.h>
#include <imgui.h>


namespace Aura
{
	static void GLFWErrorCallback(int error, const char* description)
	{
		AURA_CORE_ERROR_TAG("GLFW", "GLFW Error ({0}): {1}", error, description);
	}

	static bool s_GLFWInitialized = false;

	WindowsWindow::WindowsWindow(const WindowSpecification& spec) : m_specification(spec)
	{

	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init()
	{
		m_data.title = m_specification.title;
		m_data.width = m_specification.width;
		m_data.height = m_specification.height;

		AURA_CORE_INFO_TAG("GLFW", "Creating window {0} ({1}, {2})", m_data.title, m_data.width, m_data.height);

		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			glfwSetErrorCallback(GLFWErrorCallback);

			s_GLFWInitialized = true;
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		if (!m_specification.decorated)
		{
			glfwWindowHint(GLFW_DECORATED, false);
		}

		if (m_specification.fullscreen)
		{
			GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
			const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);

			glfwWindowHint(GLFW_DECORATED, false);
			glfwWindowHint(GLFW_RED_BITS, mode->redBits);
			glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
			glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
			glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

			m_window = glfwCreateWindow(mode->width, mode->height, m_data.title.c_str(), primaryMonitor, nullptr);
		}
		else
		{
			m_window = glfwCreateWindow((int)m_data.width, (int)m_data.height, m_data.title.c_str(), nullptr, nullptr);
		}

		glfwSetWindowUserPointer(m_window, &m_data);

		if (glfwRawMouseMotionSupported())
			glfwSetInputMode(m_window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
		else
			AURA_CORE_WARN("GLFW", "Raw mouse motion not supported");

		m_rendererContext = RendererContext::Create();
		m_rendererContext->Init();

		glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height)
			{
				auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

				WindowResizeEvent event((uint32_t)width, (uint32_t)height);
				data.eventCallback(event);
				data.width = width;
				data.height = height;
			});

		glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window)
			{
				auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

				WindowClosedEvent event;
				data.eventCallback(event);
			});

		glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				auto& data = *((WindowData*)glfwGetWindowUserPointer(window));
				
				switch (action)
				{
				case GLFW_PRESS:
					{
						Input::UpdateKeyState((KeyCode)key, KeyState::Pressed);
						KeyPressedEvent event((KeyCode)key, 0);
						data.eventCallback(event);
						break;
					}
				case GLFW_RELEASE:
					{
						Input::UpdateKeyState((KeyCode)key, KeyState::Released);
						KeyReleasedEvent event((KeyCode)key);
						data.eventCallback(event);
						break;
					}
				case GLFW_REPEAT:
					{
						Input::UpdateKeyState((KeyCode)key, KeyState::Pressed);
						KeyPressedEvent event((KeyCode)key, 1);
						data.eventCallback(event);
						break;
					}
				}
			});

		glfwSetCharCallback(m_window, [](GLFWwindow* window, uint32_t codepoint)
			{
				auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

				KeyTypedEvent event((KeyCode)codepoint);
				data.eventCallback(event);
			});

		glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods)
			{
				auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

				switch (action)
				{
				case GLFW_PRESS:
					{
						Input::UpdateButtonState((MouseButton)button, KeyState::Pressed);
						MouseButtonPressedEvent event((MouseButton)button);
						data.eventCallback(event);
						break;
					}
				case GLFW_RELEASE:
					{
						Input::UpdateButtonState((MouseButton)button, KeyState::Released);
						MouseButtonReleasedEvent event((MouseButton)button);
						data.eventCallback(event);
						break;
					}
				case GLFW_REPEAT:
					{
						Input::UpdateButtonState((MouseButton)button, KeyState::Held);
						MouseButtonDownEvent event((MouseButton)button);
						data.eventCallback(event);
						break;
					}
				}
			});

		glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				auto& data = *((WindowData*)glfwGetWindowUserPointer(window));

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.eventCallback(event);
			});

		glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double x, double y)
			{
				auto& data = *((WindowData*)glfwGetWindowUserPointer(window));
				MouseMovedEvent event((float)x, (float)y);
				data.eventCallback(event);
			});

		m_imguiMouseCursors[ImGuiMouseCursor_Arrow] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);
		m_imguiMouseCursors[ImGuiMouseCursor_TextInput] = glfwCreateStandardCursor(GLFW_IBEAM_CURSOR);
		m_imguiMouseCursors[ImGuiMouseCursor_ResizeAll] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);   // FIXME: GLFW doesn't have this.
		m_imguiMouseCursors[ImGuiMouseCursor_ResizeNS] = glfwCreateStandardCursor(GLFW_VRESIZE_CURSOR);
		m_imguiMouseCursors[ImGuiMouseCursor_ResizeEW] = glfwCreateStandardCursor(GLFW_HRESIZE_CURSOR);
		m_imguiMouseCursors[ImGuiMouseCursor_ResizeNESW] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
		m_imguiMouseCursors[ImGuiMouseCursor_ResizeNWSE] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR);  // FIXME: GLFW doesn't have this.
		m_imguiMouseCursors[ImGuiMouseCursor_Hand] = glfwCreateStandardCursor(GLFW_HAND_CURSOR);

		// Update window size to actual size
		{
			int width, height;
			glfwGetWindowSize(m_window, &width, &height);
			m_data.width = width;
			m_data.height = height;
		}

	}
	void WindowsWindow::ProcessEvents()
	{
		glfwPollEvents();
		Input::Update();
	}
	void WindowsWindow::SwapBuffer()
	{

	}

	void WindowsWindow::Shutdown()
	{
		glfwTerminate();
		s_GLFWInitialized = false;
	}


	std::pair<float, float> WindowsWindow::GetWindowPos() const
	{
		return { 0, 0 };
	}

	void WindowsWindow::Maximize()
	{

	}
	void WindowsWindow::CenterWindow()
	{

	}

	void WindowsWindow::SetEventCallback(EventCallbackFn const& fn)
	{
		m_data.eventCallback = fn;
	}

	void WindowsWindow::SetVSync(bool enabled)
	{

	}
	bool WindowsWindow::IsVSync() const
	{
		return false;
	}
	void WindowsWindow::SetResizable(bool resizable) const
	{

	}

	void WindowsWindow::SetTitle(const std::string& title)
	{

	}
}
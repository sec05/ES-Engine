#include "espch.h"
#include "WindowsWindow.h"
#include "ES/Events/Event.h"
#include "ES/Events/KeyEvent.h"
#include "ES/Events/ApplicationEvent.h"
#include "ES/Events/MouseEvent.h"
#include "ES/Platform/OpenGL/OpenGLContext.h"

namespace ES
{
	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		ES_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
	}
	Window* Window::Create(const WindowProps& props)
	{
		//makes the window
		return new WindowsWindow(props);
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		ES_PROFILE_FUNCTION();
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;


		ES_CORE_INFO("Creating window {0} ({1} x {2})", props.Title, props.Width, props.Height);
		

		if (!s_GLFWInitialized)//if glfw fails
		{
			ES_PROFILE_FUNCTION();
			int sucess = glfwInit();
			ES_CORE_ASSERT(sucess, "Could not init GLFW!");
			glfwSetErrorCallback(GLFWErrorCallback);
			s_GLFWInitialized = true;
		}

		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);//makes the window via glfw glfwGetPrimaryMonitor() makes it fullscreen
		
		m_Context = new OpenGLContext(m_Window);
		m_Context->Init();
		
		
		
		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks

		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.Width = width;
				data.Height = height;

				WindowResizeEvent event(width, height);
				data.EventCallback(event);

			});
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				
				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}

				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
				}
			});
		glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				KeyTypedEvent event(keycode);
				data.EventCallback(event);
			});
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) 
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
					break;
				}
				}
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event((float)xPos, (float)yPos);
				data.EventCallback(event);
			});
	}

	void WindowsWindow::Shutdown()
	{
		ES_PROFILE_FUNCTION();
		glfwDestroyWindow(m_Window);
		
	}
	void WindowsWindow::OnUpdate()
	{
		ES_PROFILE_FUNCTION();
		glfwPollEvents();
		m_Context->SwapBuffers();
		//glfwSwapBuffers(m_Window);//waits amount of windows before swapping buffers
	}
	void WindowsWindow::SetVSync(bool enabled)
	{
		ES_PROFILE_FUNCTION();
		if (enabled)

			glfwSwapInterval(1);//vsync
		else
			glfwSwapInterval(0);//no vsync

		m_Data.VSync = enabled;
	}
	bool WindowsWindow::IsVsync() const
	{
		return m_Data.VSync;
	}
}
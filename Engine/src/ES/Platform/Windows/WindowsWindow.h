#pragma once

#include "ES/Core/Window.h"
#include "ES/Renderer/GraphicsContext.h"
#include <GLFW/glfw3.h>
namespace ES
{
	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);//from window class
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_Data.Width; }
		inline unsigned int GetHeight() const override { return m_Data.Height; }
		inline std::string  GetTitle() const { return m_Data.Title; }
		//Window Attributes
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
		void SetVSync(bool enabled) override;
		bool IsVsync() const override;
		inline void* GetNativeWindow() const { return m_Window; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;
		GraphicsContext* m_Context;
		//can pass struct instead of whole class data
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;

			EventCallbackFn EventCallback;
		};
		WindowData m_Data;
	};
}
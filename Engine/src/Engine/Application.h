#pragma once
#include "Core.h"
#include "Engine/Events/Event.h"
#include "Window.h"
#include "Engine/Events/ApplicationEvent.h"
#include "Engine/LayerStack.h"
#include "Core.h"
#include "Engine/ImGui/ImGuiLayer.h"
#include "Engine/Core/Timestep.h"
namespace Engine {
	class   Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
		void OnEvent(Event& e);
		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline  static Application& Get() { return *s_Instance; }
		inline Window& GetWindow() { return *m_Window; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
	private:
		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;
		float m_LastFrameTime = 0.0f;
	private:
		static Application* s_Instance;
	};
	Application* CreateApplication();//defined in client
}

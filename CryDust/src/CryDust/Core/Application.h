#pragma once

#include "CryDust/Core/Base.h"
#include "CryDust/Events/Event.h"

#include "CryDust/Events/ApplicationEvent.h"
#include "CryDust/Core/Window.h"
#include "CryDust/Core/LayerStack.h"
#include "CryDust/ImGui/ImGuiLayer.h"
#include "CryDust/Renderer/Shader.h"
#include "CryDust/Renderer/Buffer.h"
#include "CryDust/Renderer/VertexArray.h"
#include "CryDust/Core/Timestep.h"

#include "CryDust/ImGui/ImGuiLayer.h"

int main(int argc, char** argv);

namespace CryDust {

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		char** Args = nullptr;
		const char* operator[](int index) const
		{
			CORE_DEBUG_ASSERT(index < Count);
			return Args[index];
		}
	};


	struct ApplicationSpecification
	{
		std::string Name = "CryDust Application";
		std::string WorkingDirectory;
		ApplicationCommandLineArgs CommandLineArgs;
	};

	class  Application
	{
	public:
		Application(const ApplicationSpecification& specification);

		virtual ~Application();
		void OnEvent(Event& e);



		void PushLayer(Layer* layer);
		void PushOverlay(Layer* later);


		inline Window& GetWindow() { return *m_Window; }
		void Close();

		ImGuiLayer* GetImGuiLayer() { return m_ImGuiLayer; }

		static Application& Get() { return *s_Instance; }

		const ApplicationSpecification& GetSpecification() const { return m_Specification; }

		void SubmitToMainThread(const std::function<void()>& function);
	private:

		void Run();
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
		void ExecuteMainThreadQueue();	
	private:
		ApplicationSpecification m_Specification;

		Scope<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;

		bool m_Running = true;
		bool m_Minimized = false;

		LayerStack m_LayerStack;

		float m_LastFrameTime = 0.0f;


		std::vector<std::function<void()>> m_MainThreadQueue;
		std::mutex m_MainThreadQueueMutex;
	private:

		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	//To be defined in client
	Application* CreateApplication(ApplicationCommandLineArgs args);

}

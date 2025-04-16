
#include <CryDust.h>
#include <CryDust/Core/EntryPoint.h>


#include "Sandbox2D.h"
#include "ExampleLayer.h"

class Sandbox : public CryDust::Application
{
public:
	Sandbox(const CryDust::ApplicationSpecification& specification)
		: CryDust::Application(specification)
	{
		//PushLayer(new ExampleLayer());	//推送到层栈
		PushLayer(new Sandbox2D());
		//PushOverlay(new CryDust::ImGuiLayer());
	}

	~Sandbox()
	{
	}
};


CryDust::Application* CryDust::CreateApplication(ApplicationCommandLineArgs args)
{
	ApplicationSpecification spec;
	spec.Name = "Sandbox";
	spec.WorkingDirectory = "../Hazelnut";
	spec.CommandLineArgs = args;

	return new Sandbox(spec);
}

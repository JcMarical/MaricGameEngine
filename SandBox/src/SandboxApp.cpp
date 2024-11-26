#include <CryDust.h>

class ExampleLayer : public CryDust::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		DEBUG_INFO("ExampleLayer::Update");
	}

	void OnEvent(CryDust::Event& event) override
	{
		DEBUG_TRACE("{0}", event.ToString());
	}


};

class Sandbox : public CryDust::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());	//推送到层栈
		PushOverlay(new CryDust::ImGuiLayer());
	}

	~Sandbox()
	{

	}
};


CryDust::Application* CryDust::CreateApplication()
{
	return new Sandbox();
}
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
		if (CryDust::Input::IsKeyPressed(CD_KEY_TAB))
			DEBUG_TRACE("Tab key is pressed (poll)!");
	}

	void OnEvent(CryDust::Event& event) override
	{
		DEBUG_TRACE("{0}", event.ToString());
		if (event.GetEventType() == CryDust::EventType::KeyPressed)
		{
			CryDust::KeyPressedEvent& e = (CryDust::KeyPressedEvent&)event;
			if (e.GetKeyCode() == CD_KEY_TAB)
				DEBUG_TRACE("Tab key is pressed (event)!");
			DEBUG_TRACE("{0}", (char)e.GetKeyCode());
		}
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
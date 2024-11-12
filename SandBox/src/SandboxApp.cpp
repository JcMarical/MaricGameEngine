#include <CryDust.h>

class Sandbox : public CryDust::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox()
	{

	}
};


CryDust::Application* CryDust::CreateApplication()
{
	return new Sandbox();
}
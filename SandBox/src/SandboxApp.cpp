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

int main()
{
	printf("CryDust MariGameEngine!!!\n");
	Sandbox* sandBox = new Sandbox();
	sandBox->Run();
	delete sandBox;
}

CryDust::Application* CreateApplication()
{
	return new Sandbox();
}
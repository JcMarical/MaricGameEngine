#include "cdpch.h"
#include "OpenGLContext.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>
namespace CryDust {
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		: m_WindowHandle(windowHandle)
	{
		CORE_DEBUG_ASSERT(windowHandle, "Window handle is null!")
	}
	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		CORE_DEBUG_ASSERT(status, "Failed to initialize Glad!");

	//	CORE_DEBUG_INFO("OpenGL Info:");
		//CORE_DEBUG_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
		//CORE_DEBUG_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
		//CORE_DEBUG_INFO("  Version: {0}", glGetString(GL_VERSION));
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}
#include "cdpch.h"
#include "Shader.h"
#include <glad/glad.h>
namespace CryDust {
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc)
	{
		// 创建空顶点着色器句柄
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		//·发送顶点着色器源代码到GL
		//·注意std::string的。c_str是NULL字符终止。
		const GLchar* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);
		// 编译顶点着色器
		glCompileShader(vertexShader);
		GLint isCompiled = 0;
		//判断编译状态
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		
		//编译失败
		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);
			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
			// We don't need the shader anymore.
			glDeleteShader(vertexShader);
			CORE_DEBUG_ERROR("{0}", infoLog.data());
			CORE_DEBUG_ASSERT(false, "Vertex shader compilation failure!");
			return;
		}
		// 片元着色器，创建方式同理
		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		// Send the fragment shader source code to GL
		// Note that std::string's .c_str is NULL character terminated.
		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);
		// Compile the fragment shader
		glCompileShader(fragmentShader);
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);

		if (isCompiled == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);
			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);
			// We don't need the shader anymore.
			glDeleteShader(fragmentShader);
			// Either of them. Don't leak shaders.
			glDeleteShader(vertexShader);
			CORE_DEBUG_ERROR("{0}", infoLog.data());
			CORE_DEBUG_ASSERT(false, "Fragment shader compilation failure!");
			return;
		}
		// Vertex and fragment shaders are successfully compiled.
		// Now time to link them together into a program.
		// Get a program object.

		//两个都编译成功，获得个程序物体，赋予id
		m_RendererID = glCreateProgram();
		GLuint program = m_RendererID;
		// 给程序附加上两个着色器
		glAttachShader(program, vertexShader);
		glAttachShader(program, fragmentShader);
		// 链接着色器文件
		glLinkProgram(program);
		// Note the different functions here: glGetProgram* instead of glGetShader*.
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		//处理链接失败
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);
			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			// We don't need the program anymore.
			glDeleteProgram(program);
			// Don't leak shaders either.
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);
			CORE_DEBUG_ERROR("{0}", infoLog.data());
			CORE_DEBUG_ASSERT(false, "Shader link failure!");
			return;
		}
		// 链接后，成不成功都删除着色器了
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}
	Shader::~Shader()
	{
		//删除程序
		glDeleteProgram(m_RendererID);
	}
	void Shader::Bind() const
	{
		//使用着色器程序
		glUseProgram(m_RendererID);
	}
	void Shader::Unbind() const
	{
		glUseProgram(0);
	}
}
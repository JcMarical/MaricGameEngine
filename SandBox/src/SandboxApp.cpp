#include <CryDust.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>

class ExampleLayer : public CryDust::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		//构造初始数据
		m_VertexArray.reset(CryDust::VertexArray::Create());

		float vertices[3 * 7] = {
					-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
					 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
					 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		///设置顶点缓冲
		std::shared_ptr<CryDust::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(CryDust::VertexBuffer::Create(vertices, sizeof(vertices)));
		CryDust::BufferLayout layout = {
			{ CryDust::ShaderDataType::Float3, "a_Position" },
			{ CryDust::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);



		//设置索引数组
		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<CryDust::IndexBuffer> indexBuffer;
		indexBuffer.reset(CryDust::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		///新增正方形顶点数组
		m_SquareVA.reset(CryDust::VertexArray::Create());
		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		///正方形顶点缓冲
		std::shared_ptr<CryDust::VertexBuffer> squareVB;
		squareVB.reset(CryDust::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ CryDust::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);


		///正方形索引换成
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<CryDust::IndexBuffer> squareIB;
		squareIB.reset(CryDust::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);
		//着色器程序
		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			
			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);	
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";
		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;



			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";
		m_Shader.reset(new CryDust::Shader(vertexSrc, fragmentSrc));


		///正方形Shader
		std::string blueShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;


			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);	
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string blueShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(0.2, 0.3, 0.8, 1.0);
			}
		)";

		m_BlueShader.reset(new CryDust::Shader(blueShaderVertexSrc, blueShaderFragmentSrc));

	}

	virtual void OnImGuiRender() override
	{

	}
	
	void OnUpdate(CryDust::Timestep ts) override
	{
		//相机控制
		if (CryDust::Input::IsKeyPressed(CD_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (CryDust::Input::IsKeyPressed(CD_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (CryDust::Input::IsKeyPressed(CD_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (CryDust::Input::IsKeyPressed(CD_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (CryDust::Input::IsKeyPressed(CD_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		if (CryDust::Input::IsKeyPressed(CD_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		//渲染指令
		CryDust::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		CryDust::RenderCommand::Clear();

		//相机位置
		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		//局内渲染
		CryDust::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				CryDust::Renderer::Submit(m_BlueShader, m_SquareVA, transform);
			}
		}
		CryDust::Renderer::Submit(m_Shader, m_VertexArray);

		CryDust::Renderer::EndScene();
	}



	void OnEvent(CryDust::Event& event) override
	{

	}
	private:
		std::shared_ptr<CryDust::Shader> m_Shader;
		std::shared_ptr<CryDust::VertexArray> m_VertexArray;

		std::shared_ptr<CryDust::Shader> m_BlueShader;
		std::shared_ptr<CryDust::VertexArray> m_SquareVA;

		CryDust::OrthographicCamera m_Camera;
		glm::vec3 m_CameraPosition;
		float m_CameraMoveSpeed = 5.0f;

		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 180.0f;
};

class Sandbox : public CryDust::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());	//推送到层栈
		//PushOverlay(new CryDust::ImGuiLayer());
	}

	~Sandbox()
	{
	}
};


CryDust::Application* CryDust::CreateApplication()
{
	return new Sandbox();
}
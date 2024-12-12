
#include "ExampleLayer.h"

#include "imgui/imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


	ExampleLayer::ExampleLayer()
		: Layer("ExampleLayer"), m_CameraController(1280.0f / 720.0f)
	{
		//构造初始数据
		m_VertexArray = CryDust::VertexArray::Create();

		float vertices[3 * 7] = {
					-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
					 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
					 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		///设置顶点缓冲
		CryDust::Ref<CryDust::VertexBuffer> vertexBuffer = CryDust::VertexBuffer::Create(vertices, sizeof(vertices));


		CryDust::BufferLayout layout = {
			{ CryDust::ShaderDataType::Float3, "a_Position" },
			{ CryDust::ShaderDataType::Float4, "a_Color" }
		};


		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);



		//设置索引数组
		uint32_t indices[3] = { 0, 1, 2 };
		CryDust::Ref<CryDust::IndexBuffer> indexBuffer = CryDust::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

		m_VertexArray->SetIndexBuffer(indexBuffer);

		///新增正方形顶点数组
		m_SquareVA = CryDust::VertexArray::Create();
		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f,0.0f,0.0f,
			 0.5f, -0.5f, 0.0f,1.0f,0.0f,
			 0.5f,  0.5f, 0.0f,1.0f,1.0f,
			-0.5f,  0.5f, 0.0f,0.0f,1.0f
		};

		///正方形顶点缓冲
		CryDust::Ref<CryDust::VertexBuffer> squareVB = CryDust::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ CryDust::ShaderDataType::Float3, "a_Position" },
			{ CryDust::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);


		///正方形索引换成
		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		CryDust::Ref<CryDust::IndexBuffer> squareIB = CryDust::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
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
		m_Shader = CryDust::Shader::Create("VertexPosColor", vertexSrc, fragmentSrc);


		///正方形Shader
		std::string flatColorShaderVertexSrc = R"(
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

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader = CryDust::Shader::Create("FlatColor", flatColorShaderVertexSrc, flatColorShaderFragmentSrc);


		//文件读取shader
		auto textureShader = m_ShaderLibrary.Load("assets/shaders/Texture.glsl");

		m_Texture = CryDust::Texture2D::Create("assets/textures/Checkerboard.png");
		m_ChernoLogoTexture = CryDust::Texture2D::Create("assets/textures/ChernoLogo.png");

		textureShader->Bind();
		textureShader->SetInt("u_Texture", 0);

	}

	void ExampleLayer::OnAttach()
	{
	}
	void ExampleLayer::OnDetach()
	{
	}

	void ExampleLayer::OnImGuiRender() 
	{

		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void ExampleLayer::OnUpdate(CryDust::Timestep ts) 
	{
		//相机更新
		m_CameraController.OnUpdate(ts);

		//渲染指令
		CryDust::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		CryDust::RenderCommand::Clear();



		//局内渲染
		CryDust::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		//绑定
		m_FlatColorShader->Bind();
		m_FlatColorShader->SetFloat3("u_Color", m_SquareColor);


		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				CryDust::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}
		auto textureShader = m_ShaderLibrary.Get("Texture");

		m_Texture->Bind();
		CryDust::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		m_ChernoLogoTexture->Bind();
		CryDust::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		// CryDust::Renderer::Submit(m_Shader, m_VertexArray);

		CryDust::Renderer::EndScene();
	}



	void ExampleLayer:: OnEvent(CryDust::Event& e)
	{
		m_CameraController.OnEvent(e);
	}



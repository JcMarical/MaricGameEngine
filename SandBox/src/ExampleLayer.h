#pragma once
#include "CryDust.h"
class ExampleLayer : public CryDust::Layer
{
public:
	ExampleLayer();
	virtual ~ExampleLayer() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(CryDust::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(CryDust::Event& e) override;
private:
	CryDust::ShaderLibrary m_ShaderLibrary;
	CryDust::Ref<CryDust::Shader> m_Shader;
	CryDust::Ref<CryDust::VertexArray> m_VertexArray;
	CryDust::Ref<CryDust::Shader> m_FlatColorShader;
	CryDust::Ref<CryDust::VertexArray> m_SquareVA;
	CryDust::Ref<CryDust::Texture2D> m_Texture, m_ChernoLogoTexture;
	CryDust::OrthographicCameraController m_CameraController;
	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
};

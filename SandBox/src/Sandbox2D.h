#pragma once
#include "CryDust.h"
class Sandbox2D : public CryDust::Layer
{
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;
	virtual void OnAttach() override;
	virtual void OnDetach() override;
	void OnUpdate(CryDust::Timestep ts) override;
	virtual void OnImGuiRender() override;
	void OnEvent(CryDust::Event& e) override;
private:
	CryDust::OrthographicCameraController m_CameraController;

	// Temp
	CryDust::Ref<CryDust::VertexArray> m_SquareVA;
	CryDust::Ref<CryDust::Shader> m_FlatColorShader;

	CryDust::Ref<CryDust::Texture2D> m_CheckerboardTexture;


	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};
#include "cdpch.h"
#include "Crydust/Scene/SceneCamera.h"
#include <glm/gtc/matrix_transform.hpp>
namespace CryDust {

	void SceneCamera::RecalculateProjection()
	{
		float orthoLeft = -m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthoRight = m_OrthographicSize * m_AspectRatio * 0.5f;
		float orthoBottom = -m_OrthographicSize * 0.5f;
		float orthoTop = m_OrthographicSize * 0.5f;
		m_Projection = glm::ortho(orthoLeft, orthoRight,
			orthoBottom, orthoTop, m_OrthographicNear, m_OrthographicFar);
	}

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}



	/// <summary>
	/// 目前还是正交相机的实现
	/// </summary>
	/// <param name="verticalFOV"></param>
	/// <param name="nearClip"></param>
	/// <param name="farClip"></param>
	void SceneCamera::SetPerspective(float verticalFOV, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspectiveFOV = verticalFOV;
		m_PerspectiveNear = nearClip;
		m_PerspectiveFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthographicSize = size;
		m_OrthographicNear = nearClip;
		m_OrthographicFar = farClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		CORE_DEBUG_ASSERT(width > 0 && height > 0);
		m_AspectRatio = (float)width / (float)height;
		RecalculateProjection();
	}


}

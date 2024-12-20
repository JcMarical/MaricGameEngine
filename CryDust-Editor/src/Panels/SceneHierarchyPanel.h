#pragma once
#include "CryDust/Core/Base.h"

#include "CryDust/Scene/Scene.h"
#include "CryDust/Scene/Entity.h"
namespace CryDust {
	class SceneHierarchyPanel
	{
	public:
		SceneHierarchyPanel() = default;
		SceneHierarchyPanel(const Ref<Scene>& scene);
		void SetContext(const Ref<Scene>& scene);
		void OnImGuiRender();
		void DrawComponents(Entity entity);
	private:
		void DrawEntityNode(Entity entity);
	private:
		Ref<Scene> m_Context;
		Entity m_SelectionContext;
	};
}

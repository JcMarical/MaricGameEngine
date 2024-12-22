#pragma once
#include "entt.hpp"
#include "CryDust/Core/Timestep.h"

namespace CryDust {
	class Entity;
	class Scene
	{
	public:

		Scene();
		~Scene();
		Entity CreateEntity(const std::string& name = std::string());
		void DestroyEntity(Entity entity);
		void OnUpdate(Timestep ts);
		void OnViewportResize(uint32_t width, uint32_t height);
		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
	
	
	private:
		entt::registry m_Registry;//注册表--存储实体数据
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;//viewport信息

		friend class Entity; //友元，Entity可以调用scene的所有东西
		friend class SceneSerializer;
		friend class SceneHierarchyPanel; //设置友元，比如让层次显示能调用registry注册表
	};

}


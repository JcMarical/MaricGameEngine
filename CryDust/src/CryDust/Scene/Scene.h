#pragma once

#include "CryDust/Core/Timestep.h"
#include "CryDust/Core/UUID.h"


#include "CryDust/Renderer/EditorCamera.h"
#include "entt.hpp"


// Box2D
class b2World;


namespace CryDust {
	class Entity;
	class Scene
	{
	public:

		Scene();
		~Scene();

		static Ref<Scene> Copy(Ref<Scene> other);

		Entity CreateEntity(const std::string& name = std::string());
		Entity CreateEntityWithUUID(UUID uuid, const std::string& name = std::string());
		void DestroyEntity(Entity entity);

		void OnRuntimeStart();
		void OnRuntimeStop();


		void OnUpdateRuntime(Timestep ts);
		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		void DuplicateEntity(Entity entity);

		Entity GetPrimaryCameraEntity();
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		
	
	private:
		entt::registry m_Registry;//注册表--存储实体数据
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;//viewport信息

		b2World* m_PhysicsWorld = nullptr;


		friend class Entity; //友元，Entity可以调用scene的所有东西
		friend class SceneSerializer;
		friend class SceneHierarchyPanel; //设置友元，比如让层次显示能调用registry注册表
	};

}


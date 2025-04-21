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

		void OnSimulationStart();
		void OnSimulationStop();

		void OnUpdateRuntime(Timestep ts);

		void OnUpdateSimulation(Timestep ts, EditorCamera& camera);

		void OnUpdateEditor(Timestep ts, EditorCamera& camera);
		void OnViewportResize(uint32_t width, uint32_t height);

		void DuplicateEntity(Entity entity);

		Entity FindEntityByName(std::string_view name);
		Entity GetEntityByUUID(UUID uuid);

		Entity GetPrimaryCameraEntity();

		bool IsRunning() const { return m_IsRunning; }

		//可变参数
		template<typename... Components>
		auto GetAllEntitiesWith()
		{
			return m_Registry.view<Components...>();
		}
	private:
		template<typename T>
		void OnComponentAdded(Entity entity, T& component);
		
		void OnPhysics2DStart();
		void OnPhysics2DStop();

		void RenderScene(EditorCamera& camera);

	
	private:
		entt::registry m_Registry;//注册表--存储实体数据
		uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;//viewport信息

		bool m_IsRunning = false;

		b2World* m_PhysicsWorld = nullptr;

		std::unordered_map<UUID, entt::entity> m_EntityMap;

		friend class Entity; //友元，Entity可以调用scene的所有东西
		friend class SceneSerializer;
		friend class SceneHierarchyPanel; //设置友元，比如让层次显示能调用registry注册表
	};

}


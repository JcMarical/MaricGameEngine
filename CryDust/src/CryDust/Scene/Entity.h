#pragma once

#include "CryDust/Core/UUID.h"
#include "Scene.h"
#include "Components.h"

#include <entt.hpp>
namespace CryDust {
	class Entity
	{
	public:
		Entity() = default;
		Entity(entt::entity handle, Scene* scene);
		Entity(const Entity& other) = default;

		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			CORE_DEBUG_ASSERT(!HasComponent<T>(), "Entity already has component!");
			T& component = m_Scene->m_Registry.emplace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T, typename... Args>
		T& AddOrReplaceComponent(Args&&... args)
		{
			T& component = m_Scene->m_Registry.emplace_or_replace<T>(m_EntityHandle, std::forward<Args>(args)...);
			m_Scene->OnComponentAdded<T>(*this, component);
			return component;
		}

		template<typename T>
		T& GetComponent()
		{
			CORE_DEBUG_ASSERT(IsValid(), "Entity is not valid!");
			CORE_DEBUG_ASSERT(HasComponent<T>(), "Entity does not have component!");

			return m_Scene->m_Registry.get<T>(m_EntityHandle);
		}
		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_EntityHandle);
		}
		template<typename T>
		void RemoveComponent()
		{
			CORE_DEBUG_ASSERT(HasComponent<T>(), "Entity does not have component!");
			m_Scene->m_Registry.remove<T>(m_EntityHandle);
		}
		operator bool() const { return m_EntityHandle != entt::null; }

		// 句柄是否在所属 Scene 的注册表中真实有效。
		// 鼠标拾取/悬停时像素读回的 EntityID 可能是垃圾值（着色器缓存过期、
		// 场景切换后的陈旧句柄等），用 operator bool 无法甄别，必须先调用本函数
		// 再使用 GetComponent，否则注册表越界访问会直接崩溃。
		bool IsValid() const { return m_Scene && m_Scene->m_Registry.valid(m_EntityHandle); }
	
		operator entt::entity() const { return m_EntityHandle; }
		operator uint32_t() const { return (uint32_t)m_EntityHandle; }

		UUID GetUUID() { return GetComponent<IDComponent>().ID; }
		const std::string& GetName() { return GetComponent<TagComponent>().Tag; }


		bool operator==(const Entity& other) const
		{
			return m_EntityHandle == other.m_EntityHandle && m_Scene == other.m_Scene;

		}


		bool operator!=(const Entity& other) const
		{
			return !(*this == other);
		}
	
	
	private:
		entt::entity m_EntityHandle{ entt::null };
		Scene* m_Scene = nullptr;
	};
}

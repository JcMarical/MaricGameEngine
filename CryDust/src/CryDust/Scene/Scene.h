#pragma once
#include "entt.hpp"
#include "CryDust/Core/Timestep.h"
namespace CryDust {
	class Scene
	{
	public:
		Scene();
		~Scene();
		entt::entity CreateEntity(); //创建实体
		// TEMP
		entt::registry& Reg() { return m_Registry; } //注册实体
		void OnUpdate(Timestep ts);
	private:
		entt::registry m_Registry;//注册
	};
}


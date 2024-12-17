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
		void OnUpdate(Timestep ts);
	private:



		entt::registry m_Registry;//注册表--存储实体数据

		friend class Entity; //友元，Entity可以调用scene的所有东西
	};

}


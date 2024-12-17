#include "cdpch.h"
#include "Entity.h"
namespace CryDust {
	Entity::Entity(entt::entity handle, Scene* scene)
		: m_EntityHandle(handle), m_Scene(scene)
	{

	}
}
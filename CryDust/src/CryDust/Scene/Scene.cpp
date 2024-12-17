#include "cdpch.h"
#include "Scene.h"
#include "Components.h"
#include "CryDust/Renderer/Renderer2D.h"
#include <glm/glm.hpp>
namespace CryDust {
	static void DoMath(const glm::mat4& transform)
	{
	}

	//变换构造
	static void OnTransformConstruct(entt::registry& registry, entt::entity entity)
	{
	}



	Scene::Scene()
	{
#if ENTT_EXAMPLE_CODE
		entt::entity entity = m_Registry.create();
		m_Registry.emplace<TransformComponent>(entity, glm::mat4(1.0f));	//实体添加组件，并提供默认参数
		m_Registry.on_construct<TransformComponent>().connect<&OnTransformConstruct>(); //添加变换构造方法，构造后调用

		if (m_Registry.has<TransformComponent>(entity))  //判断entity是否存在某个组件
			TransformComponent& transform = m_Registry.get<TransformComponent>(entity); // 拿到entity上的物体



		//运行时视图
		auto view = m_Registry.view<TransformComponent>();
		for (auto entity : view)
		{
			TransformComponent& transform = view.get<TransformComponent>(entity);		//get拿到运行时的的组件，一般改变也是在这里处理
		}

		//group:对多个组件进行分组
		auto group = m_Registry.group<TransformComponent>(entt::get<MeshComponent>);
		for (auto entity : group)
		{
			auto& [transform, mesh] = group.get<TransformComponent, MeshComponent>(entity);	//使用可变参数，可以拿到多个返回值，C++17的功能

			Renderer::Submit(mesh, transform);//拿到组件后，去提交
		}
#endif
	}
	Scene::~Scene()
	{
	}
	entt::entity Scene::CreateEntity()
	{
		return m_Registry.create();
	}

	//拿到组件
	void Scene::OnUpdate(Timestep ts)
	{
		//注册组件
		auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
		for (auto entity : group)
		{
			//拿到对应的组件
			
			auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);	
			Renderer2D::DrawQuad(transform, sprite.Color);
		}
	}
}
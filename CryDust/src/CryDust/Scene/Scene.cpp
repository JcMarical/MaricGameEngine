#include "cdpch.h"
#include "Scene.h"

#include "Components.h"
#include "CryDust/Renderer/Renderer2D.h"
#include <glm/glm.hpp>

#include "Entity.h"
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

		if (m_Registry.all_of<TransformComponent>(entity))  //判断entity是否存在某个组件
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

	/// <summary>
	/// 构造实体
	/// </summary>
	/// <param name="name"></param>
	/// <returns></returns>
	Entity Scene::CreateEntity(const std::string& name)
	{

		Entity entity = { m_Registry.create(), this };
		entity.AddComponent<TransformComponent>();
		auto& tag = entity.AddComponent<TagComponent>(); //创建tag组件
		tag.Tag = name.empty() ? "Entity" : name;	//设置tag组件为名字
		return entity;
	}

	//拿到组件
	void Scene::OnUpdate(Timestep ts) 
	{
		// 相机组件与其对应的transform组件
		Camera* mainCamera = nullptr;
		glm::mat4* cameraTransform = nullptr;
		{	
			auto view = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : view)
			{
				auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);
				//设置主相机
				if (camera.Primary)
				{
					mainCamera = &camera.Camera;
					cameraTransform = &transform.Transform;
					break;
				}
			}
		}
		//如果主相机存在，再渲染
		if (mainCamera)
		{
			//执行对应的渲染器逻辑
			Renderer2D::BeginScene(mainCamera->GetProjection(), *cameraTransform);
			auto group = m_Registry.group<TransformComponent>(entt::get<SpriteRendererComponent>);
			for (auto entity : group)
			{
				//拿到数据，执行逻辑
				auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);
				Renderer2D::DrawQuad(transform, sprite.Color);
			}
			Renderer2D::EndScene();
		}

	}

	/// <summary>
	/// 窗口設置
	/// </summary>
	/// <param name="width"></param>
	/// <param name="height"></param>
	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		
		m_ViewportWidth = width;
		m_ViewportHeight = height;
		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.FixedAspectRatio)
				cameraComponent.Camera.SetViewportSize(width, height);
		}
	}
}
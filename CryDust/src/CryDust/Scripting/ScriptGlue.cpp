#include "cdpch.h"
#include "ScriptGlue.h"
#include "ScriptEngine.h"

#include "CryDust/Core/UUID.h"
#include "CryDust/Core/KeyCodes.h"
#include "CryDust/Core/Input.h"

#include "CryDust/Scene/Scene.h"
#include "CryDust/Scene/Entity.h"

#include "mono/metadata/object.h"

#include "mono/metadata/reflection.h"

#include "box2d/b2_body.h"


namespace CryDust {

	
	static std::unordered_map<MonoType*, std::function<bool(Entity)>> s_EntityHasComponentFuncs;

#define CD_ADD_INTERNAL_CALL(Name) mono_add_internal_call("CryDust.InternalCalls::" #Name, Name)

	static void NativeLog(MonoString* string, int parameter)
	{
		char* cStr = mono_string_to_utf8(string);
		std::string str(cStr);
		mono_free(cStr);
		std::cout << str << ", " << parameter << std::endl;
	}

	static void NativeLog_Vector(glm::vec3* parameter, glm::vec3* outResult)
	{
		CORE_DEBUG_WARN("Value: {0},{1},{2}", parameter->r,parameter->g,parameter->b);
		*outResult = glm::normalize(*parameter);
	}

	static float NativeLog_VectorDot(glm::vec3* parameter)
	{
		CORE_DEBUG_WARN("Value: {0},{1},{2}", parameter->r, parameter->g, parameter->b);
		return glm::dot(*parameter, *parameter);
	}




	//-------------------------------
	//Component组件相关
	//-------------------------------


	static bool Entity_HasComponent(UUID entityID, MonoReflectionType* componentType)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CORE_DEBUG_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CORE_DEBUG_ASSERT(entity);

		MonoType* managedType = mono_reflection_type_get_type(componentType);
		CORE_DEBUG_ASSERT(s_EntityHasComponentFuncs.find(managedType) != s_EntityHasComponentFuncs.end());
		return s_EntityHasComponentFuncs.at(managedType)(entity);
	}

	static void TransformComponent_GetTranslation(UUID entityID, glm::vec3* outTranslation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CORE_DEBUG_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CORE_DEBUG_ASSERT(entity);

		*outTranslation = entity.GetComponent<TransformComponent>().Translation;
	}

	static void TransformComponent_SetTranslation(UUID entityID, glm::vec3* translation)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CORE_DEBUG_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CORE_DEBUG_ASSERT(entity);

		entity.GetComponent<TransformComponent>().Translation = *translation;
	}

	static void Rigidbody2DComponent_ApplyLinearImpulse(UUID entityID, glm::vec2* impulse, glm::vec2* point, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CORE_DEBUG_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CORE_DEBUG_ASSERT(entity);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulse(b2Vec2(impulse->x, impulse->y), b2Vec2(point->x, point->y), wake);
	}

	static void Rigidbody2DComponent_ApplyLinearImpulseToCenter(UUID entityID, glm::vec2* impulse, bool wake)
	{
		Scene* scene = ScriptEngine::GetSceneContext();
		CORE_DEBUG_ASSERT(scene);
		Entity entity = scene->GetEntityByUUID(entityID);
		CORE_DEBUG_ASSERT(entity);

		auto& rb2d = entity.GetComponent<Rigidbody2DComponent>();
		b2Body* body = (b2Body*)rb2d.RuntimeBody;
		body->ApplyLinearImpulseToCenter(b2Vec2(impulse->x, impulse->y), wake);
	}


	static bool Input_IsKeyDown(KeyCode keycode)
	{
		return Input::IsKeyPressed(keycode);
	}


	/// <summary>
	/// 注册函数
	/// </summary>
	/// <typeparam name="...Component"></typeparam>
	template<typename... Component>
	static void RegisterComponent()
	{
		([]()
			{
				std::string_view typeName = typeid(Component).name();
				size_t pos = typeName.find_last_of(':');
				std::string_view structName = typeName.substr(pos + 1);
				std::string managedTypename = fmt::format("CryDust.{}", structName);

				MonoType* managedType = mono_reflection_type_from_name(managedTypename.data(), ScriptEngine::GetCoreAssemblyImage());
				if (!managedType)
				{
					CORE_DEBUG_ERROR("Could not find component type {}", managedTypename);
					return;
				}
				s_EntityHasComponentFuncs[managedType] = [](Entity entity) { return entity.HasComponent<Component>(); };
			}(), ...);
	}

	template<typename... Component>
	static void RegisterComponent(ComponentGroup<Component...>)
	{
		RegisterComponent<Component...>();
	}

	void ScriptGlue::RegisterComponents()
	{
		s_EntityHasComponentFuncs.clear();
		RegisterComponent(AllComponents{});
	}

	void ScriptGlue::RegisterFunctions()
	{
		CD_ADD_INTERNAL_CALL(NativeLog);
		CD_ADD_INTERNAL_CALL(NativeLog_Vector);
		CD_ADD_INTERNAL_CALL(NativeLog_VectorDot);

		CD_ADD_INTERNAL_CALL(Entity_HasComponent);
		CD_ADD_INTERNAL_CALL(TransformComponent_GetTranslation);
		CD_ADD_INTERNAL_CALL(TransformComponent_SetTranslation);

		CD_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulse);
		CD_ADD_INTERNAL_CALL(Rigidbody2DComponent_ApplyLinearImpulseToCenter);

		CD_ADD_INTERNAL_CALL(Input_IsKeyDown);
	}
}

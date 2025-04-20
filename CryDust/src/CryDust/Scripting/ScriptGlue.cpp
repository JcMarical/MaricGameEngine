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

	void ScriptGlue::RegisterFunctions()
	{
		CD_ADD_INTERNAL_CALL(NativeLog);
		CD_ADD_INTERNAL_CALL(NativeLog_Vector);
		CD_ADD_INTERNAL_CALL(NativeLog_VectorDot);
	}

}

#include "cdpch.h"
#include "ScriptEngine.h"

#include "mono/jit/jit.h"
#include "mono/metadata/assembly.h"
#include "mono/metadata/object.h"

namespace CryDust {

	//集中管理Mono域和程序集指针，确保全局唯一访问。
	struct ScriptEngineData
	{
		MonoDomain* RootDomain = nullptr;
		MonoDomain* AppDomain = nullptr;	

		
		MonoAssembly* CoreAssembly = nullptr;
	};

	//单例
	static ScriptEngineData* s_Data = nullptr;

	void ScriptEngine::Init()
	{
		s_Data = new ScriptEngineData();

		InitMono();
	}

	void ScriptEngine::Shutdown()
	{
		ShutdownMono();
		delete s_Data;
	}

	char* ReadBytes(const std::string& filepath, uint32_t* outSize)
	{
		std::ifstream stream(filepath, std::ios::binary | std::ios::ate);//确保按原始二进制数据读取文件。快速定位到文件末尾，常用于获取文件大小。

		if (!stream)
		{
			return nullptr;
		}
		
		std::streampos end = stream.tellg();	//返回当前读取位置
		stream.seekg(0, std::ios::beg);			//读取位置重置到开头
		uint32_t size = end - stream.tellg();	//计算文件大小

		if (size == 0)
		{
			// File is empty
			return nullptr;
		}

		//字符流读取文件
		char* buffer = new char[size];
		stream.read((char*)buffer, size);
		stream.close();

		*outSize = size;
		return buffer;
	}


	/// <summary>
	/// 读取为二进制数据，确保非托管加载
	/// </summary>
	MonoAssembly* LoadCSharpAssembly(const std::string& assemblyPath)
	{
		uint32_t fileSize = 0;
		char* fileData = ReadBytes(assemblyPath, &fileSize);	//先读文件，拿到文件大小

		MonoImageOpenStatus status;
		MonoImage* image = mono_image_open_from_data_full(fileData, fileSize, 1, &status, 0);	//既在

		if (status != MONO_IMAGE_OK)
		{
			const char* errorMessage = mono_image_strerror(status);
			// Log some error message using the errorMessage data
			return nullptr;
		}

		MonoAssembly* assembly = mono_assembly_load_from_full(image, assemblyPath.c_str(), &status, 0);
		mono_image_close(image);

		// Don't forget to free the file data
		delete[] fileData;

		return assembly;
	}


	/// <summary>
	/// 遍历程序集的所有类型并打印命名空间和类名，用于验证加载结果
	/// </summary>
	void PrintAssemblyTypes(MonoAssembly* assembly)
	{
		MonoImage* image = mono_assembly_get_image(assembly);
		const MonoTableInfo* typeDefinitionsTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
		int32_t numTypes = mono_table_info_get_rows(typeDefinitionsTable);

		for (int32_t i = 0; i < numTypes; i++)
		{
			uint32_t cols[MONO_TYPEDEF_SIZE];
			mono_metadata_decode_row(typeDefinitionsTable, i, cols, MONO_TYPEDEF_SIZE);

			const char* nameSpace = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);
			const char* name = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);

			CORE_DEBUG_TRACE("{}.{}", nameSpace, name);
		}
	}


	void ScriptEngine::InitMono()
	{
		mono_set_assemblies_path("mono/lib");	//设置mono库路径

		MonoDomain* rootDomain = mono_jit_init("CryDustJITRuntime");
		CORE_DEBUG_ASSERT(rootDomain);

		// Store the root domain pointer
		s_Data->RootDomain = rootDomain;//设置根域：作为Mono运行时的入口点

		// Create an App Domain
		s_Data->AppDomain = mono_domain_create_appdomain("CryDustScriptRuntime", nullptr);	//创建应用域（AppDomain）,实现脚本的隔离执行环境
		mono_domain_set(s_Data->AppDomain, true);

		// Move this maybe
		s_Data->CoreAssembly = LoadCSharpAssembly("Resources/Scripts/CryDust-ScriptCore.dll");	//加载C#程序集
		PrintAssemblyTypes(s_Data->CoreAssembly);

		//----------反射机制​​------------
		MonoImage* assemblyImage = mono_assembly_get_image(s_Data->CoreAssembly);
		MonoClass* monoClass = mono_class_from_name(assemblyImage, "CryDust", "Main");//从程序集镜像中查找指定类（如CryDust.Main）

		// 1. create an object (and call constructor) 1.创建物体object并调用构造器
		MonoObject* instance = mono_object_new(s_Data->AppDomain, monoClass);	//创建类实例
		mono_runtime_object_init(instance);//调用构造函数

		// 2. call function 调用方法
		MonoMethod* printMessageFunc = mono_class_get_method_from_name(monoClass, "PrintMessage", 0);	//获取方法指针
		mono_runtime_invoke(printMessageFunc, instance, nullptr, nullptr);//调用方法

		// 3. call function with param	//调用参数和方法。
		MonoMethod* printIntFunc = mono_class_get_method_from_name(monoClass, "PrintInt", 1);

		int value = 5;
		void* param = &value;

		mono_runtime_invoke(printIntFunc, instance, &param, nullptr);

		MonoMethod* printIntsFunc = mono_class_get_method_from_name(monoClass, "PrintInts", 2);
		int value2 = 508;
		void* params[2] =
		{
			&value,
			&value2
		};
		mono_runtime_invoke(printIntsFunc, instance, params, nullptr);

		MonoString* monoString = mono_string_new(s_Data->AppDomain, "Hello World from C++!");//处理值类型（如int）和字符串,正确传递给方法。
		MonoMethod* printCustomMessageFunc = mono_class_get_method_from_name(monoClass, "PrintCustomMessage", 1);
		void* stringParam = monoString;
		mono_runtime_invoke(printCustomMessageFunc, instance, &stringParam, nullptr);

		// HZ_CORE_ASSERT(false);
	}

	void ScriptEngine::ShutdownMono()
	{
		// NOTE(Yan): mono is a little confusing to shutdown, so maybe come back to this

		// mono_domain_unload(s_Data->AppDomain);
		s_Data->AppDomain = nullptr;

		// mono_jit_cleanup(s_Data->RootDomain);
		s_Data->RootDomain = nullptr;
	}
}

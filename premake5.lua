include "./vendor/premake/premake_customization/solution_items.lua"


workspace "CryDust"
	buildoptions "/utf-8"
	architecture "x86_64"
	startproject "CryDust-Editor"


	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}
	
	solution_items
	{
		".editorconfig"
	}

	flags
	{
		"MultiProcessorCompile"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


-- 构建一个Include的table，用来附加包含项目
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/CryDust/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/CryDust/vendor/Glad/include"
IncludeDir["ImGui"] = "%{wks.location}/CryDust/vendor/imgui"
IncludeDir["glm"] = "%{wks.location}/CryDust/vendor/glm"
IncludeDir["stb_image"] = "%{wks.location}/CryDust/vendor/stb_image"
IncludeDir["entt"] = "%{wks.location}/CryDust/vendor/entt/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/CryDust/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "%{wks.location}/CryDust/vendor/ImGuizmo"
-- 这个include，相当于把glfw下的premake5.lua内容拷贝到这里


group "Dependencies"
    include "vendor/premake"
    include "CryDust/vendor/GLFW"
    include "CryDust/vendor/Glad"
    include "CryDust/vendor/imgui"
	include "CryDust/vendor/yaml-cpp"
    
group ""

include "CryDust"
include "Sandbox"
include "CryDust-Editor"
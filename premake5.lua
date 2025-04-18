include "./vendor/premake/premake_customization/solution_items.lua"
include "Dependencies.lua"

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




group "Dependencies"
    include "vendor/premake"
	include "CryDust/vendor/Box2D"
    include "CryDust/vendor/GLFW"
    include "CryDust/vendor/Glad"
    include "CryDust/vendor/imgui"
	include "CryDust/vendor/yaml-cpp"
    
group ""

group "Core"
	include "CryDust"
	include "CryDust-ScriptCore"
group ""

group "Tools"
	include "CryDust-Editor"
group ""

group "Misc"
	include "Sandbox"
group ""
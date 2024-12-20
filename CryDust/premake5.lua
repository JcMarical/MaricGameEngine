project "CryDust"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"
	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	pchheader "cdpch.h"
	pchsource "src/cdpch.cpp"
	files
	{
		"src/**.h",
		"src/**.cpp",
		"vendor/stb_image/**.h",
		"vendor/stb_image/**.cpp",
		"vendor/glm/glm/**.hpp",
		"vendor/glm/glm/**.inl",
	}
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GLFW_INCLUDE_NONE"
	}
	includedirs
	{
		"src",
		"vendor/spdlog/include",
		"%{IncludeDir.GLFW}",
		"%{IncludeDir.Glad}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.entt}"
	}
	links
	{
		"GLFW",
		"Glad",
		"ImGui",
		"opengl32.lib"
	}
	filter "system:windows"
        buildoptions "/utf-8"
		systemversion "latest"
		defines
		{
		}
	filter "configurations:Debug"
		defines "CD_DEBUG"
		runtime "Debug"
		symbols "on"
	filter "configurations:Release"
		defines "CD_RELEASE"
		runtime "Release"
		optimize "on"
	filter "configurations:Dist"
		defines "CD_DIST"
		runtime "Release"
		optimize "on"
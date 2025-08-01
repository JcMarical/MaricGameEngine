project "CryDust-Editor"
	kind "ConsoleApp"
	language "C++"
	cppdialect "C++17"
	staticruntime "off"

	targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/bin-int/" .. outputdir .. "/%{prj.name}")
	files
	{
		"src/**.h",
		"src/**.cpp"
	}
	includedirs
	{
		"%{wks.location}/CryDust/vendor/spdlog/include",
		"%{wks.location}/CryDust/src",
		"%{wks.location}/CryDust/vendor",
		"%{IncludeDir.filewatch}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.entt}",

		"%{IncludeDir.ImGuizmo}"
	}

	
	links
	{
		"CryDust"
	}


	filter "system:windows"
		systemversion "latest"


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



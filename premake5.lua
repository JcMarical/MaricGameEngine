workspace "CryDust"
	architecture "x64"
	startproject "SandBox"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}


outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"


-- 构建一个Include的table，用来附加包含项目
IncludeDir = {}
IncludeDir["GLFW"] = "CryDust/vendor/GLFW/include"
IncludeDir["Glad"] = "CryDust/vendor/Glad/include"
IncludeDir["ImGui"] = "CryDust/vendor/imgui"
-- 这个include，相当于把glfw下的premake5.lua内容拷贝到这里
include "CryDust/vendor/GLFW"
include "CryDust/vendor/Glad"
include "CryDust/vendor/imgui"




project "CryDust"
    location "CryDust"
    kind "SharedLib"
    language "C++"
    staticruntime "off"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


    pchheader "cdpch.h"
    pchsource "CryDust/src/cdpch.cpp"

    --文件
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp"
    }
    
    --包含库
    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",        
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}"
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
        cppdialect "C++17"
        systemversion "latest"


        --windows项目预处理宏定义
        defines
        {
            "CD_PLATFORM_WINDOWS";
            "CD_BUILD_DLL";
            "CD_ENABLE_ASSERTS";
            "GLFW_INCLUDE_NONE"
        }


        postbuildcommands
        {
            ("{COPYFILE} %{cfg.buildtarget.relpath} \"../bin/" .. outputdir .. "/SandBox/\"")
        }

    filter "configurations:Debug"
        defines "CD_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "CD_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "CD_DIST"
        runtime "Release"
        optimize "On"

project "SandBox"
        location "SandBox"
        kind "ConsoleApp"
        language "C++"
        staticruntime "off"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("bin-int/" .. outputdir .. "/%{prj.name}")
    
        --文件
        files
        {
            "%{prj.name}/src/**.h",
            "%{prj.name}/src/**.cpp"
        }
        
        --包含库
        includedirs
        {
            "CryDust/vendor/spdlog/include",
            "CryDust/src",
            "CryDust/vendor"
        }
    
        links
        {
            "CryDust"
        }

    
    filter "system:windows"
        buildoptions "/utf-8"
        cppdialect "C++17"
        systemversion "latest"

        --windows项目预处理宏定义
        defines
        {
            "CD_PLATFORM_WINDOWS";
        }
     
    filter "configurations:Debug"
        defines "CD_DEBUG"
        runtime "Debug"
        symbols "On"

    filter "configurations:Release"
        defines "CD_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "CD_DIST"
       runtime "Release"
        optimize "On"
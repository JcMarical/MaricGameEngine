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
IncludeDir["glm"] = "CryDust/vendor/glm"
IncludeDir["stb_image"] = "CryDust/vendor/stb_image"
-- 这个include，相当于把glfw下的premake5.lua内容拷贝到这里


group "Dependencies"

    include "CryDust/vendor/GLFW"
    include "CryDust/vendor/Glad"
    include "CryDust/vendor/imgui"
    
group ""

project "CryDust"
    location "CryDust"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-int/" .. outputdir .. "/%{prj.name}")


    pchheader "cdpch.h"
    pchsource "CryDust/src/cdpch.cpp"



    --文件
    files
    {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/vendor/stb_image/**.h",
		"%{prj.name}/vendor/stb_image/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
		"%{prj.name}/vendor/glm/glm/**.inl",
    }

    defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}
    
    --包含库
    includedirs
    {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",        
        "%{IncludeDir.Glad}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.glm}",
		"%{IncludeDir.stb_image}"
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


        --windows项目预处理宏定义
        defines
        {
            "CD_PLATFORM_WINDOWS";
            "CD_BUILD_DLL";
            --"CD_ENABLE_ASSERTS";
            "GLFW_INCLUDE_NONE"
        }


		-- 编译好后移动Hazel.dll文件到Sandbox文件夹下
		-- postbuildcommands{
		-- 	("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
		-- }

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

project "SandBox"
        location "SandBox"
        kind "ConsoleApp"
        language "C++"
        cppdialect "C++17"
        staticruntime "on"

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
            "CryDust/vendor",
            "%{IncludeDir.glm}"
        }
    
        links
        {
            "CryDust",
            "GLFW",
		    "opengl32.lib"
        }

    
    filter "system:windows"
        buildoptions "/utf-8"
        systemversion "latest"

        --windows项目预处理宏定义
        defines
        {
            "CD_PLATFORM_WINDOWS";
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
-- CryDust Dependencies
VULKAN_SDK = os.getenv("VULKAN_SDK")
IncludeDir = {}

IncludeDir["stb_image"] = "%{wks.location}/CryDust/vendor/stb_image"
IncludeDir["GLFW"] = "%{wks.location}/CryDust/vendor/GLFW/include"
IncludeDir["Glad"] = "%{wks.location}/CryDust/vendor/Glad/include"
IncludeDir["yaml_cpp"] = "%{wks.location}/CryDust/vendor/yaml-cpp/include"
IncludeDir["Box2D"] = "%{wks.location}/CryDust/vendor/Box2D/include"
IncludeDir["ImGui"] = "%{wks.location}/CryDust/vendor/ImGui"
IncludeDir["ImGuizmo"] = "%{wks.location}/CryDust/vendor/ImGuizmo"
IncludeDir["glm"] = "%{wks.location}/CryDust/vendor/glm"
IncludeDir["entt"] = "%{wks.location}/CryDust/vendor/entt/include"
IncludeDir["shaderc"] = "%{wks.location}/CryDust/vendor/shaderc/include"
IncludeDir["SPIRV_Cross"] = "%{wks.location}/CryDust/vendor/SPIRV-Cross"
IncludeDir["VulkanSDK"] = "%{VULKAN_SDK}/Include"

LibraryDir = {}

LibraryDir["VulkanSDK"] = "%{VULKAN_SDK}/Lib"
LibraryDir["VulkanSDK_Debug"] = "%{wks.location}/CryDust/vendor/VulkanSDK/Lib"
LibraryDir["VulkanSDK_DebugDLL"] = "%{wks.location}/CryDust/vendor/VulkanSDK/Bin"

Library = {}

Library["Vulkan"] = "%{LibraryDir.VulkanSDK}/vulkan-1.lib"
Library["VulkanUtils"] = "%{LibraryDir.VulkanSDK}/VkLayer_utils.lib"

Library["ShaderC_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/shaderc_sharedd.lib"
Library["SPIRV_Cross_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-cored.lib"
Library["SPIRV_Cross_GLSL_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/spirv-cross-glsld.lib"
Library["SPIRV_Tools_Debug"] = "%{LibraryDir.VulkanSDK_Debug}/SPIRV-Toolsd.lib"

Library["ShaderC_Release"] = "%{LibraryDir.VulkanSDK}/shaderc_shared.lib"
Library["SPIRV_Cross_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-core.lib"
Library["SPIRV_Cross_GLSL_Release"] = "%{LibraryDir.VulkanSDK}/spirv-cross-glsl.lib"
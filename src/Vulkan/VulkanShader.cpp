#include "Vulkan/VulkanShader.hpp"

#include "Macros/Macros.hpp"
#include <cstdint>
#include <fstream>
#include <shaderc/shaderc.h>
#include <vulkan/vulkan.h>
#include "Vulkan/VulkanDevice.hpp"
#include "File/FilePath.hpp"

using namespace Eternal::Graphics;
using namespace Eternal::File;

static const shaderc_shader_kind SHADER_KINDS[] =
{
	shaderc_glsl_vertex_shader,
	shaderc_glsl_geometry_shader,
	shaderc_glsl_fragment_shader,
	shaderc_glsl_compute_shader
};

static const char* SHADER_ENTRY_POINTS[] =
{
	"VS",
	"GS",
	"PS",
	"CS"
};

static shaderc_include_result* IncludeResolver(void* UserData, const char* RequestedSource, int Type, const char* RequestingSource, size_t IncludeDepth)
{
	return nullptr;
}

static void IncludeReleaser(void* UserData, shaderc_include_result* IncludeResult)
{

}

VulkanShader::VulkanShader(_In_ VulkanDevice& Device, _In_ const string& Name, _In_ const string& Src, const ShaderType& Type)
{
	string FullPathSrc = FilePath::Find(Src, FilePath::SHADERS);

	ifstream ShaderFile(FullPathSrc.c_str(), ios::in | ios::binary | ios::ate);

	if (!ShaderFile)
	{
		ETERNAL_ASSERT(false);
	}

	uint32_t FileSize = (uint32_t)ShaderFile.tellg();
	_ShaderCode.resize(FileSize);
	ShaderFile.seekg(0, ios::beg);
	ShaderFile.read(_ShaderCode.data(), FileSize);
	ShaderFile.close();

	//shaderc_compiler_t Compiler = shaderc_compiler_initialize();
	//shaderc_compile_options_t CompilerOptions = shaderc_compile_options_initialize();

	//shaderc_compile_options_set_source_language(CompilerOptions, shaderc_source_language_hlsl);
	//shaderc_compile_options_set_warnings_as_errors(CompilerOptions);
	//shaderc_compile_options_set_target_env(CompilerOptions, shaderc_target_env_vulkan, 0);
	//shaderc_compile_options_set_optimization_level(CompilerOptions, shaderc_optimization_level_zero);
	//shaderc_compile_options_set_include_callbacks(CompilerOptions, IncludeResolver, IncludeReleaser, nullptr);

	//shaderc_compilation_result_t CompilationResult = shaderc_compile_into_spv_assembly(Compiler, _ShaderCode.data(), _ShaderCode.size(), SHADER_KINDS[Type], FullPathSrc.c_str(), SHADER_ENTRY_POINTS[Type], CompilerOptions);

	//shaderc_compile_options_release(CompilerOptions);
	//shaderc_compiler_release(Compiler);

	VkShaderModuleCreateInfo ShaderModuleInfo;
	ShaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	ShaderModuleInfo.pNext = nullptr;
	ShaderModuleInfo.pCode = (const uint32_t*)_ShaderCode.data();
	ShaderModuleInfo.codeSize = _ShaderCode.size();
	ShaderModuleInfo.flags = 0;

	VkResult Result = vkCreateShaderModule(Device.GetDevice(), &ShaderModuleInfo, nullptr, &_ShaderModule);
	ETERNAL_ASSERT(!Result);
}

VkShaderModule_T* VulkanShader::GetVulkanShader()
{
	return _ShaderModule;
}

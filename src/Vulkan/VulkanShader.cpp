#include "Vulkan/VulkanShader.hpp"

#include "Macros/Macros.hpp"
#include <cstdint>
#include <fstream>
#include <shaderc/shaderc.h>
#include <vulkan/vulkan.h>
#include "Vulkan/VulkanDevice.hpp"
#include "File/FilePath.hpp"
#include "File/File.hpp"
#include "File/FileFactory.hpp"

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
	std::vector<char>* ShaderSourceCode = new std::vector<char>();
	string FullPathSrc = FilePath::Find(RequestedSource, FilePath::SHADERS);
	Eternal::File::File* ShaderFile = Eternal::File::CreateFile(FullPathSrc);
	ShaderFile->Open(Eternal::File::File::READ);
	uint64_t ShaderFileSize = ShaderFile->GetFileSize();
	ShaderSourceCode->resize(ShaderFileSize);
	ShaderFile->Read((uint8_t*)ShaderSourceCode->data(), ShaderSourceCode->size());
	ShaderFile->Close();
	//DestroyFile(ShaderFile);

	shaderc_include_result* ShaderIncludeResult = new shaderc_include_result;
	ShaderIncludeResult->source_name			= RequestedSource;
	ShaderIncludeResult->source_name_length		= strlen(RequestedSource);
	ShaderIncludeResult->content				= ShaderSourceCode->data();
	ShaderIncludeResult->content_length			= ShaderSourceCode->size();
	ShaderIncludeResult->user_data				= ShaderSourceCode;
	return ShaderIncludeResult;
}

static void IncludeReleaser(void* UserData, shaderc_include_result* IncludeResult)
{
	delete (std::vector<char>*)IncludeResult->user_data;
	delete IncludeResult;
}

VulkanShader::VulkanShader(_In_ Device& DeviceObj, _In_ const string& Name, _In_ const string& Src, const ShaderType& Type)
{
#ifdef ETERNAL_DEBUG
	_CompileFile(DeviceObj, Src, Type);
#else
	_LoadFile(DeviceObj, Src);
#endif
}

void VulkanShader::_CompileFile(_In_ Device& DeviceObj, _In_ const string& Src, const ShaderType& Type)
{
	std::vector<char> ShaderSourceCode;
	string FullPathSrc = FilePath::Find(Src, FilePath::SHADERS);
	Eternal::File::File* ShaderFile = File::CreateFile(FullPathSrc);
	ShaderFile->Open(Eternal::File::File::READ);
	uint64_t ShaderFileSize = ShaderFile->GetFileSize();
	ShaderSourceCode.resize(ShaderFileSize);
	ShaderFile->Read((uint8_t*)ShaderSourceCode.data(), ShaderSourceCode.size());
	ShaderFile->Close();
	//DestroyFile(ShaderFile);

	shaderc_compiler_t Compiler						= shaderc_compiler_initialize();
	shaderc_compile_options_t CompilerOptions		= shaderc_compile_options_initialize();

	shaderc_compile_options_set_source_language(CompilerOptions, shaderc_source_language_hlsl);
	shaderc_compile_options_set_warnings_as_errors(CompilerOptions);
	shaderc_compile_options_set_target_env(CompilerOptions, shaderc_target_env_vulkan, 0);
	shaderc_compile_options_set_optimization_level(CompilerOptions, shaderc_optimization_level_zero);
	shaderc_compile_options_set_include_callbacks(CompilerOptions, IncludeResolver, IncludeReleaser, nullptr);

	shaderc_compilation_result_t CompilationResult	= shaderc_compile_into_spv(Compiler, ShaderSourceCode.data(), ShaderSourceCode.size(), SHADER_KINDS[Type], FullPathSrc.c_str(), SHADER_ENTRY_POINTS[Type], CompilerOptions);
	size_t ShaderByteCodeLength						= shaderc_result_get_length(CompilationResult);
	const char* ShaderByteCode						= shaderc_result_get_bytes(CompilationResult);

	size_t CompilationErrors						= shaderc_result_get_num_errors(CompilationResult);
	size_t CompilationWarnings						= shaderc_result_get_num_warnings(CompilationResult);
	shaderc_compilation_status CompilationStatus	= shaderc_result_get_compilation_status(CompilationResult);
	const char* CompilationErrorMessages			= shaderc_result_get_error_message(CompilationResult);

	shaderc_compile_options_release(CompilerOptions);
	shaderc_compiler_release(Compiler);

	VkShaderModuleCreateInfo ShaderModuleInfo;
	ShaderModuleInfo.sType		= VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	ShaderModuleInfo.pNext		= nullptr;
	ShaderModuleInfo.flags		= 0;
	ShaderModuleInfo.pCode		= (const uint32_t*)ShaderByteCode;
	ShaderModuleInfo.codeSize	= ShaderByteCodeLength;

	VkResult Result = vkCreateShaderModule(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice(), &ShaderModuleInfo, nullptr, &_ShaderModule);
	ETERNAL_ASSERT(!Result);
}

VkShaderModule_T* VulkanShader::GetVulkanShader()
{
	return _ShaderModule;
}

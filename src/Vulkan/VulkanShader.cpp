#include "Vulkan/VulkanShader.hpp"

#include <fstream>
#include <shaderc/shaderc.h>
#include "NextGenGraphics/Context.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Graphics/ShaderType.hpp"
#include "File/FilePath.hpp"
#include "File/File.hpp"
#include "File/FileFactory.hpp"

using namespace Eternal::Graphics;
using namespace Eternal::File;

static const shaderc_shader_kind SHADER_KINDS[] =
{
	shaderc_glsl_vertex_shader,
	shaderc_glsl_tess_control_shader,
	shaderc_glsl_tess_evaluation_shader,
	shaderc_glsl_geometry_shader,
	shaderc_glsl_fragment_shader,
	shaderc_glsl_compute_shader
};

static const char* SHADER_ENTRY_POINTS[] =
{
	"VS",
	"HS",
	"DS",
	"GS",
	"PS",
	"CS"
};
ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(SHADER_KINDS) == int(ShaderType::SHADER_TYPE_COUNT),		"Shader kinds does not match ShaderType definition.");
ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(SHADER_ENTRY_POINTS) == int(ShaderType::SHADER_TYPE_COUNT),	"Shader entry points does not match ShaderType definition.");

static shaderc_include_result* IncludeResolver(void* UserData, const char* RequestedSource, int Type, const char* RequestingSource, size_t IncludeDepth)
{
	std::vector<char>* ShaderSourceCode = new std::vector<char>();
	string FullPathSrc = FilePath::Find(RequestedSource, FileType::SHADERS);
	Eternal::File::File* ShaderFile = CreateFileHandle(FullPathSrc);
	ShaderFile->Open(Eternal::File::File::READ);
	uint64_t ShaderFileSize = ShaderFile->GetFileSize();
	ShaderSourceCode->resize(ShaderFileSize);
	ShaderFile->Read((uint8_t*)ShaderSourceCode->data(), ShaderSourceCode->size());
	ShaderFile->Close();
	DestroyFileHandle(ShaderFile);

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

VulkanShader::VulkanShader(_In_ Device& DeviceObj, _In_ const string& Name, _In_ const string& Source, _In_ const ShaderType& Type, _In_ const vector<string>& Defines /* = vector<string>() */)
	: Shader(Name)
{
#ifdef ETERNAL_DEBUG
	_CompileFile(DeviceObj, Source, Type, Defines);
#else
	_LoadFile(DeviceObj, Source);
#endif
}

VulkanShader::VulkanShader(_In_ GraphicsContext& Context, const ShaderCreateInformation& CreateInformation)
	: VulkanShader(Context.GetDevice(),
				   CreateInformation.Name,
				   CreateInformation.FileName,
				   CreateInformation.Stage)
{
}

void VulkanShader::_CompileFile(_In_ Device& DeviceObj, _In_ const string& Source, _In_ const ShaderType& Type, _In_ const vector<string>& Defines)
{
	using namespace Vulkan;

	ETERNAL_ASSERT(!(Defines.size() % 2));

	const uint32_t VulkanVersion = static_cast<VulkanDevice&>(DeviceObj).GetVulkanVersion();

	std::vector<char> ShaderSourceCode;
	string FullPathSrc = FilePath::Find(Source, FileType::SHADERS);
	Eternal::File::File* ShaderFile = CreateFileHandle(FullPathSrc);
	ShaderFile->Open(Eternal::File::File::READ);
	uint64_t ShaderFileSize = ShaderFile->GetFileSize();
	ShaderSourceCode.resize(ShaderFileSize);
	ShaderFile->Read((uint8_t*)ShaderSourceCode.data(), ShaderSourceCode.size());
	ShaderFile->Close();
	DestroyFileHandle(ShaderFile);

	shaderc_compiler_t Compiler						= shaderc_compiler_initialize();
	shaderc_compile_options_t CompilerOptions		= shaderc_compile_options_initialize();

	shaderc_compile_options_set_source_language(CompilerOptions, shaderc_source_language_hlsl);
	shaderc_compile_options_set_warnings_as_errors(CompilerOptions);

	shaderc_env_version ShaderEnvironmentVersion = shaderc_env_version_vulkan_1_0;
	shaderc_spirv_version ShaderSpirvVersion = shaderc_spirv_version_1_0;

	switch (VulkanVersion)
	{
	case VK_API_VERSION_1_1:
		ShaderEnvironmentVersion = shaderc_env_version_vulkan_1_1;
		ShaderSpirvVersion = shaderc_spirv_version_1_3;
		break;

	case VK_API_VERSION_1_0:
	default:
		break;
	}

	shaderc_compile_options_set_target_env(CompilerOptions, shaderc_target_env_vulkan, ShaderEnvironmentVersion);
	shaderc_compile_options_set_target_spirv(CompilerOptions, ShaderSpirvVersion);

	shaderc_compile_options_set_optimization_level(CompilerOptions, shaderc_optimization_level_zero);
	shaderc_compile_options_set_generate_debug_info(CompilerOptions);
	shaderc_compile_options_set_include_callbacks(CompilerOptions, IncludeResolver, IncludeReleaser, nullptr);

	for (int DefineIndex = 0, DefineCount = int(Defines.size() / 2); DefineIndex < DefineCount; ++DefineIndex)
	{
		shaderc_compile_options_add_macro_definition(
			CompilerOptions,
			Defines[DefineIndex * 2].c_str(),
			Defines[DefineIndex * 2].size(),
			Defines[DefineIndex * 2 + 1].c_str(),
			Defines[DefineIndex * 2 + 1].size()
		);
	}

	shaderc_compilation_result_t CompilationResult	= shaderc_compile_into_spv(Compiler, ShaderSourceCode.data(), ShaderSourceCode.size(), SHADER_KINDS[int(Type)], FullPathSrc.c_str(), SHADER_ENTRY_POINTS[int(Type)], CompilerOptions);
	size_t ShaderByteCodeLength						= shaderc_result_get_length(CompilationResult);
	const char* ShaderByteCode						= shaderc_result_get_bytes(CompilationResult);

	size_t CompilationErrors						= shaderc_result_get_num_errors(CompilationResult);
	size_t CompilationWarnings						= shaderc_result_get_num_warnings(CompilationResult);
	shaderc_compilation_status CompilationStatus	= shaderc_result_get_compilation_status(CompilationResult);
	const char* CompilationErrorMessages			= shaderc_result_get_error_message(CompilationResult);

	ETERNAL_ASSERT(CompilationStatus == shaderc_compilation_status_success);

	{
		shaderc_compilation_result_t DebugCompilationResult = shaderc_compile_into_spv_assembly(Compiler, ShaderSourceCode.data(), ShaderSourceCode.size(), SHADER_KINDS[int(Type)], FullPathSrc.c_str(), SHADER_ENTRY_POINTS[int(Type)], CompilerOptions);
		size_t DebugShaderByteCodeLength = shaderc_result_get_length(DebugCompilationResult);
		const char* DebugShaderByteCode = shaderc_result_get_bytes(DebugCompilationResult);

		size_t DebugCompilationErrors = shaderc_result_get_num_errors(DebugCompilationResult);
		size_t DebugCompilationWarnings = shaderc_result_get_num_warnings(DebugCompilationResult);
		shaderc_compilation_status DebugCompilationStatus = shaderc_result_get_compilation_status(DebugCompilationResult);
		const char* DebugCompilationErrorMessages = shaderc_result_get_error_message(DebugCompilationResult);

		shaderc_result_release(DebugCompilationResult);
	}
	shaderc_compile_options_release(CompilerOptions);
	shaderc_compiler_release(Compiler);

	vk::ShaderModuleCreateInfo ShaderModuleInfo(
		vk::ShaderModuleCreateFlagBits(),
		ShaderByteCodeLength,
		reinterpret_cast<const uint32_t*>(ShaderByteCode)
	);

	VerifySuccess(static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice().createShaderModule(&ShaderModuleInfo, nullptr, &_ShaderModule));

	shaderc_result_release(CompilationResult);
}

vk::ShaderModule& VulkanShader::GetVulkanShader()
{
	return _ShaderModule;
}

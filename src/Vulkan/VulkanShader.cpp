#if ETERNAL_ENABLE_VULKAN

#include "Vulkan/VulkanShader.hpp"

#include <fstream>
#include <shaderc/shaderc.h>
#include "Vulkan/VulkanGraphicsContext.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Graphics/ShaderType.hpp"
#include "File/FilePath.hpp"
#include "File/File.hpp"
#include "File/FileFactory.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::FileSystem;

		static constexpr shaderc_shader_kind SHADER_KINDS[] =
		{
			shaderc_vertex_shader,
			shaderc_tess_control_shader,
			shaderc_tess_evaluation_shader,
			shaderc_geometry_shader,
			shaderc_fragment_shader,
			shaderc_compute_shader,
			shaderc_mesh_shader,
			shaderc_task_shader,
			shaderc_raygen_shader,
			shaderc_closesthit_shader,
			shaderc_miss_shader,
			shaderc_anyhit_shader
		};

		static const char* SHADER_ENTRY_POINTS[] =
		{
			"ShaderVertex",
			"ShaderHull",
			"ShaderDomain",
			"ShaderGeometry",
			"ShaderPixel",
			"ShaderCompute",
			"ShaderMesh",
			"ShaderAmplification",
			"ShaderRayGeneration",
			"ShaderClosestHit",
			"ShaderMiss",
			"ShaderAnyHit"
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(SHADER_KINDS) == static_cast<uint32_t>(ShaderType::SHADER_TYPE_COUNT),			"Shader kinds does not match ShaderType definition.");
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(SHADER_ENTRY_POINTS) == static_cast<uint32_t>(ShaderType::SHADER_TYPE_COUNT),	"Shader entry points does not match ShaderType definition.");

		namespace VulkanPrivate
		{
			struct VulkanShaderUserData
			{
				VulkanShaderUserData(_Inout_ GraphicsContext& InOutContext)
					: Context(InOutContext)
				{
				}

				GraphicsContext& Context;
			};
		}

		static shaderc_include_result* IncludeResolver(void* InUserData, const char* InRequestedSource, int InType, const char* InRequestingSource, size_t InIncludeDepth)
		{
			VulkanPrivate::VulkanShaderUserData& UserData = *reinterpret_cast<VulkanPrivate::VulkanShaderUserData*>(InUserData);

			FileContent* ShaderSourceCode = new FileContent();
			string FullPathSource = FilePath::Find(InRequestedSource, FileType::FILE_TYPE_SHADERS);
			*ShaderSourceCode = LoadFileToMemory(FullPathSource);

			shaderc_include_result* ShaderIncludeResult = new shaderc_include_result;
			ShaderIncludeResult->source_name			= InRequestedSource;
			ShaderIncludeResult->source_name_length		= strlen(InRequestedSource);
			ShaderIncludeResult->content				= reinterpret_cast<const char*>(ShaderSourceCode->Content);
			ShaderIncludeResult->content_length			= ShaderSourceCode->Size;
			ShaderIncludeResult->user_data				= ShaderSourceCode;
			return ShaderIncludeResult;
		}

		static void IncludeReleaser(void* InUserData, shaderc_include_result* InIncludeResult)
		{
			FileContent* ShaderSourceCode = reinterpret_cast<FileContent*>(InIncludeResult->user_data);
			UnloadFileFromMemory(*ShaderSourceCode);
			delete ShaderSourceCode;
			delete InIncludeResult;
		}

		VulkanShader::VulkanShader(_Inout_ GraphicsContext& InOutContext, const ShaderCreateInformation& InCreateInformation)
			: Shader(
				InOutContext,
				InCreateInformation
			)
		{
#if ETERNAL_DEBUG || 1
			_CompileFile(InOutContext, InCreateInformation.FileName, InCreateInformation.Stage, InCreateInformation.Defines);
#else
			// Not implemented
#endif
		}

		VulkanShader::VulkanShader()
			: Shader()
		{
		}

		void VulkanShader::_CompileFile(_Inout_ GraphicsContext& InOutContext, _In_ const string& InFileName, _In_ const ShaderType& InStage, _In_ const vector<string>& InDefines)
		{
			using namespace Vulkan;

			ETERNAL_ASSERT(!(InDefines.size() % 2));

			Device& InDevice				= InOutContext.GetDevice();
			const uint32_t VulkanVersion	= static_cast<VulkanDevice&>(InDevice).GetVulkanVersion();

			string FullPathSource			= FilePath::Find(InFileName, FileType::FILE_TYPE_SHADERS);
			FileContent ShaderSourceCode	= LoadFileToMemory(FullPathSource);

			string ShaderFileContent		= R"HLSLINCLUDE(
				#include "platform.common.hlsl"
				#include "ShadersReflection/HLSLReflection.hpp"
			)HLSLINCLUDE";
			ShaderFileContent				+= reinterpret_cast<const char*>(ShaderSourceCode.Content);

			shaderc_compiler_t Compiler						= shaderc_compiler_initialize();
			shaderc_compile_options_t CompilerOptions		= shaderc_compile_options_initialize();

			shaderc_compile_options_set_source_language(CompilerOptions, shaderc_source_language_hlsl);
			shaderc_compile_options_set_warnings_as_errors(CompilerOptions);

			shaderc_env_version ShaderEnvironmentVersion	= shaderc_env_version_vulkan_1_0;
			shaderc_spirv_version ShaderSpirvVersion		= shaderc_spirv_version_1_0;

			switch (VulkanVersion)
			{
			case VK_API_VERSION_1_2:
				ShaderEnvironmentVersion	= shaderc_env_version_vulkan_1_2;
				ShaderSpirvVersion			= shaderc_spirv_version_1_3;
				break;

			case VK_API_VERSION_1_1:
				ShaderEnvironmentVersion	= shaderc_env_version_vulkan_1_1;
				ShaderSpirvVersion			= shaderc_spirv_version_1_3;
				break;

			case VK_API_VERSION_1_0:
			default:
				break;
			}

			shaderc_compile_options_set_target_env(CompilerOptions, shaderc_target_env_vulkan, ShaderEnvironmentVersion);
			shaderc_compile_options_set_target_spirv(CompilerOptions, ShaderSpirvVersion);

			shaderc_compile_options_set_optimization_level(CompilerOptions, shaderc_optimization_level_zero);
			shaderc_compile_options_set_generate_debug_info(CompilerOptions);

			VulkanPrivate::VulkanShaderUserData UserData(InOutContext);
			shaderc_compile_options_set_include_callbacks(CompilerOptions, IncludeResolver, IncludeReleaser, &UserData);

			for (int32_t DefineIndex = 0, DefineCount = static_cast<int32_t>(InDefines.size() / 2); DefineIndex < DefineCount; ++DefineIndex)
			{
				shaderc_compile_options_add_macro_definition(
					CompilerOptions,
					InDefines[DefineIndex * 2].c_str(),
					InDefines[DefineIndex * 2].size(),
					InDefines[DefineIndex * 2 + 1].c_str(),
					InDefines[DefineIndex * 2 + 1].size()
				);
			}

			static const std::string MacroPlatformName = "ETERNAL_PLATFORM_VULKAN";
			static const std::string MacroPlatformValue = "1";
			shaderc_compile_options_add_macro_definition(
				CompilerOptions,
				MacroPlatformName.c_str(),
				MacroPlatformName.size(),
				MacroPlatformValue.c_str(),
				MacroPlatformValue.size()
			);

			uint32_t StageOffset = static_cast<uint32_t>(InStage) * (VulkanGraphicsContext::ShaderRegisterSamplersOffset + VulkanGraphicsContext::MaxSamplersCountPerShader);
	
			static const std::string MacroRegisterOffsetShaderResourcesName		= "REGISTER_OFFSET_SHADER_RESOURCES";
			const std::string MacroRegisterOffsetShaderResourcesValue			= std::to_string(StageOffset + VulkanGraphicsContext::ShaderRegisterShaderResourcesOffset);
			shaderc_compile_options_add_macro_definition(
				CompilerOptions,
				MacroRegisterOffsetShaderResourcesName.c_str(),
				MacroRegisterOffsetShaderResourcesName.size(),
				MacroRegisterOffsetShaderResourcesValue.c_str(),
				MacroRegisterOffsetShaderResourcesValue.size()
			);

			static const std::string MacroRegisterOffsetConstantBuffersName		= "REGISTER_OFFSET_CONSTANT_BUFFERS";
			const std::string MacroRegisterOffsetConstantBuffersValue			= std::to_string(StageOffset + VulkanGraphicsContext::ShaderRegisterConstantBuffersOffset);
			shaderc_compile_options_add_macro_definition(
				CompilerOptions,
				MacroRegisterOffsetConstantBuffersName.c_str(),
				MacroRegisterOffsetConstantBuffersName.size(),
				MacroRegisterOffsetConstantBuffersValue.c_str(),
				MacroRegisterOffsetConstantBuffersValue.size()
			);

			static const std::string MacroRegisterOffsetUnorderedAccessesName	= "REGISTER_OFFSET_UNORDERED_ACCESSES";
			const std::string MacroRegisterOffsetUnorderedAccessesValue			= std::to_string(StageOffset + VulkanGraphicsContext::ShaderRegisterUnorderedAccessesOffset);
			shaderc_compile_options_add_macro_definition(
				CompilerOptions,
				MacroRegisterOffsetUnorderedAccessesName.c_str(),
				MacroRegisterOffsetUnorderedAccessesName.size(),
				MacroRegisterOffsetUnorderedAccessesValue.c_str(),
				MacroRegisterOffsetUnorderedAccessesValue.size()
			);

			static const std::string MacroRegisterOffsetSamplersName			= "REGISTER_OFFSET_SAMPLERS";
			const std::string MacroRegisterOffsetSamplersValue					= std::to_string(StageOffset + VulkanGraphicsContext::ShaderRegisterSamplersOffset);
			shaderc_compile_options_add_macro_definition(
				CompilerOptions,
				MacroRegisterOffsetSamplersName.c_str(),
				MacroRegisterOffsetSamplersName.size(),
				MacroRegisterOffsetSamplersValue.c_str(),
				MacroRegisterOffsetSamplersValue.size()
			);

			shaderc_compile_options_set_binding_base(CompilerOptions, shaderc_uniform_kind_texture,					0);
			shaderc_compile_options_set_binding_base(CompilerOptions, shaderc_uniform_kind_buffer,					0);
			shaderc_compile_options_set_binding_base(CompilerOptions, shaderc_uniform_kind_unordered_access_view,	0);
			shaderc_compile_options_set_binding_base(CompilerOptions, shaderc_uniform_kind_sampler,					0);

			shaderc_compilation_result_t PreprocessedCompilationResult = shaderc_compile_into_preprocessed_text(
				Compiler,
				reinterpret_cast<const char*>(ShaderFileContent.c_str()),
				ShaderFileContent.size(),
				SHADER_KINDS[static_cast<int32_t>(InStage)],
				FullPathSource.c_str(),
				SHADER_ENTRY_POINTS[static_cast<int32_t>(InStage)],
				CompilerOptions
			);

			shaderc_compilation_result_t CompilationResult	= shaderc_compile_into_spv(
				Compiler,
				reinterpret_cast<const char*>(ShaderFileContent.c_str()),
				ShaderFileContent.size(),
				SHADER_KINDS[static_cast<int32_t>(InStage)],
				FullPathSource.c_str(),
				SHADER_ENTRY_POINTS[static_cast<int32_t>(InStage)],
				CompilerOptions
			);
			size_t ShaderByteCodeLength						= shaderc_result_get_length(CompilationResult);
			const char* ShaderByteCode						= shaderc_result_get_bytes(CompilationResult);

			size_t CompilationErrors						= shaderc_result_get_num_errors(CompilationResult);
			size_t CompilationWarnings						= shaderc_result_get_num_warnings(CompilationResult);
			shaderc_compilation_status CompilationStatus	= shaderc_result_get_compilation_status(CompilationResult);
			const char* CompilationErrorMessages			= shaderc_result_get_error_message(CompilationResult);
			const char* PreprocessedCompilationText			= shaderc_result_get_bytes(PreprocessedCompilationResult);

			ETERNAL_ASSERT(CompilationStatus == shaderc_compilation_status_success);

			{
				shaderc_compilation_result_t DebugCompilationResult = shaderc_compile_into_spv_assembly(
					Compiler,
					ShaderFileContent.c_str(),
					ShaderFileContent.size(),
					SHADER_KINDS[static_cast<int32_t>(InStage)],
					FullPathSource.c_str(),
					SHADER_ENTRY_POINTS[static_cast<int32_t>(InStage)],
					CompilerOptions
				);
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

			VerifySuccess(static_cast<VulkanDevice&>(InDevice).GetVulkanDevice().createShaderModule(&ShaderModuleInfo, nullptr, &_ShaderModule));

			shaderc_result_release(CompilationResult);
		}

		void VulkanShader::SerializeShader(_Inout_ File* InOutFile)
		{
			ETERNAL_BREAK();
		}

		vk::ShaderModule& VulkanShader::GetVulkanShader()
		{
			return _ShaderModule;
		}

		bool VulkanShader::IsShaderCompiled() const
		{
			return _ShaderModule;
		}
	}
}

#endif

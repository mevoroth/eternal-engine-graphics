#include "Graphics/Shader.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "File/FilePath.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Shader::Shader(_Inout_ GraphicsContext& InOutContext, _In_ const ShaderCreateInformation& InShaderCreateInformation)
			: _ShaderCreateInformation(InShaderCreateInformation)
		{
			using namespace Eternal::FileSystem;

			string ShaderFullPathSource = FilePath::Find(_ShaderCreateInformation.FileName, FileType::FILE_TYPE_SHADERS);
			FilePath::NormalizePath(ShaderFullPathSource);
			InOutContext.GetPipelineDependency().RegisterShaderDependency(this, ShaderFullPathSource);
		}

		const string& Shader::GetName() const
		{
			return _ShaderCreateInformation.Name;
		}

		const string& Shader::GetFileName() const
		{
			return _ShaderCreateInformation.FileName;
		}

		ShaderCreateInformation& Shader::GetShaderCreateInformation()
		{
			return _ShaderCreateInformation;
		}

		const ShaderCreateInformation& Shader::GetShaderCreateInformation() const
		{
			return _ShaderCreateInformation;
		}

		//////////////////////////////////////////////////////////////////////////

		ShaderCreateInformation::ShaderCreateInformation(_In_ ShaderType InStage, _In_ const char* InName, _In_ const char* InFileName, _In_ const vector<string>& InDefines /* = vector<string>() */)
			: Stage(InStage)
			, Name(InName)
			, FileName(InFileName)
			, Defines(InDefines)
		{
			switch (InStage)
			{
			case ShaderType::SHADER_TYPE_MESH:
			case ShaderType::SHADER_TYPE_AMPLIFICATION:
			case ShaderType::SHADER_TYPE_RAYTRACING_RAY_GENERATION:
			case ShaderType::SHADER_TYPE_RAYTRACING_CLOSEST_HIT:
			case ShaderType::SHADER_TYPE_RAYTRACING_MISS:
			case ShaderType::SHADER_TYPE_RAYTRACING_ANY_HIT:
				D3D12CompilerPreference = D3D12ShaderCompilerType::D3D12_SHADER_COMPILER_TYPE_DXC;
				break;
			}
		}
	}
}

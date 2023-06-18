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
	}
}

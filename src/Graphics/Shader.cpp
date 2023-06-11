#include "Graphics/Shader.hpp"
#include "Graphics/GraphicsContext.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Shader::Shader(_Inout_ GraphicsContext& InOutContext, _In_ const string& InName, _In_ const string& InFileName)
			: _Name(InName)
			, _FileName(InFileName)
		{
			InOutContext.GetPipelineDependency().RegisterShaderDependency(this, _FileName);
		}

		const string& Shader::GetName() const
		{
			return _Name;
		}
	}
}

#include "Graphics/Shader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Shader::Shader(_In_ const string& InName)
			: _Name(InName)
		{
		}

		const string& Shader::GetName() const
		{
			return _Name;
		}
	}
}

#include "Graphics/ShaderType.hpp"

namespace Eternal
{
	namespace Graphics
	{
		ShaderTypeFlags operator|(_In_ const ShaderTypeFlags& InLeftShaderTypeFlags, _In_ const ShaderTypeFlags& InRightShaderTypeFlags)
		{
			return static_cast<ShaderTypeFlags>(
				static_cast<uint32_t>(InLeftShaderTypeFlags) | static_cast<uint32_t>(InRightShaderTypeFlags)
			);
		}
		ShaderTypeFlags operator&(_In_ const ShaderTypeFlags& InLeftShaderTypeFlags, _In_ const ShaderTypeFlags& InRightShaderTypeFlags)
		{
			return static_cast<ShaderTypeFlags>(
				static_cast<uint32_t>(InLeftShaderTypeFlags) & static_cast<uint32_t>(InRightShaderTypeFlags)
			);
		}
		ShaderTypeFlags& operator|=(_Inout_ ShaderTypeFlags& InOutLeftShaderTypeFlags, _In_ const ShaderTypeFlags& InRightShaderTypeFlags)
		{
			InOutLeftShaderTypeFlags = InOutLeftShaderTypeFlags | InRightShaderTypeFlags;
			return InOutLeftShaderTypeFlags;
		}
	}
}

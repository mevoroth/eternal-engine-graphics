#pragma once

namespace Eternal
{
	namespace Graphics
	{
		enum class ShaderType
		{
			VS = 0,
			HS,
			DS,
			GS,
			PS,
			CS,
			MS,
			AS,
			SHADER_TYPE_COUNT
		};

		enum class ShaderTypeFlags
		{
			CS = 0x0,
			VS = 0x1,
			HS = 0x2,
			DS = 0x4,
			GS = 0x8,
			PS = 0x10,
			MS = 0x20,
			AS = 0x40,

			VS_PS		= VS | PS,
			MS_PS		= MS | PS,
			MS_AS_PS	= MS | AS | PS,

			SHADER_TYPE_FLAGS_UNDEFINED
		};

		ShaderTypeFlags operator|(_In_ const ShaderTypeFlags& InLeftShaderTypeFlags, _In_ const ShaderTypeFlags& InRightShaderTypeFlags);
		ShaderTypeFlags operator&(_In_ const ShaderTypeFlags& InLeftShaderTypeFlags, _In_ const ShaderTypeFlags& InRightShaderTypeFlags);
	}
}

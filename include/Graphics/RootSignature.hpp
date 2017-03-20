#ifndef _ROOT_SIGNATURE_HPP_
#define _ROOT_SIGNATURE_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		enum RootSignatureAccess
		{
			ROOT_SIGNATURE_NONE		= 0x0,
			ROOT_SIGNATURE_IA		= 0x1,
			ROOT_SIGNATURE_VS		= 0x2,
			ROOT_SIGNATURE_HS		= 0x4,
			ROOT_SIGNATURE_DS		= 0x8,
			ROOT_SIGNATURE_GS		= 0x10,
			ROOT_SIGNATURE_PS		= 0x20,
			ROOT_SIGNATURE_SO		= 0x40
		};

		enum RootSignatureDynamicParameterType
		{
			SAMPLER,
			TEXTURE,
			RW_TEXTURE,
			STRUCTURED_BUFFER,
			RW_STRUCTURED_BUFFER,
			BUFFER,
			RW_BUFFER,
			DYNAMIC_BUFFER,
			RW_DYNAMIC_BUFFER,
			RENDER_TARGET,
		};

		struct RootSignatureDynamicParameter
		{
			RootSignatureDynamicParameterType	Type;
			uint32_t							Count;
		};

		class RootSignature
		{

		};
	}
}

#endif

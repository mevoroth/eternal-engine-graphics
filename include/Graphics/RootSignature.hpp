#ifndef _ROOT_SIGNATURE_HPP_
#define _ROOT_SIGNATURE_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		enum RootSignatureAccess
		{
			NONE	= 0x0,
			IA		= 0x1,
			VS		= 0x2,
			HS		= 0x4,
			DS		= 0x8,
			GS		= 0x10,
			PS		= 0x20,
			SO		= 0x40
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

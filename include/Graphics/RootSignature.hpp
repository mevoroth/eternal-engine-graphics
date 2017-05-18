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
			ROOT_SIGNATURE_PARAMETER_SAMPLER	= 0,
			ROOT_SIGNATURE_PARAMETER_TEXTURE,
			ROOT_SIGNATURE_PARAMETER_RW_TEXTURE,
			ROOT_SIGNATURE_PARAMETER_BUFFER,
			ROOT_SIGNATURE_PARAMETER_RW_BUFFER,
			ROOT_SIGNATURE_PARAMETER_STRUCTURED_BUFFER,
			ROOT_SIGNATURE_PARAMETER_RW_STRUCTURED_BUFFER,
			ROOT_SIGNATURE_PARAMETER_DYNAMIC_BUFFER,
			ROOT_SIGNATURE_PARAMETER_RW_DYNAMIC_BUFFER,
			ROOT_SIGNATURE_PARAMETER_RENDER_TARGET,
			ROOT_SIGNATURE_PARAMETER_DEPTH_STENCIL,
			ROOT_SIGNATURE_PARAMETER_COUNT
		};

		struct RootSignatureDynamicParameter
		{
			RootSignatureDynamicParameterType	Type;
			uint32_t							Count;
		};

		class RootSignature
		{
		public:
			virtual ~RootSignature() {}
		};
	}
}

#endif

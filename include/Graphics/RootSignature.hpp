#ifndef _ROOT_SIGNATURE_HPP_
#define _ROOT_SIGNATURE_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		enum RootSignatureAccess
		{
			ROOT_SIGNATURE_ACCESS_NONE		= 0x0,
			ROOT_SIGNATURE_ACCESS_IA		= 0x1,
			ROOT_SIGNATURE_ACCESS_VS		= 0x2,
			ROOT_SIGNATURE_ACCESS_HS		= 0x4,
			ROOT_SIGNATURE_ACCESS_DS		= 0x8,
			ROOT_SIGNATURE_ACCESS_GS		= 0x10,
			ROOT_SIGNATURE_ACCESS_PS		= 0x20,
			ROOT_SIGNATURE_ACCESS_SO		= 0x40,

			ROOT_SIGNATURE_ACCESS_IA_VS_PS	= (ROOT_SIGNATURE_ACCESS_IA | ROOT_SIGNATURE_ACCESS_VS | ROOT_SIGNATURE_ACCESS_PS)
		};

		enum RootSignatureParameterType
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

		struct RootSignatureParameter
		{
			RootSignatureParameterType	Parameter;
			RootSignatureAccess			Access;
			uint32_t					Register;
		};

		struct RootSignatureDynamicParameter
		{
			RootSignatureParameterType	Type;
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

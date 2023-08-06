#include "Graphics/Types/HLSLTypes.hpp"

#include "Graphics/RootSignature.hpp"

namespace Eternal
{
	namespace Graphics
	{
		static constexpr HLSLRegisterType HLSL_REGISTER_TYPES[] =
		{
			HLSLRegisterType::HLSL_REGISTER_TYPE_SAMPLER,
			HLSLRegisterType::HLSL_REGISTER_TYPE_SRV,
			HLSLRegisterType::HLSL_REGISTER_TYPE_UAV,
			HLSLRegisterType::HLSL_REGISTER_TYPE_SRV,
			HLSLRegisterType::HLSL_REGISTER_TYPE_UAV,
			HLSLRegisterType::HLSL_REGISTER_TYPE_CBV,
			HLSLRegisterType::HLSL_REGISTER_TYPE_SRV,
			HLSLRegisterType::HLSL_REGISTER_TYPE_UAV,
			HLSLRegisterType::HLSL_REGISTER_TYPE_SRV,
			HLSLRegisterType::HLSL_REGISTER_TYPE_INVALID
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(HLSL_REGISTER_TYPES) == static_cast<int32_t>(RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_COUNT), "Mismatch between abstraction and d3d12 register types");

		const HLSLRegisterType& ConvertRootSignatureParameterTypeToHLSLRegisterType(_In_ const RootSignatureParameterType& InRootSignatureParameterType)
		{
			ETERNAL_ASSERT(InRootSignatureParameterType != RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE);
			return HLSL_REGISTER_TYPES[static_cast<int32_t>(InRootSignatureParameterType)];
		}

		uint32_t ConvertRootSignatureParameterTypeToHLSLRegisterTypeUInt(_In_ const RootSignatureParameterType& InRootSignatureParameterType)
		{
			return static_cast<uint32_t>(ConvertRootSignatureParameterTypeToHLSLRegisterType(InRootSignatureParameterType));
		}
	}
}

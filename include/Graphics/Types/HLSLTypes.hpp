#pragma once

namespace Eternal
{
	namespace Graphics
	{
		enum class RootSignatureParameterType;

		enum class HLSLRegisterType
		{
			HLSL_REGISTER_TYPE_SRV = 0,
			HLSL_REGISTER_TYPE_CBV,
			HLSL_REGISTER_TYPE_UAV,
			HLSL_REGISTER_TYPE_SAMPLER,
			HLSL_REGISTER_TYPE_COUNT,
			HLSL_REGISTER_TYPE_INVALID = HLSL_REGISTER_TYPE_COUNT
		};

		const HLSLRegisterType& ConvertRootSignatureParameterTypeToHLSLRegisterType(_In_ const RootSignatureParameterType& InRootSignatureParameterType);
		uint32_t ConvertRootSignatureParameterTypeToHLSLRegisterTypeUInt(_In_ const RootSignatureParameterType& InRootSignatureParameterType);
	}
}

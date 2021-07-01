#include "Graphics/RootSignature.hpp"

namespace Eternal
{
	namespace Graphics
	{
		bool RootSignatureDescriptorTable::operator==(_In_ const RootSignatureDescriptorTable& InOtherRootSignatureRootDescriptorTable) const
		{
			bool IsEqual = Parameters.size() == InOtherRootSignatureRootDescriptorTable.Parameters.size();
			if (IsEqual)
			{
				for (uint32_t ParameterIndex = 0; ParameterIndex < Parameters.size(); ++ParameterIndex)
					Parameters[ParameterIndex] == InOtherRootSignatureRootDescriptorTable.Parameters[ParameterIndex];
			}
			return IsEqual;
		}

		bool RootSignatureParameter::operator==(_In_ const RootSignatureParameter& InOther) const
		{
			bool IsEqual = Parameter == InOther.Parameter
						&& Access == InOther.Access;

			if (Parameter == RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_SAMPLER)
				IsEqual &= SamplerParameter == InOther.SamplerParameter;

			if (Parameter == RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE)
				IsEqual &= DescriptorTable == InOther.DescriptorTable;

			return IsEqual;
		}

		bool RootSignatureDescriptorTableParameter::operator==(_In_ const RootSignatureDescriptorTableParameter& InOtherRootSignatureDescriptorTableParameter) const
		{
			ETERNAL_ASSERT(Parameter != RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE);

			bool IsEqual = static_cast<const RootSignatureParameter&>(*this) == static_cast<const RootSignatureParameter&>(InOtherRootSignatureDescriptorTableParameter);
			IsEqual &= DescriptorsCount == InOtherRootSignatureDescriptorTableParameter.DescriptorsCount;
			if (Parameter == RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_SAMPLER)
			{
				IsEqual &= Samplers.size() == InOtherRootSignatureDescriptorTableParameter.Samplers.size();
				if (IsEqual)
				{
					for (uint32_t SamplerIndex = 0; SamplerIndex < Samplers.size(); ++SamplerIndex)
						IsEqual &= Samplers[SamplerIndex] == InOtherRootSignatureDescriptorTableParameter.Samplers[SamplerIndex];
				}
			}
			return IsEqual;
		}

		bool RootSignature::operator==(_In_ const RootSignature& InOtherRootSignature) const
		{
			bool IsEqual = true;

			const bool ThisHasInputAssembler								= _CreateInformation.HasInputAssembler;
			const bool OtherHasInputAssembler								= InOtherRootSignature._CreateInformation.HasInputAssembler;
			const vector<RootSignatureParameter>& ThisParameters			= _CreateInformation.Parameters;
			const vector<RootSignatureParameter>& OtherParameters			= InOtherRootSignature._CreateInformation.Parameters;
			const vector<RootSignatureStaticSampler>& ThisSamplers			= _CreateInformation.StaticSamplers;
			const vector<RootSignatureStaticSampler>& OtherStaticSamplers	= InOtherRootSignature._CreateInformation.StaticSamplers;
			const vector<RootSignatureConstants>& ThisConstants				= _CreateInformation.Constants;
			const vector<RootSignatureConstants>& OtherConstants			= InOtherRootSignature._CreateInformation.Constants;

			IsEqual &= ThisHasInputAssembler == OtherHasInputAssembler;
			IsEqual &= ThisParameters.size() == OtherParameters.size();
			IsEqual &= ThisSamplers.size() == OtherStaticSamplers.size();
			IsEqual &= ThisConstants.size() == ThisConstants.size();

			for (uint32_t ParameterIndex = 0; ParameterIndex < ThisParameters.size(); ++ParameterIndex)
				IsEqual &= ThisParameters[ParameterIndex] == OtherParameters[ParameterIndex];

			if (IsEqual)
			{
				for (uint32_t SamplerIndex = 0; SamplerIndex < ThisSamplers.size(); ++SamplerIndex)
					IsEqual &= ThisSamplers[SamplerIndex] == OtherStaticSamplers[SamplerIndex];
			}

			if (IsEqual)
			{
				for (uint32_t ConstantIndex = 0; ConstantIndex < ThisConstants.size(); ++ConstantIndex)
					IsEqual &= ThisConstants[ConstantIndex] == OtherConstants[ConstantIndex];
			}

			return IsEqual;
		}
	}
}

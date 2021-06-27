#pragma once

#include <vector>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		struct RootSignatureDescriptorTableParameter;
		
		class Sampler;

		enum class RootSignatureAccess
		{
			ROOT_SIGNATURE_ACCESS_VS = 0,
			ROOT_SIGNATURE_ACCESS_HS,
			ROOT_SIGNATURE_ACCESS_DS,
			ROOT_SIGNATURE_ACCESS_GS,
			ROOT_SIGNATURE_ACCESS_PS,
			ROOT_SIGNATURE_ACCESS_CS,
			ROOT_SIGNATURE_ACCESS_COUNT,
			ROOT_SIGNATURE_ACCESS_INVALID = ROOT_SIGNATURE_ACCESS_COUNT
		};

		enum class RootSignatureParameterType
		{
			ROOT_SIGNATURE_PARAMETER_SAMPLER = 0,
			ROOT_SIGNATURE_PARAMETER_TEXTURE,
			ROOT_SIGNATURE_PARAMETER_RW_TEXTURE,
			ROOT_SIGNATURE_PARAMETER_STRUCTURED_BUFFER,
			ROOT_SIGNATURE_PARAMETER_RW_STRUCTURED_BUFFER,
			ROOT_SIGNATURE_PARAMETER_CONSTANT_BUFFER,
			ROOT_SIGNATURE_PARAMETER_BUFFER,
			ROOT_SIGNATURE_PARAMETER_RW_BUFFER,
			ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE,
			ROOT_SIGNATURE_PARAMETER_COUNT,
			ROOT_SIGNATURE_PARAMETER_INVALID = ROOT_SIGNATURE_PARAMETER_COUNT
		};

		struct RootSignatureDescriptorTable
		{
			vector<RootSignatureDescriptorTableParameter> Parameters;
		};

		struct RootSignatureConstants
		{
			RootSignatureConstants(_In_ const RootSignatureAccess& InAccess, _In_ const uint32_t InCount)
				: Access(InAccess)
				, Count(InCount)
			{
			}

			RootSignatureAccess	Access	= RootSignatureAccess::ROOT_SIGNATURE_ACCESS_INVALID;
			uint32_t			Count	= 0;
		};

		struct RootSignatureStaticSampler
		{
			RootSignatureStaticSampler(_In_ const RootSignatureAccess& InAccess, _In_ Sampler* InStaticSampler)
				: Access(InAccess)
				, StaticSampler(InStaticSampler)
			{
			}

			RootSignatureAccess	Access			= RootSignatureAccess::ROOT_SIGNATURE_ACCESS_INVALID;
			Sampler*			StaticSampler	= nullptr;
		};

		struct RootSignatureParameter
		{
			RootSignatureParameter(_In_ const RootSignatureParameterType& InParameter, _In_ const RootSignatureAccess& InAccess)
				: Parameter(InParameter)
				, Access(InAccess)
			{
				ETERNAL_ASSERT(InParameter != RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_INVALID);
				ETERNAL_ASSERT(InParameter != RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_SAMPLER);
				ETERNAL_ASSERT(InParameter != RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE);
			}

			RootSignatureParameter(_In_ const RootSignatureDescriptorTable& InDescriptorTable, _In_ const RootSignatureAccess& InAccess)
				: Parameter(RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE)
				, Access(InAccess)
				, DescriptorTable(InDescriptorTable)
			{
			}

			RootSignatureParameter(_In_ Sampler* InSampler, _In_ const RootSignatureAccess& InAccess)
				: Parameter(RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_SAMPLER)
				, Access(InAccess)
				, SamplerParameter(InSampler)
			{
			}

			bool operator==(_In_ const RootSignatureParameter& InOther) const
			{
				return Parameter == InOther.Parameter
					&& Access == InOther.Access;
			}


			RootSignatureParameterType			Parameter			= RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_COUNT;
			RootSignatureAccess					Access				= RootSignatureAccess::ROOT_SIGNATURE_ACCESS_INVALID;
			RootSignatureDescriptorTable		DescriptorTable;
			Sampler*							SamplerParameter	= nullptr;
		};

		struct RootSignatureDescriptorTableParameter : public RootSignatureParameter
		{
			RootSignatureDescriptorTableParameter(_In_ const RootSignatureParameterType& InParameter, _In_ const RootSignatureAccess& InAccess, _In_ uint32_t InDescriptorCount)
				: RootSignatureParameter(InParameter, InAccess)
				, DescriptorCount(InDescriptorCount)
			{
			}

			RootSignatureDescriptorTableParameter(_In_ const vector<Sampler*>& InSamplers, _In_ const RootSignatureAccess& InAccess)
				: RootSignatureParameter(RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_SAMPLER, InAccess)
				, Samplers(InSamplers)
			{
			}

			uint32_t						DescriptorCount = 0;
			vector<Sampler*>				Samplers;

		private:
			// Forbidden constructors
			RootSignatureDescriptorTableParameter(_In_ const RootSignatureDescriptorTable& InDescriptorTable, _In_ const RootSignatureAccess& InAccess)
				: RootSignatureParameter(InDescriptorTable, InAccess)
			{
			}

			RootSignatureDescriptorTableParameter(_In_ const RootSignatureParameterType& InParameter, _In_ const RootSignatureAccess& InAccess)
				: RootSignatureParameter(InParameter, InAccess)
			{
			}

			RootSignatureDescriptorTableParameter(_In_ Sampler* InSampler, _In_ const RootSignatureAccess& InAccess)
				: RootSignatureParameter(InSampler, InAccess)
			{
			}
		};

		struct RootSignatureCreateInformation
		{
			RootSignatureCreateInformation()
			{
			}

			RootSignatureCreateInformation(
				_In_ const vector<RootSignatureParameter>& InParameters,
				_In_ const vector<RootSignatureStaticSampler>& InStaticSamplers,
				_In_ const vector<RootSignatureConstants>& InConstants = vector<RootSignatureConstants>(),
				bool InHasInputAssembler = false
			)
				: Parameters(InParameters)
				, StaticSamplers(InStaticSamplers)
				, Constants(InConstants)
				, HasInputAssembler(InHasInputAssembler)
			{
			}

			vector<RootSignatureParameter>		Parameters;
			vector<RootSignatureStaticSampler>	StaticSamplers;
			vector<RootSignatureConstants>		Constants;
			bool								HasInputAssembler = false;
		};

		class RootSignature
		{
		public:
			RootSignature(_In_ const RootSignatureCreateInformation& InRootSignatureCreateInformation)
				: _CreateInformation(InRootSignatureCreateInformation)
			{
			}

			RootSignature() {}

			virtual ~RootSignature() {}

		private:
			RootSignatureCreateInformation _CreateInformation;
		};
	}
}

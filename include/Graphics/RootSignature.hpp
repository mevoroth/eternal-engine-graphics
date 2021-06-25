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
			ROOT_SIGNATURE_ACCESS_ALL = 0,
			ROOT_SIGNATURE_ACCESS_VS,
			ROOT_SIGNATURE_ACCESS_HS,
			ROOT_SIGNATURE_ACCESS_DS,
			ROOT_SIGNATURE_ACCESS_GS,
			ROOT_SIGNATURE_ACCESS_PS,
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

		struct RootSignatureParameter
		{
			RootSignatureParameter(_In_ const RootSignatureParameterType& InParameter, _In_ const RootSignatureAccess& InAccess, _In_ uint32_t InRegister)
				: Parameter(InParameter)
				, Access(InAccess)
			{
			}

			RootSignatureParameter(_In_ const RootSignatureParameterType& InParameter, _In_ const RootSignatureAccess& InAccess, _In_ uint32_t InRegister, _In_ const RootSignatureDescriptorTable& InDescriptorTable)
				: Parameter(InParameter)
				, Access(InAccess)
				, DescriptorTable(InDescriptorTable)
			{
			}

			bool operator==(_In_ const RootSignatureParameter& Other) const
			{
				return Parameter == Other.Parameter
					&& Access == Other.Access;
			}

			RootSignatureParameterType		Parameter	= RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_COUNT;
			RootSignatureAccess				Access		= RootSignatureAccess::ROOT_SIGNATURE_ACCESS_ALL;
			RootSignatureDescriptorTable	DescriptorTable;
		};

		struct RootSignatureDescriptorTableParameter : public RootSignatureParameter
		{
			RootSignatureDescriptorTableParameter(_In_ const RootSignatureParameterType& InParameter, _In_ const RootSignatureAccess& InAccess, _In_ uint32_t InRegister, _In_ uint32_t InDescriptorCount)
				: RootSignatureParameter(InParameter, InAccess, InRegister)
				, DescriptorCount(InDescriptorCount)
			{
			}

			uint32_t						DescriptorCount = 0;
		};

		struct RootSignatureCreateInformation
		{
			RootSignatureCreateInformation()
			{
			}

			RootSignatureCreateInformation(_In_ const vector<RootSignatureParameter>& InParameters, _In_ const vector<Sampler*>& InStaticSamplers)
			{
			}

			vector<RootSignatureParameter>	Parameters;
			vector<Sampler*>				StaticSamplers;
			bool							HasInputAssembler = false;
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

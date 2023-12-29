#include "Graphics/RootSignature.hpp"

#include "Graphics/Device.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Vulkan/VulkanDescriptorTable.hpp"

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
					IsEqual &= Parameters[ParameterIndex] == InOtherRootSignatureRootDescriptorTable.Parameters[ParameterIndex];
			}
			return IsEqual;
		}

		bool RootSignatureParameter::operator==(_In_ const RootSignatureParameter& InOther) const
		{
			bool IsEqual = Parameter == InOther.Parameter
						&& Access == InOther.Access;

			if (Parameter == RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE)
				IsEqual &= DescriptorTable == InOther.DescriptorTable;

			return IsEqual;
		}

		bool RootSignatureDescriptorTableParameter::operator==(_In_ const RootSignatureDescriptorTableParameter& InOtherRootSignatureDescriptorTableParameter) const
		{
			ETERNAL_ASSERT(Parameter != RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE);

			bool IsEqual = static_cast<const RootSignatureParameter&>(*this) == static_cast<const RootSignatureParameter&>(InOtherRootSignatureDescriptorTableParameter);
			IsEqual &= DescriptorsCount == InOtherRootSignatureDescriptorTableParameter.DescriptorsCount;
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

		DescriptorTable* RootSignature::CreateRootDescriptorTable(_In_ GraphicsContext& InContext) const
		{
			switch (InContext.GetDevice().GetDeviceType())
			{
#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
#endif
			case DeviceType::DEVICE_TYPE_NULL:
			case DeviceType::DEVICE_TYPE_PROXY:
				return new DescriptorTable(this);
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
				return new VulkanDescriptorTable(InContext, this);
#endif
			default:
#if ETERNAL_USE_PRIVATE
				return _CreateRootDescriptorTablePrivate(InContext);
#endif
				break;
			}

			ETERNAL_BREAK();
			return nullptr;
		}

		DescriptorTable* RootSignature::CreateSubDescriptorTable(_In_ GraphicsContext& InContext, _In_ uint32_t SubDescriptorTableIndex) const
		{
			const vector<RootSignatureParameter>& Parameters = _CreateInformation.Parameters;
			uint32_t DescriptorTableCount = 0;
			for (uint32_t ParameterIndex = 0; ParameterIndex < Parameters.size(); ++ParameterIndex)
			{
				if (Parameters[ParameterIndex].Parameter == RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE)
				{
					if (DescriptorTableCount++ == SubDescriptorTableIndex)
					{
						switch (InContext.GetDevice().GetDeviceType())
						{
#if ETERNAL_ENABLE_D3D12
						case DeviceType::DEVICE_TYPE_D3D12:
#endif
						case DeviceType::DEVICE_TYPE_NULL:
						case DeviceType::DEVICE_TYPE_PROXY:
							return new DescriptorTable(Parameters[ParameterIndex].DescriptorTable);
#if ETERNAL_ENABLE_VULKAN
						case DeviceType::DEVICE_TYPE_VULKAN:
							return new VulkanDescriptorTable(
								InContext,
								Parameters[ParameterIndex].DescriptorTable,
								*this,
								SubDescriptorTableIndex
							);
#endif
						default:
#if ETERNAL_USE_PRIVATE
							return _CreateSubDescriptorTable(InContext, SubDescriptorTableIndex, Parameters[ParameterIndex].DescriptorTable);
#endif
							break;
						}
					}
				}
			}
			ETERNAL_BREAK();
			return nullptr;
		}

		void RootSignature::CreateDescriptorTables(_In_ GraphicsContext& InContext, _Out_ vector<DescriptorTable*>& OutDescriptorTables)
		{
			const vector<RootSignatureParameter>& Parameters = _CreateInformation.Parameters;
			uint32_t DescriptorTableCount = 0;
			for (uint32_t ParameterIndex = 0; ParameterIndex < Parameters.size(); ++ParameterIndex)
			{
				if (Parameters[ParameterIndex].Parameter == RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE)
					++DescriptorTableCount;
			}
			OutDescriptorTables.resize(
				DescriptorTableCount
				+ (Parameters.size() > DescriptorTableCount ? 1 : 0)
			);

			DescriptorTableCount = 0;
			for (uint32_t ParameterIndex = 0; ParameterIndex < Parameters.size(); ++ParameterIndex)
			{
				if (Parameters[ParameterIndex].Parameter == RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE)
				{
					DescriptorTable* Table = nullptr;
					switch (InContext.GetDevice().GetDeviceType())
					{
#if ETERNAL_ENABLE_D3D12
					case DeviceType::DEVICE_TYPE_D3D12:
#endif
					case DeviceType::DEVICE_TYPE_NULL:
					case DeviceType::DEVICE_TYPE_PROXY:
						Table = new DescriptorTable(Parameters[ParameterIndex].DescriptorTable);
#if ETERNAL_ENABLE_VULKAN
					case DeviceType::DEVICE_TYPE_VULKAN:
						Table = new VulkanDescriptorTable(
							InContext,
							Parameters[ParameterIndex].DescriptorTable,
							*this,
							DescriptorTableCount
						);
#endif
					default:
#if ETERNAL_USE_PRIVATE
						Table = _CreateSubDescriptorTable(InContext, DescriptorTableCount, Parameters[ParameterIndex].DescriptorTable);
#else
						ETERNAL_BREAK();
						break;
#endif
					}
					++DescriptorTableCount;
					OutDescriptorTables.push_back(Table);
				}
			}
		}

		DescriptorTable* RootSignature::_CreateRootDescriptorTablePrivate(_In_ GraphicsContext& InContext) const
		{
			return new DescriptorTable(this);
		}

		DescriptorTable* RootSignature::_CreateSubDescriptorTable(_In_ GraphicsContext& InContext, _In_ uint32_t InSubDescriptorTableIndex, _In_ const RootSignatureDescriptorTable& InRootSignatureDescriptorTable) const
		{
			(void)InContext;
			(void)InSubDescriptorTableIndex;
			return new DescriptorTable(InRootSignatureDescriptorTable);
		}

		void DestroyDescriptorTable(_Inout_ DescriptorTable*& InOutDescriptorTable)
		{
			delete InOutDescriptorTable;
			InOutDescriptorTable = nullptr;
		}
	}
}

#include "Graphics/DescriptorTable.hpp"
#include "Graphics/RootSignature.hpp"

namespace Eternal
{
	namespace Graphics
	{
		DescriptorTable::DescriptorTable(_In_ const RootSignature* InRootSignature)
			: _RootSignature(InRootSignature)
		{
			ETERNAL_ASSERT(InRootSignature);
			GetConstants().resize(InRootSignature->GetCreateInformation().Constants.size());
			GetConstantsDirtyFlags().Resize(static_cast<uint32_t>(InRootSignature->GetCreateInformation().Constants.size()));
			GetResources().resize(InRootSignature->GetCreateInformation().Parameters.size());
			GetResourcesDirtyFlags().Resize(static_cast<uint32_t>(InRootSignature->GetCreateInformation().Parameters.size()));
		}

		DescriptorTable::DescriptorTable(_In_ const RootSignatureDescriptorTable& InRootSignatureDescriptorTable)
		{
			uint32_t ParametersCount = 0;
			for (uint32_t DescriptorTableIndex = 0; DescriptorTableIndex < InRootSignatureDescriptorTable.Parameters.size(); ++DescriptorTableIndex)
				ParametersCount += InRootSignatureDescriptorTable.Parameters[DescriptorTableIndex].DescriptorsCount;
			
			GetResources().resize(ParametersCount);
			GetResourcesDirtyFlags().Resize(ParametersCount);
		}

		template<> void DescriptorTable::SetDescriptor<Sampler>(_In_ uint32_t InSlot, _In_ const Sampler* InDescriptor)
		{
			ETERNAL_ASSERT(InSlot < _Resources.size());
			_Resources[InSlot].ResourceSampler = InDescriptor;
			_ResourcesDirtyFlags.Set(InSlot);
		}

		template<> void DescriptorTable::SetDescriptor<View>(_In_ uint32_t InSlot, _In_ const View* InDescriptor)
		{
			ETERNAL_ASSERT(InSlot < _Resources.size());
			_Resources[InSlot].ResourceView = InDescriptor;
			_ResourcesDirtyFlags.Set(InSlot);
		}

		template<> void DescriptorTable::SetDescriptor<DescriptorTable>(_In_ uint32_t InSlot, _In_ const DescriptorTable* InDescriptor)
		{
			ETERNAL_ASSERT(InSlot < _Resources.size());
			_Resources[InSlot].ResourceDescriptorTable = InDescriptor;
			_ResourcesDirtyFlags.Set(InSlot);
		}
	}
}

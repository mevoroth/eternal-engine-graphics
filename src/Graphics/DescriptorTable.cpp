#include "Graphics/DescriptorTable.hpp"
#include "Graphics/RootSignature.hpp"

namespace Eternal
{
	namespace Graphics
	{
		DescriptorTable::DescriptorTable(_In_ const RootSignature& InRootSignature)
			: _RootSignature(InRootSignature)
		{
			GetConstants().resize(InRootSignature.GetCreateInformation().Constants.size());
			GetConstantsDirtyFlags().Resize(InRootSignature.GetCreateInformation().Constants.size());
			GetResources().resize(InRootSignature.GetCreateInformation().Parameters.size());
			GetResourcesDirtyFlags().Resize(InRootSignature.GetCreateInformation().Parameters.size());
			GetStaticSamplers().resize(InRootSignature.GetCreateInformation().StaticSamplers.size());
			GetStaticSamplersDirtyFlags().Resize(InRootSignature.GetCreateInformation().StaticSamplers.size());
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

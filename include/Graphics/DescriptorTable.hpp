#pragma once

#include "Bit/BitField.hpp"
#include <vector>

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Bit;
		using namespace std;

		class View;
		class Sampler;
		class DescriptorTable;
		class RootSignature;

		using ResourcesDirtyFlagsType = DynamicBitField<uint32_t>;

		struct DescriptorTableConstants
		{
			vector<uint32_t> Constants;
		};

		union DescriptorTableResource
		{
			DescriptorTableResource(_In_ const View* InView)
				: ResourceView(InView)
			{
			}

			DescriptorTableResource(_In_ const Sampler* InSampler)
				: ResourceSampler(InSampler)
			{
			}

			DescriptorTableResource(_In_ const DescriptorTable* InDescriptorTable)
				: ResourceDescriptorTable(InDescriptorTable)
			{
			}

			DescriptorTableResource() {}

			const View*					ResourceView;
			const Sampler*				ResourceSampler;
			const DescriptorTable*		ResourceDescriptorTable;

		private:
			void*					_Dummy = nullptr;
		};

		class DescriptorTable
		{
		public:

			DescriptorTable(_In_ const RootSignature& InRootSignature);

			template<typename DescriptorType>
			void SetDescriptor(_In_ uint32_t InSlot, _In_ const DescriptorType* InDescriptor);

			const vector<DescriptorTableConstants>& GetConstants() const { return _Constants; }
			const vector<DescriptorTableResource>& GetResources() const { return _Resources; }
			const vector<const Sampler*>& GetStaticSamplers() const { return _StaticSamplers; }
			const ResourcesDirtyFlagsType& GetResourcesDirtyFlags() const { return _ResourcesDirtyFlags; }
			ResourcesDirtyFlagsType& GetResourcesDirtyFlags() { return _ResourcesDirtyFlags; }
			const ResourcesDirtyFlagsType& GetConstantsDirtyFlags() const { return _ConstantsDirtyFlags; }
			ResourcesDirtyFlagsType& GetConstantsDirtyFlags() { return _ConstantsDirtyFlags; }
			const ResourcesDirtyFlagsType& GetStaticSamplersDirtyFlags() const { return _StaticSamplersDirtyFlags; }
			ResourcesDirtyFlagsType& GetStaticSamplersDirtyFlags() { return _StaticSamplersDirtyFlags; }
			const RootSignature& GetRootSignature() const { return _RootSignature; }

		protected:
			vector<DescriptorTableConstants>& GetConstants() { return _Constants; }
			vector<DescriptorTableResource>& GetResources() { return _Resources; }
			vector<const Sampler*>& GetStaticSamplers() { return _StaticSamplers; }

		private:
			const RootSignature&				_RootSignature;
			vector<DescriptorTableConstants>	_Constants;
			ResourcesDirtyFlagsType				_ConstantsDirtyFlags;
			vector<DescriptorTableResource>		_Resources;
			ResourcesDirtyFlagsType				_ResourcesDirtyFlags;
			vector<const Sampler*>				_StaticSamplers;
			ResourcesDirtyFlagsType				_StaticSamplersDirtyFlags;
		};
	}
}

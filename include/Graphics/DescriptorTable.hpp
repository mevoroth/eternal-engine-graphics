#pragma once

#include "Bit/BitField.hpp"
#include <vector>

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Bit;
		using namespace std;

		struct RootSignatureDescriptorTable;

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
			void*						_Dummy = nullptr;
		};

		class DescriptorTable
		{
			friend class RootSignature;
		public:

			template<typename DescriptorType>
			void SetDescriptor(_In_ uint32_t InSlot, _In_ const DescriptorType* InDescriptor);

			const vector<DescriptorTableConstants>& GetConstants() const { return _Constants; }
			const vector<DescriptorTableResource>& GetResources() const { return _Resources; }
			const ResourcesDirtyFlagsType& GetResourcesDirtyFlags() const { return _ResourcesDirtyFlags; }
			ResourcesDirtyFlagsType& GetResourcesDirtyFlags() { return _ResourcesDirtyFlags; }
			const ResourcesDirtyFlagsType& GetConstantsDirtyFlags() const { return _ConstantsDirtyFlags; }
			ResourcesDirtyFlagsType& GetConstantsDirtyFlags() { return _ConstantsDirtyFlags; }
			const RootSignature* GetRootSignature() const
			{
				ETERNAL_ASSERT(_RootSignature);
				return _RootSignature;
			}

		protected:

			DescriptorTable(_In_ const RootSignature* InRootSignature);
			DescriptorTable(_In_ const RootSignatureDescriptorTable& InRootSignatureDescriptorTable);

			vector<DescriptorTableConstants>& GetConstants() { return _Constants; }
			vector<DescriptorTableResource>& GetResources() { return _Resources; }

		private:

			const RootSignature*				_RootSignature = nullptr;
			vector<DescriptorTableConstants>	_Constants;
			ResourcesDirtyFlagsType				_ConstantsDirtyFlags;
			vector<DescriptorTableResource>		_Resources;
			ResourcesDirtyFlagsType				_ResourcesDirtyFlags;
		};
	}
}

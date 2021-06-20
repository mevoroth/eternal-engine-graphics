#ifndef _DESCRIPTOR_HEAP_HPP_
#define _DESCRIPTOR_HEAP_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class DescriptorTable;
		class Device;
		struct RootSignatureParameter;

		class DescriptorHeap
		{
		public:
			DescriptorHeap(_In_ const RootSignatureParameter Resources[], _In_ uint32_t ResourcesCount);

			virtual ~DescriptorHeap() {}

			virtual DescriptorTable* CreateView(_In_ Device& DeviceObj) = 0;

		protected:
			static bool IsD3D12Compatible(_In_ const RootSignatureParameter Resources[], _In_ uint32_t ResourcesCount);

			uint32_t GetResourcesCount() const { return _ResourcesCount; }

		private:
			uint32_t _ResourcesCount = 0;
		};
	}
}

#endif

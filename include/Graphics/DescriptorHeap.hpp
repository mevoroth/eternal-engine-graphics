#ifndef _DESCRIPTOR_HEAP_HPP_
#define _DESCRIPTOR_HEAP_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		class View;
		class Device;
		struct RootSignatureParameter;

		class DescriptorHeap
		{
		public:
			DescriptorHeap(_In_ const RootSignatureParameter Resources[], _In_ uint32_t ResourcesCount);

			virtual ~DescriptorHeap() {}

			virtual View* CreateView(_In_ Device& DeviceObj) = 0;

		protected:
			static bool IsD3D12Compatible(_In_ const RootSignatureParameter Resources[], _In_ uint32_t ResourcesCount);
		};
	}
}

#endif

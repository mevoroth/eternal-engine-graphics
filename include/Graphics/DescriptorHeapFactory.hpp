#ifndef _DESCRIPTOR_HEAP_FACTORY_HPP_
#define _DESCRIPTOR_HEAP_FACTORY_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		struct RootSignatureParameter;
		class Device;
		class DescriptorHeap;

		//DescriptorHeap* CreateDescriptorHeap(_In_ Device& DeviceObj, _In_ uint32_t Space, _In_ uint32_t Register, _In_ const RootSignatureParameterType& HeapTypeObj, _In_ uint32_t ResourcesCount, _In_ const RootSignatureAccess& RootSignatureAccessObj);
		DescriptorHeap* CreateDescriptorHeap(_In_ Device& DeviceObj, _In_ const RootSignatureParameter Resources[], _In_ uint32_t ResourcesCount);
	}
}

#endif

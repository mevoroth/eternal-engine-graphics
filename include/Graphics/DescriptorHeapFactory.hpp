#ifndef _DESCRIPTOR_HEAP_FACTORY_HPP_
#define _DESCRIPTOR_HEAP_FACTORY_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		enum RootSignatureDynamicParameterType;
		enum RootSignatureAccess;
		class Device;
		class DescriptorHeap;

		DescriptorHeap* CreateDescriptorHeap(_In_ Device& DeviceObj, _In_ uint32_t Space, _In_ const RootSignatureDynamicParameterType& HeapTypeObj, _In_ uint32_t ResourcesCount, _In_ const RootSignatureAccess& RootSignatureAccessObj);
	}
}

#endif

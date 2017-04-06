#ifndef _ROOT_SIGNATURE_FACTORY_HPP_
#define _ROOT_SIGNATURE_FACTORY_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class RootSignature;
		class Sampler;
		class DescriptorHeap;
		enum RootSignatureAccess;

		RootSignature* CreateRootSignature(_In_ Device& DeviceObj);
		RootSignature* CreateRootSignature(_In_ Device& DeviceObj, _In_ Sampler* StaticSamplers[], _In_ uint32_t StaticSamplersCount, _In_ DescriptorHeap* DescriptorHeaps[], _In_ uint32_t DescriptorHeapsCount, _In_ const RootSignatureAccess& RootSignatureAccessObj);
	}
}

#endif

#ifndef _ROOT_SIGNATURE_FACTORY_HPP_
#define _ROOT_SIGNATURE_FACTORY_HPP_

#include <cstdint>
#include <vector>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;
		class RootSignature;
		//class Sampler;
		//class DescriptorHeap;
		struct RootSignatureParameter;
		enum RootSignatureAccess;

		RootSignature* CreateRootSignature(_In_ Device& DeviceObj);
		//RootSignature* CreateRootSignature(_In_ Device& DeviceObj, _In_ Sampler* StaticSamplers[], _In_ uint32_t StaticSamplersCount, _In_ DescriptorHeap* DescriptorHeaps[], _In_ uint32_t DescriptorHeapsCount, _In_ const RootSignatureAccess& RootSignatureAccessObj);
		RootSignature* CreateRootSignature(_In_ Device& DeviceObj, _In_ const vector<RootSignatureParameter> Resources[], _In_ uint32_t ResourcesCount, _In_ const RootSignatureAccess& RootSignatureAccessObj);
	}
}

#endif

#ifndef _D3D12_ROOT_SIGNATURE_HPP_
#define _D3D12_ROOT_SIGNATURE_HPP_

#include <vector>
#include "Graphics/RootSignature.hpp"

struct ID3D12RootSignature;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		enum class RootSignatureAccess;

		class Device;
		class Sampler;
		class DescriptorHeap;

		class D3D12RootSignature : public RootSignature
		{
		public:
			D3D12RootSignature(_In_ Device& DeviceObj, _In_ const vector<RootSignatureParameter> Resources[], _In_ uint32_t ResourcesCount, _In_ const RootSignatureAccess& RootSignatureAccessObj);
			//D3D12RootSignature(_In_ Device& DeviceObj, _In_ Sampler* StaticSamplers[], _In_ uint32_t StaticSamplersCount, _In_ DescriptorHeap* DescriptorHeaps[], _In_ uint32_t DescriptorHeapsCount, _In_ const RootSignatureAccess& RootSignatureAccessObj);
			D3D12RootSignature(_In_ Device& DeviceObj);
			~D3D12RootSignature();

			ID3D12RootSignature* GetD3D12RootSignature() { return _RootSignature; }

		private:
			ID3D12RootSignature* _RootSignature = nullptr;
		};
	}
}

#endif

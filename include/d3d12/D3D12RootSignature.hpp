#ifndef _D3D12_ROOT_SIGNATURE_HPP_
#define _D3D12_ROOT_SIGNATURE_HPP_

#include "Graphics/RootSignature.hpp"

struct ID3D12RootSignature;

namespace Eternal
{
	namespace Graphics
	{
		class Device;

		class D3D12RootSignature : public RootSignature
		{
		public:
			D3D12RootSignature(_In_ Device& DeviceObj);
			~D3D12RootSignature();

			ID3D12RootSignature* GetD3D12RootSignature() { return _RootSignature; }

		private:
			ID3D12RootSignature* _RootSignature = nullptr;
		};
	}
}

#endif

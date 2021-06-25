#pragma once

#include "Graphics/View.hpp"
#include "d3d12/D3D12Utils.hpp"
#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		extern const D3D12_RTV_DIMENSION D3D12_RTV_DIMENSIONS[];

		class D3D12View : public View
		{
		public:
			D3D12View(_In_ const RenderTargetViewCreateInformation& InViewCreateInformation);
			~D3D12View();

			D3D12_CPU_DESCRIPTOR_HANDLE GetD3D12CPUDescriptorHandle() const { return _D3D12Handle.D3D12CPUDescriptorHandle; }

		private:
			D3D12Handle _D3D12Handle;
		};
	}
}

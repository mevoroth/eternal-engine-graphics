#pragma once

#include "Graphics/View.hpp"
#include "Bit/BitField.hpp"
#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Bit;

		extern const D3D12_RTV_DIMENSION D3D12_RTV_DIMENSIONS[];

		class D3D12View : public View
		{
		public:
			D3D12View(_In_ const RenderTargetViewCreateInformation& InViewCreateInformation);
			~D3D12View();

			D3D12_CPU_DESCRIPTOR_HANDLE GetD3D12CPUDescriptorHandle() const { return _D3D12CPUDescriptorHandle; }

		private:
			Handle _ViewHandle	= Handle::InvalidHandle;
			D3D12_CPU_DESCRIPTOR_HANDLE	_D3D12CPUDescriptorHandle;
		};
	}
}

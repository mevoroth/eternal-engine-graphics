#ifndef _D3D12_RENDER_TARGET_HPP_
#define _D3D12_RENDER_TARGET_HPP_

#include "NextGenGraphics/RenderTarget.hpp"
#include <cstdint>
#include "d3d12/D3D12Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class SwapChain;
		class D3D12DescriptorHeap;

		class D3D12RenderTarget
			: public D3D12Resource
			, public RenderTarget
		{
		public:
			D3D12RenderTarget(_In_ Device& DeviceObj, _In_ SwapChain& SwapChainObj, _In_ D3D12DescriptorHeap& DescriptorHeap, _In_ uint32_t BackBufferFrameIndex);
		};
	}
}

#endif

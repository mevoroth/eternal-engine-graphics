#pragma once

#include "Graphics/View.hpp"

#if ETERNAL_ENABLE_D3D12

#include "d3d12/D3D12Utils.hpp"
#include "d3d12/D3D12Library.h"

namespace Eternal
{
	namespace Graphics
	{
		class D3D12View final : public View
		{
		public:
			D3D12View(_In_ const RenderTargetViewCreateInformation& InViewCreateInformation);
			D3D12View(_In_ const ConstantBufferViewCreateInformation& InViewCreateInformation);
			D3D12View(_In_ const ShaderResourceViewCreateInformation& InViewCreateInformation);
			D3D12View(_In_ const UnorderedAccessViewCreateInformation& InViewCreateInformation);
			D3D12View(_In_ const DepthStencilViewCreateInformation& InViewCreateInformation);
			virtual ~D3D12View() override final;

			const D3D12_GPU_DESCRIPTOR_HANDLE& GetD3D12GPUDescriptorHandle() const { return _D3D12Handle.D3D12GPUDescriptorHandle; }
			const D3D12_CPU_DESCRIPTOR_HANDLE& GetD3D12CPUDescriptorHandle() const { return _D3D12Handle.D3D12CPUDescriptorHandle; }

			// Constant buffer
			D3D12_GPU_VIRTUAL_ADDRESS GetD3D12OffsettedConstantBuffer() const;

			// Structured buffer
			D3D12_GPU_VIRTUAL_ADDRESS GetD3D12OffsettedBuffer() const;

		private:
			D3D12Handle _D3D12Handle;
		};
	}
}

#endif

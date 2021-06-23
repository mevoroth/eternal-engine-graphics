#pragma once

#include "Graphics/GraphicsContext.hpp"
#include "Bit/BitField.hpp"

struct ID3D12DescriptorHeap;

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Bit;

		class D3D12GraphicsContext : public GraphicsContext
		{
		public:
			static constexpr uint32_t MaxConstantBufferViewCount	= 2048;
			static constexpr uint32_t MaxShaderResourceViewCount	= 16384;
			static constexpr uint32_t MaxUnorderedAccessViewCount	= 2048;
			static constexpr uint32_t MaxSamplerCount				= 128;
			static constexpr uint32_t MaxRenderTargetViewCount		= 2048;
			static constexpr uint32_t MaxDepthStencilViewCount		= 256;

			D3D12GraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation);
			~D3D12GraphicsContext();

			inline ID3D12DescriptorHeap* GetCBV_SRV_UAV_DescriptorHeap() { return _CBV_SRV_UAV_DescriptorHeap; }
			inline ID3D12DescriptorHeap* GetSamplerDescriptorHeap() { return _SamplerDescriptorHeap; }
			inline ID3D12DescriptorHeap* GetRenderTargetViewDescriptorHeap() { return _RenderTargetViewDescriptorHeap; }
			inline ID3D12DescriptorHeap* GetDepthStencilViewDescriptorHeap() { return _DepthStencilViewDescriptorHeap; }

		private:
			ID3D12DescriptorHeap* _CBV_SRV_UAV_DescriptorHeap		= nullptr;
			ID3D12DescriptorHeap* _SamplerDescriptorHeap			= nullptr;
			ID3D12DescriptorHeap* _RenderTargetViewDescriptorHeap	= nullptr;
			ID3D12DescriptorHeap* _DepthStencilViewDescriptorHeap	= nullptr;

			BitField<MaxConstantBufferViewCount>	_ConstantBufferViewHandles;
			BitField<MaxShaderResourceViewCount>	_ShaderResourceViewHandles;
			BitField<MaxUnorderedAccessViewCount>	_UnorderedAccessViewHandles;
			BitField<MaxSamplerCount>				_SamplerHandles;
			BitField<MaxRenderTargetViewCount>		_RenderTargetViewHandles;
			BitField<MaxDepthStencilViewCount>		_DepthStencilViewHandles;
		};
	}
}

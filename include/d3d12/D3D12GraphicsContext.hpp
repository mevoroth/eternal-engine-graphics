#pragma once

#include "Graphics/GraphicsContext.hpp"
#include "Bit/BitField.hpp"

struct ID3D12DescriptorHeap;
struct D3D12_CPU_DESCRIPTOR_HANDLE;

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

			D3D12_CPU_DESCRIPTOR_HANDLE AllocateConstantBufferViewDescriptor(_Out_ Handle& OutHandle);
			D3D12_CPU_DESCRIPTOR_HANDLE AllocateShaderResourceViewDescriptor(_Out_ Handle& OutHandle);
			D3D12_CPU_DESCRIPTOR_HANDLE AllocateUnorderedAccessViewDescriptor(_Out_ Handle& OutHandle);
			D3D12_CPU_DESCRIPTOR_HANDLE AllocateSamplerDescriptor(_Out_ Handle& OutHandle);
			D3D12_CPU_DESCRIPTOR_HANDLE AllocateRenderTargetViewDescriptor(_Out_ Handle& OutHandle);
			D3D12_CPU_DESCRIPTOR_HANDLE AllocateDepthStencilViewDescriptor(_Out_ Handle& OutHandle);

			void ReleaseConstantBufferViewDescriptor(_Inout_ Handle& InOutHandle);
			void ReleaseShaderResourceViewDescriptor(_Inout_ Handle& InOutHandle);
			void ReleaseUnorderedAccessViewDescriptor(_Inout_ Handle& InOutHandle);
			void ReleaseSamplerDescriptor(_Inout_ Handle& InOutHandle);
			void ReleaseRenderTargetViewDescriptor(_Inout_ Handle& InOutHandle);
			void ReleaseDepthStencilViewDescriptor(_Inout_ Handle& InOutHandle);

			inline ID3D12DescriptorHeap* GetCBV_SRV_UAV_DescriptorHeap() { return _CBV_SRV_UAV_DescriptorHeap; }
			inline ID3D12DescriptorHeap* GetSamplerDescriptorHeap() { return _SamplerDescriptorHeap; }
			inline ID3D12DescriptorHeap* GetRenderTargetViewDescriptorHeap() { return _RenderTargetViewDescriptorHeap; }
			inline ID3D12DescriptorHeap* GetDepthStencilViewDescriptorHeap() { return _DepthStencilViewDescriptorHeap; }

		private:
			uint32_t _CBV_SRV_UAV_DescriptorHandleIncrementSize		= 0;
			uint32_t _SamplerDescriptorHandleIncrementSize			= 0;
			uint32_t _RenderTargetViewDescriptorHandleIncrementSize	= 0;
			uint32_t _DepthStencilViewDescriptorHandleIncrementSize	= 0;

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

#pragma once

#if ETERNAL_ENABLE_D3D12

#if ETERNAL_PLATFORM_WINDOWS
#include "Windows/WindowsGraphicsContext.hpp"
#endif
#include "Bit/BitField.hpp"
#include "d3d12/D3D12Utils.hpp"

struct ID3D12DescriptorHeap;
struct D3D12_CPU_DESCRIPTOR_HANDLE;

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Bit;

		class D3D12GraphicsContext final : public WindowsGraphicsContext
		{
		public:
			static constexpr uint32_t MaxConstantBufferViewCount	= 2048;
			static constexpr uint32_t MaxShaderResourceViewCount	= 16384;
			static constexpr uint32_t MaxUnorderedAccessViewCount	= 2048;
			static constexpr uint32_t MaxSamplerCount				= 128;
			static constexpr uint32_t MaxRenderTargetViewCount		= 2048;
			static constexpr uint32_t MaxDepthStencilViewCount		= 256;

			D3D12GraphicsContext(_In_ const WindowsGraphicsContextCreateInformation& InWindowsGraphicsContextCreateInformation);
			~D3D12GraphicsContext();

			virtual void ResetFrameStates() override final;

			D3D12Handle AllocateConstantBufferViewDescriptor();
			D3D12Handle AllocateShaderResourceViewDescriptor();
			D3D12Handle AllocateUnorderedAccessViewDescriptor();
			D3D12Handle AllocateSamplerDescriptor();
			D3D12Handle AllocateRenderTargetViewDescriptor();
			D3D12Handle AllocateDepthStencilViewDescriptor();

			void ReleaseConstantBufferViewDescriptor(_Inout_ D3D12Handle& InOutHandle);
			void ReleaseShaderResourceViewDescriptor(_Inout_ D3D12Handle& InOutHandle);
			void ReleaseUnorderedAccessViewDescriptor(_Inout_ D3D12Handle& InOutHandle);
			void ReleaseSamplerDescriptor(_Inout_ D3D12Handle& InOutHandle);
			void ReleaseRenderTargetViewDescriptor(_Inout_ D3D12Handle& InOutHandle);
			void ReleaseDepthStencilViewDescriptor(_Inout_ D3D12Handle& InOutHandle);

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

			StaticHandlePool<MaxConstantBufferViewCount>	_ConstantBufferViewHandles;
			StaticHandlePool<MaxShaderResourceViewCount>	_ShaderResourceViewHandles;
			StaticHandlePool<MaxUnorderedAccessViewCount>	_UnorderedAccessViewHandles;
			StaticHandlePool<MaxSamplerCount>				_SamplerHandles;
			StaticHandlePool<MaxRenderTargetViewCount>		_RenderTargetViewHandles;
			StaticHandlePool<MaxDepthStencilViewCount>		_DepthStencilViewHandles;
		};
	}
}

#endif

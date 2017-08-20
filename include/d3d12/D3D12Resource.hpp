#ifndef _D3D12_RESOURCE_HPP_
#define _D3D12_RESOURCE_HPP_

#include <cstdint>
#include <d3d12.h>
#include "NextGenGraphics/Resource.hpp"

struct ID3D12Resource;

namespace Eternal
{
	namespace Graphics
	{
		enum TransitionState;
		enum ResourceState
		{
			COMMON			= 0x0,
			RENDERTARGET	= 0x4,
			PRESENT			= 0x0
		};

		class Heap;
		class D3D12CommandList;

		class D3D12Resource : public Resource
		{
		public:
			D3D12Resource(Device& DeviceObj, _In_ Heap& HeapObj, _In_ const ResourceDimension& Dimension, _In_ const Format& FormatObj, const TextureType& Type, _In_ uint32_t Width, _In_ uint32_t Height, _In_ uint32_t Depth, _In_ uint32_t MipCount, _In_ const TransitionState& State);
			D3D12Resource(_In_ Device& DeviceObj, _In_ Heap& HeapObj, _In_ uint64_t Stride, _In_ uint64_t Size, _In_ const BufferType& Type);
			D3D12Resource(_In_ ID3D12Resource* BackBuffer);
			virtual ~D3D12Resource();

			virtual void SetName(_In_ const wchar_t* Name) override;

			virtual View* CreateView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const TextureView& ViewType, _In_ const Format& FormatObj) override;
			virtual View* CreateView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const Format& FormatObj, _In_ uint64_t Offset, _In_ uint64_t Size) override;
			virtual View* CreateRenderTargetView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const Format& FormatObj) override;
			virtual View* CreateDepthStencilView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const Format& FormatObj) override;

			virtual void* Map(_In_ Device& DeviceObj) override;
			virtual void Unmap(_In_ Device& DeviceObj) override;

			inline ID3D12Resource*& GetD3D12Resource() { return _Resource; }

			TransitionState& GetCurrentTransitionState() { return _DebugCurrentTransition; }

		private:
			ID3D12Resource*		_Resource = nullptr;
			TransitionState		_DebugCurrentTransition;
		};
	}
}

#endif

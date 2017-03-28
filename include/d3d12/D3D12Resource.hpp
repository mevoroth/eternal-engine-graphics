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
			D3D12Resource(_In_ Device& DeviceObj, _In_ Heap& HeapObj);
			D3D12Resource(_In_ ID3D12Resource* BackBuffer);
			virtual ~D3D12Resource();

			virtual View* CreateView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const TextureView& ViewType, _In_ const Format& FormatObj) override;
			virtual View* CreateRenderTargetView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj) override;
			void Transition(_In_ D3D12CommandList& CommandList, _In_ const ResourceState& NewState);

			inline ID3D12Resource*& GetResource() { return _Resource; }

		private:
			ID3D12Resource*		_Resource = nullptr;
			ResourceState		_OldState = COMMON;
		};
	}
}

#endif

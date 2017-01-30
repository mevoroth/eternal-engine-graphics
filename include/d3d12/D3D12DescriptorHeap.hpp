#ifndef _D3D12_DESCRIPTOR_HEAP_HPP_
#define _D3D12_DESCRIPTOR_HEAP_HPP_

#include <d3d12.h>
#include <cstdint>
struct ID3D12DescriptorHeap;

namespace Eternal
{
	namespace Graphics
	{
		class D3D12Device;

		class D3D12DescriptorHeap
		{
		public:
			enum HeapType
			{
				BUFFER			= 0,
				SAMPLER			= 1,
				RENDERTARGET	= 2,
				DEPTHSTENCIL	= 3
			};

			D3D12DescriptorHeap(_In_ D3D12Device& DeviceObj, _In_ const HeapType HeapTypeObj, _In_ uint32_t ResourceCount);

			inline ID3D12DescriptorHeap* GetDescriptorHeap() { return _DescriptorHeap; }
			D3D12_CPU_DESCRIPTOR_HANDLE GetSlot(_In_ uint32_t Slot) const;

		private:
			ID3D12DescriptorHeap* _DescriptorHeap = nullptr;
			D3D12_CPU_DESCRIPTOR_HANDLE _DescriptorHandleStart;
			uint32_t _DescriptorHandleSize = 0;
			uint32_t _ResourceCount = 0;
		};
	}
}

#endif

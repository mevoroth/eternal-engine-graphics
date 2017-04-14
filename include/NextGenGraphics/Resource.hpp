#ifndef _RESOURCE_HPP_
#define _RESOURCE_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		enum Format;
		enum TextureView;
		class View;
		class Device;
		class DescriptorHeap;
		class Heap;

		enum ResourceType
		{
			BUFFER_TRANSFER_READ		= 0x1,
			BUFFER_TRANSFER_WRITE		= 0x2,
			BUFFER_STRUCTURED_READ		= 0x4,
			BUFFER_STRUCTURED_WRITE		= 0x8,
			BUFFER_RAW_BUFFER_READ		= 0x10,
			BUFFER_RAW_BUFFER_WRITE		= 0x20,
			BUFFER_INDEX				= 0x40,
			BUFFER_VERTEX				= 0x80,
			BUFFER_INDIRECT				= 0x100
		};

		class Resource
		{
		public:
			Resource(_In_ Heap& HeapObj, _In_ uint64_t Width = 1ull, _In_ uint64_t Height = 1ull, _In_ uint64_t Depth = 1ull, _In_ uint64_t MipCount = 1ull);
			virtual ~Resource();
			virtual View* CreateView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const TextureView& ViewType, _In_ const Format& FormatObj) = 0;
			virtual View* CreateView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const Format& FormatObj, _In_ uint64_t Offset = 0ull, _In_ uint64_t Size = ~0ull)  = 0;
			virtual View* CreateRenderTargetView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj) = 0;
			virtual void* Map(_In_ Device& DeviceObj) = 0;
			virtual void Unmap(_In_ Device& DeviceObj) = 0;

			uint64_t	GetHeapSlot() const { return _HeapSlot; }
			void		SetHeapSlot(_In_ uint64_t HeapSlot) { _HeapSlot = HeapSlot; }
			Heap&		GetHeap() { return _Heap; }
			uint64_t	GetSize() const;
			uint64_t	GetDepth() const { return _Depth; }
			uint64_t	GetMipCount() const { return _MipCount; }

		private:
			Heap&		_Heap;
			uint64_t	_HeapSlot	= 0xFFFFFFFFFFFFFFFFull;
			uint64_t	_Width		= 1ull;
			uint64_t	_Height		= 1ull;
			uint64_t	_Depth		= 1ull;
			uint64_t	_MipCount	= 1ull;
		};
	}
}

#endif

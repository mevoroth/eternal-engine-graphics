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
			TRANSFER_READ		= 0x1,
			TRANSFER_WRITE		= 0x2,
			STRUCTURED_READ		= 0x4,
			STRUCTURED_WRITE	= 0x8,
			RAW_BUFFER_READ		= 0x10,
			RAW_BUFFER_WRITE	= 0x20,
			INDEX				= 0x40,
			VERTEX				= 0x80,
			INDIRECT			= 0x100
		};

		//VK_BUFFER_USAGE_TRANSFER_SRC_BIT = 0x00000001,
		//	VK_BUFFER_USAGE_TRANSFER_DST_BIT = 0x00000002,
		//	VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT = 0x00000004,
		//	VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT = 0x00000008,
		//	VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT = 0x00000010,
		//	VK_BUFFER_USAGE_STORAGE_BUFFER_BIT = 0x00000020,
		//	VK_BUFFER_USAGE_INDEX_BUFFER_BIT = 0x00000040,
		//	VK_BUFFER_USAGE_VERTEX_BUFFER_BIT = 0x00000080,
		//	VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT = 0x00000100,

		class Resource
		{
		public:
			Resource(_In_ Heap& HeapObj);
			virtual ~Resource();
			virtual View* CreateView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const TextureView& ViewType, _In_ const Format& FormatObj) = 0;
			virtual View* CreateView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const Format& FormatObj, _In_ uint64_t Offset = 0ull, _In_ uint64_t Size = ~0ull)  = 0;
			virtual View* CreateRenderTargetView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj) = 0;

			uint64_t	GetHeapSlot() const { return _HeapSlot; }
			void		SetHeapSlot(_In_ uint64_t HeapSlot) { _HeapSlot = HeapSlot; }
			Heap&		GetHeap() { return _Heap; }

		private:
			Heap&		_Heap;
			uint64_t	_HeapSlot = 0xFFFFFFFFFFFFFFFFull;
		};
	}
}

#endif

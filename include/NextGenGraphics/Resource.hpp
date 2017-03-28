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
			TRANSFER	= 0,
			STRUCTURED,
			RAW_BUFFER,
			INDEX,
			VERTEX,
			INDIRECT
		};

		class Resource
		{
		public:
			Resource(_In_ Heap& HeapObj);
			virtual ~Resource();
			virtual View* CreateView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const TextureView& ViewType, _In_ const Format& FormatObj) = 0;
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

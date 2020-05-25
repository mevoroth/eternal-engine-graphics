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

		enum BufferType
		{
			BUFFER_COPY_READ		= 0x1,
			BUFFER_COPY_WRITE		= 0x2,
			BUFFER_STRUCTURED_READ	= 0x4,
			BUFFER_STRUCTURED_WRITE	= 0x8,
			BUFFER_RAW_BUFFER_READ	= 0x10,
			BUFFER_RAW_BUFFER_WRITE	= 0x20,
			BUFFER_INDEX			= 0x40,
			BUFFER_VERTEX			= 0x80,
			BUFFER_INDIRECT			= 0x100
		};

		enum TextureType
		{
			TEXTURE_COPY_READ		= 0x1,
			TEXTURE_COPY_WRITE		= 0x2,
			TEXTURE_READ			= 0x4,
			TEXTURE_WRITE			= 0x8,
			TEXTURE_RENDER_TARGET	= 0x10,
			TEXTURE_DEPTH_STENCIL	= 0x20,
			TEXTURE_TRANSIENT		= 0x40,
			TEXTURE_BLEND_READ		= 0x80
		};

		enum ResourceDimension
		{
			RESOURCE_DIMENSION_TEXTURE_1D	= 0,
			RESOURCE_DIMENSION_TEXTURE_2D	= 1,
			RESOURCE_DIMENSION_TEXTURE_3D	= 2
		};

		class Resource
		{
		public:
			Resource();
			Resource(_In_ Heap& HeapObj, _In_ const Format& ImageFormat, _In_ uint64_t Width, _In_ uint64_t Height, _In_ uint64_t Depth, _In_ uint64_t MipCount = 1ull);
			Resource(_In_ Heap& HeapObj, _In_ uint64_t Stride, _In_ uint64_t Size);
			virtual ~Resource();

			virtual void SetName(_In_ const wchar_t* Name) = 0;

			virtual View* CreateView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const TextureView& ViewType, _In_ const Format& FormatObj) = 0;
			virtual View* CreateView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const Format& FormatObj, _In_ uint64_t Offset = 0ull, _In_ uint64_t Size = ~0ull)  = 0;
			virtual View* CreateRenderTargetView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const Format& FormatObj) = 0;
			virtual View* CreateDepthStencilView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const Format& FormatObj) = 0;
			virtual void* Map(_In_ Device& DeviceObj) = 0;
			virtual void Unmap(_In_ Device& DeviceObj) = 0;

			uint64_t		GetHeapSlot() const { return _HeapSlot; }
			void			SetHeapSlot(_In_ uint64_t HeapSlot) { _HeapSlot = HeapSlot; }
			Heap&			GetHeap() { return _Heap; }

			uint64_t		GetTextureSize() const			{ return _ResourceDescription.TextureSize; }
			uint64_t		GetDepth() const				{ return _ResourceDescription.Depth; }
			uint64_t		GetMipCount() const				{ return _ResourceDescription.MipCount; }
			const Format&	GetFormat() const				{ return _ResourceDescription.ImageFormat; }

			uint64_t		GetStride() const				{ return _ResourceDescription.Stride; }
			uint64_t		GetBufferSize() const			{ return _ResourceDescription.BufferSize; }

		protected:
			void			SetTextureSize(uint64_t Size)	{ _ResourceDescription.TextureSize = Size; }

		private:
			union ResourceDescription
			{
				// Image
				struct
				{
					uint64_t	TextureSize;
					uint64_t	Width;
					uint64_t	Height;
					uint64_t	Depth;
					uint64_t	MipCount;
					Format		ImageFormat;
				};

				// Buffer
				struct 
				{
					uint64_t BufferSize;
					uint64_t Stride;
				};
			};

			ResourceDescription	_ResourceDescription;
			Heap&				_Heap;
			uint64_t			_HeapSlot	= 0xFFFFFFFFFFFFFFFFull;
		};
	}
}

#endif

#ifndef _VULKAN_RESOURCE_HPP_
#define _VULKAN_RESOURCE_HPP_

#include "NextGenGraphics/Resource.hpp"

struct VkBuffer_T;
struct VkImage_T;

namespace Eternal
{
	namespace Graphics
	{
		enum class Format;
		enum TextureView;
		enum class TransitionState;
		class Device;
		class Heap;
		class VulkanDevice;
		class VulkanView;
		class VulkanHeap;

		class VulkanResource : public Resource
		{
			union VkResource
			{
				VkBuffer_T* Buffer;
				VkImage_T* Image;
			};
		public:
			VulkanResource(_In_ Device& DeviceObj, _In_ Heap& HeapObj, _In_ uint64_t Stride, _In_ uint64_t Size, _In_ const BufferType& Type);
			VulkanResource(_In_ VkImage_T* Image);
			VulkanResource(_In_ Device& DeviceObj, _In_ Heap& HeapObj, _In_ const ResourceDimension& Dimension, _In_ const Format& FormatObj, const TextureType& Type, _In_ uint32_t Width, _In_ uint32_t Height, _In_ uint32_t Depth, _In_ uint32_t MipCount, _In_ const TransitionState& InitialState);
			virtual ~VulkanResource();

			VkBuffer_T*& GetBuffer() { return _Resource.Buffer; }
			VkImage_T*& GetImage() { return _Resource.Image; }

			virtual void SetName(_In_ const wchar_t* Name) override;

			virtual View* CreateView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const TextureView& ViewType, _In_ const Format& FormatObj) override;
			virtual View* CreateView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const Format& FormatObj, _In_ uint64_t Offset = 0ull, _In_ uint64_t Size = ~0ull) override;
			virtual View* CreateRenderTargetView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const Format& FormatObj) override;
			virtual View* CreateDepthStencilView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const Format& FormatObj) override;

			virtual void* Map(_In_ Device& DeviceObj) override;
			virtual void Unmap(_In_ Device& DeviceObj) override;

		private:
			VulkanHeap& GetVulkanHeap();

			VkResource	_Resource;
		};
	}
}

#endif

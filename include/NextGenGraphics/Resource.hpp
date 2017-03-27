#ifndef _RESOURCE_HPP_
#define _RESOURCE_HPP_

namespace Eternal
{
	namespace Graphics
	{
		enum Format;
		enum TextureView;
		class View;
		class Device;
		class DescriptorHeap;

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
			Resource() {}
			virtual ~Resource() {}
			virtual View* CreateView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ const TextureView& ViewType, _In_ const Format& FormatObj) = 0;
			virtual View* CreateRenderTargetView(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj) = 0;
		};
	}
}

#endif

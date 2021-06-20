#ifndef _D3D12_VIEW_HPP_
#define _D3D12_VIEW_HPP_

#include <d3d12.h>
#include "Graphics_deprecated/View.hpp"
#include "d3d12_deprecated/D3D12DescriptorHeap.hpp"

namespace Eternal
{
	namespace Graphics
	{
		enum Format;
		class Device;
		class Resource;
		class DescriptorHeap;

		class D3D12View : public View
		{
		public:
			D3D12View(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ Resource& ResourceObj, _In_ const TextureView& ViewType, _In_ const Format& FormatObj);
			D3D12View(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ Resource& ResourceObj, _In_ const Format& FormatObj);
			//D3D12View(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj);
			virtual ~D3D12View();

			inline DescriptorHeapHandle& GetHandle()				{ return _Handle; }
			inline const DescriptorHeapHandle& GetHandle() const	{ return _Handle; }

		private:
			DescriptorHeapHandle	_Handle;
			D3D12DescriptorHeap&	_DescriptorHeap;
		};
	}
}

#endif

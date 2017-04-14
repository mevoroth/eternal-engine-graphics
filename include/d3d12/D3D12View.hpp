#ifndef _D3D12_VIEW_HPP_
#define _D3D12_VIEW_HPP_

#include <d3d12.h>
#include "Graphics/View.hpp"

namespace Eternal
{
	namespace Graphics
	{
		enum Format;
		class Device;
		class Resource;
		class DescriptorHeap;
		class D3D12DescriptorHeap;

		class D3D12View : public View
		{
		public:
			D3D12View(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ Resource& ResourceObj, _In_ const TextureView& ViewType, _In_ const Format& FormatObj);
			D3D12View(_In_ Device& DeviceObj, _In_ DescriptorHeap& DescriptorHeapObj, _In_ Resource& ResourceObj);
			~D3D12View();

			virtual RenderTarget& GetAsRenderTarget() override;
			inline D3D12_CPU_DESCRIPTOR_HANDLE& GetCpuDescriptor() { return _CpuDescriptor; }

		private:
			D3D12DescriptorHeap&		_DescriptorHeap;
			D3D12_CPU_DESCRIPTOR_HANDLE _CpuDescriptor;
		};
	}
}

#endif

#ifndef _D3D12_RESOURCE_HPP_
#define _D3D12_RESOURCE_HPP_

#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		class D3D12Resource
		{
		public:
			inline ID3D12DescriptorHeap*& GetDescriptorHeap() { return _DescriptorHeap; }
			inline ID3D12Resource*& GetResource() { return _Resource; }
			inline D3D12_CPU_DESCRIPTOR_HANDLE& GetCpuDescriptor() { return _CpuDescriptor; }

		private:
			ID3D12DescriptorHeap* _DescriptorHeap = nullptr;
			ID3D12Resource* _Resource = nullptr;
			D3D12_CPU_DESCRIPTOR_HANDLE _CpuDescriptor;
		};
	}
}

#endif

#ifndef _D3D12_RESOURCE_HPP_
#define _D3D12_RESOURCE_HPP_

#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		enum ResourceState
		{
			COMMON			= 0x0,
			RENDERTARGET	= 0x4,
			PRESENT			= 0x0
		};

		class D3D12CommandList;

		class D3D12Resource
		{
		public:
			virtual ~D3D12Resource();

			void Transition(_In_ D3D12CommandList& CommandList, _In_ const ResourceState& NewState);

			inline ID3D12Resource*& GetResource() { return _Resource; }
			inline D3D12_CPU_DESCRIPTOR_HANDLE& GetCpuDescriptor() { return _CpuDescriptor; }

		protected:
			void SetCpuDescriptor(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& CpuDescriptor);

		private:
			ID3D12Resource* _Resource = nullptr;
			D3D12_CPU_DESCRIPTOR_HANDLE _CpuDescriptor;
			ResourceState _OldState = COMMON;
		};
	}
}

#endif

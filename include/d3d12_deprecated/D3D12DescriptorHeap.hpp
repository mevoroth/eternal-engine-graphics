#ifndef _D3D12_DESCRIPTOR_HEAP_HPP_
#define _D3D12_DESCRIPTOR_HEAP_HPP_

#include "Graphics/DescriptorHeap.hpp"
#include "Graphics/RootSignature.hpp"
#include <d3d12.h>

struct ID3D12DescriptorHeap;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;

		struct DescriptorHeapHandle
		{
			D3D12_CPU_DESCRIPTOR_HANDLE Cpu;
			D3D12_GPU_DESCRIPTOR_HANDLE Gpu;
		};

		class D3D12DescriptorHeap : public DescriptorHeap
		{
		public:
			D3D12DescriptorHeap(_In_ Device& DeviceObj, _In_ const RootSignatureParameter Resources[], _In_ uint32_t ResourcesCount);
			//D3D12DescriptorHeap(_In_ Device& DeviceObj, _In_ uint32_t Space, _In_ uint32_t Register, _In_ const RootSignatureParameterType& HeapTypeObj, _In_ uint32_t ResourcesCount);
			virtual ~D3D12DescriptorHeap();

			virtual DescriptorTable* CreateView(_In_ Device& DeviceObj) override;

			inline ID3D12DescriptorHeap* GetD3D12DescriptorHeap() { return _DescriptorHeap; }
			
			DescriptorHeapHandle Reserve(_In_ uint32_t Slot);
			DescriptorHeapHandle Pop();
			DescriptorHeapHandle GetBase() const;
			void Push(_In_ const DescriptorHeapHandle& Handle);

			D3D12_ROOT_PARAMETER_TYPE GetD3D12RootParameterType() const { return _HeapType; }

			//D3D12_GPU_DESCRIPTOR_HANDLE		_GPUDescriptorHandleStart;
			D3D12_ROOT_PARAMETER_TYPE		_HeapType;

		private:
			DescriptorHeapHandle GetSlot(_In_ uint32_t Slot) const;

			uint64_t						_ResourcesPool			= 0ull;
			//vector<uint32_t>				_ResourcesPool;
			ID3D12DescriptorHeap*			_DescriptorHeap			= nullptr;
			D3D12_CPU_DESCRIPTOR_HANDLE		_CPUDescriptorHandleStart;
			D3D12_GPU_DESCRIPTOR_HANDLE		_GPUDescriptorHandleStart;
			uint32_t						_DescriptorHandleSize	= 0;
		};
	}
}

#endif

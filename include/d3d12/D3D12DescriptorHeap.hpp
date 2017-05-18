#ifndef _D3D12_DESCRIPTOR_HEAP_HPP_
#define _D3D12_DESCRIPTOR_HEAP_HPP_

#include "Graphics/DescriptorHeap.hpp"
#include "Graphics/RootSignature.hpp"
#include <d3d12.h>
#include <cstdint>
#include <vector>

struct ID3D12DescriptorHeap;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;

		class D3D12DescriptorHeap : public DescriptorHeap
		{
		public:
			D3D12DescriptorHeap(_In_ Device& DeviceObj, _In_ uint32_t Space, _In_ uint32_t Register, _In_ const RootSignatureDynamicParameterType& HeapTypeObj, _In_ uint32_t ResourcesCount);

			inline ID3D12DescriptorHeap* GetD3D12DescriptorHeap() { return _DescriptorHeap; }
			
			D3D12_ROOT_PARAMETER_TYPE GetD3D12RootParameterType() const { return _HeapType; }

			D3D12_CPU_DESCRIPTOR_HANDLE Reserve(_In_ uint32_t Slot);
			D3D12_CPU_DESCRIPTOR_HANDLE Pop();
			void Push(_In_ const D3D12_CPU_DESCRIPTOR_HANDLE& Handle);

		private:
			D3D12_CPU_DESCRIPTOR_HANDLE GetSlot(_In_ uint32_t Slot) const;

			vector<uint32_t>				_ResourcesPool;
			D3D12_ROOT_PARAMETER_TYPE		_HeapType;
			ID3D12DescriptorHeap*			_DescriptorHeap			= nullptr;
			D3D12_CPU_DESCRIPTOR_HANDLE		_DescriptorHandleStart;
			uint32_t						_DescriptorHandleSize	= 0;
			uint32_t						_ResourcesCount			= 0;
		};
	}
}

#endif

#ifndef _D3D12_HEAP_HPP_
#define _D3D12_HEAP_HPP_

#include "Graphics_deprecated/Heap.hpp"

enum D3D12_HEAP_TYPE;
enum D3D12_CPU_PAGE_PROPERTY;
struct ID3D12Heap;

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		class Device;

		class D3D12Heap : public Heap
		{
		public:
			D3D12Heap(_In_ Device& DeviceObj, _In_ const HeapType& HeapTypeObj, _In_ uint32_t ResourcesCount, _In_ bool InVRAM, _In_ bool VisibleFromCPU, _In_ bool Coherent, _In_ bool Cached);
			~D3D12Heap();

			virtual void Initialize(_In_ size_t Size) override;
			virtual bool IsInitialized() const override;

			ID3D12Heap* GetD3D12Heap() { return _Heap; }
			const D3D12_HEAP_TYPE& GetD3D12HeapType() const { return _HeapType; }

		private:
			ID3D12Heap*				_Heap				= nullptr;
			D3D12_HEAP_TYPE			_HeapType;
			D3D12_CPU_PAGE_PROPERTY	_CpuPageProperty;
		};
	}
}

#endif

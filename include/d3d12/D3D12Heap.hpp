#ifndef _D3D12_HEAP_HPP_
#define _D3D12_HEAP_HPP_

#include <vector>
#include "Graphics/Heap.hpp"

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
			D3D12Heap(_In_ Device& DeviceObj, _In_ size_t Size, _In_ uint32_t ResourcesCount, _In_ bool InVRAM, _In_ bool VisibleFromCPU, _In_ bool Coherent, _In_ bool Cached);
			~D3D12Heap();

			ID3D12Heap* GetD3D12Heap() { return _Heap; }

		private:
			ID3D12Heap*			_Heap = nullptr;
		};
	}
}

#endif

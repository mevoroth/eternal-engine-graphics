#ifndef _D3D12_COMMAND_ALLOCATOR_HPP_
#define _D3D12_COMMAND_ALLOCATOR_HPP_

#include <d3d12.h>
#include "Graphics/CommandAllocator.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D12Device;

		class D3D12CommandAllocator : public CommandAllocator
		{
		public:
			D3D12CommandAllocator(_In_ D3D12Device& DeviceObj, _In_ const D3D12_COMMAND_LIST_TYPE& CommandListType);

			void Reset();
			inline ID3D12CommandAllocator* GetD3D12CommandAllocator() { return _CommandAllocator; }

		private:
			ID3D12CommandAllocator* _CommandAllocator = nullptr;
		};
	}
}

#endif

#if ETERNAL_ENABLE_D3D12

#include "d3d12/D3D12CommandAllocator.hpp"

#include <d3d12.h>
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12Utils.hpp"
#include "Graphics/CommandQueue.hpp"

namespace Eternal
{
	namespace Graphics
	{
		D3D12CommandAllocator::D3D12CommandAllocator(_In_ Device& InDevice, _In_ const CommandQueue& InCommandQueue)
			: _CommandType(InCommandQueue.GetCommandType())
		{
			using namespace Eternal::Graphics::D3D12;

			VerifySuccess(
				static_cast<D3D12Device&>(InDevice).GetD3D12Device()->CreateCommandAllocator(
					ConvertCommandTypeToD3D12CommandListType(_CommandType),
					__uuidof(ID3D12CommandAllocator),
					reinterpret_cast<void**>(&_CommandAllocator)
				)
			);
		}

		D3D12CommandAllocator::~D3D12CommandAllocator()
		{
			_CommandAllocator->Release();
			_CommandAllocator = nullptr;
		}

		void D3D12CommandAllocator::Reset()
		{
			D3D12::VerifySuccess(
				_CommandAllocator->Reset()
			);
		}
	}
}

#endif

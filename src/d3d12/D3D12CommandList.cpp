#include "d3d12/D3D12CommandList.hpp"

#include <d3d12.h>
#include "d3d12/D3D12Device.hpp"
#include "d3d12/D3D12CommandAllocator.hpp"
#include "d3d12/D3D12Utils.hpp"

namespace Eternal
{
	namespace Graphics
	{
		D3D12CommandList::D3D12CommandList(_In_ Device& InDevice, _In_ CommandAllocator& InCommandAllocator)
			: CommandList(InCommandAllocator)
		{
			D3D12Device& InD3DDevice = static_cast<D3D12Device&>(InDevice);
			D3D12CommandAllocator& InD3DCommandAllocator = static_cast<D3D12CommandAllocator&>(InCommandAllocator);
			D3D12::VerifySuccess(
				InD3DDevice.GetD3D12Device()->CreateCommandList(
					InD3DDevice.GetDeviceMask(),
					D3D12_COMMAND_LIST_TYPES[static_cast<int32_t>(InD3DCommandAllocator.GetCommandType())],
					InD3DCommandAllocator.GetD3D12CommandAllocator(),
					nullptr,
					__uuidof(ID3D12GraphicsCommandList5),
					(void**)&_GraphicCommandList5
				)
			);
			End();
		}

		D3D12CommandList::~D3D12CommandList()
		{
			_GraphicCommandList5->Release();
			_GraphicCommandList5 = nullptr;
		}

		void D3D12CommandList::Begin()
		{
			_GraphicCommandList5->Reset(
				GetD3D12CommandAllocator().GetD3D12CommandAllocator(),
				nullptr
			);
		}

		void D3D12CommandList::End()
		{
			_GraphicCommandList5->Close();
		}
	}
}

#ifndef _D3D12_COMMAND_QUEUE_HPP_
#define _D3D12_COMMAND_QUEUE_HPP_

#include "Graphics_deprecated/CommandQueue.hpp"
#include "Graphics_deprecated/CommandList.hpp"
#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;
		class SwapChain;
		class CommandList;
		class CommandAllocator;
		class D3D12CommandList;
		class D3D12CommandAllocator;

		class D3D12CommandQueue : public CommandQueue
		{
		public:
			D3D12CommandQueue(_In_ Device& DeviceObj, _In_ const CommandListType& Type = COMMAND_LIST_TYPE_GRAPHIC);
			virtual ~D3D12CommandQueue();

			virtual void Submit(_In_ Context& GfxContext, _In_ CommandList* CommandLists[], _In_ uint32_t CommandListsCount) override;
			//virtual CommandAllocator* GetCommandAllocator(_In_ uint32_t FrameIndex) override;

			inline ID3D12CommandQueue* GetD3D12CommandQueue() { return _CommandQueue; }
			inline D3D12_COMMAND_LIST_TYPE GetCommandListType() const { return _CommandListType; }

		private:
			ID3D12CommandQueue*				_CommandQueue		= nullptr;
			D3D12_COMMAND_LIST_TYPE			_CommandListType	= D3D12_COMMAND_LIST_TYPE_DIRECT;
		};
	}
}

#endif

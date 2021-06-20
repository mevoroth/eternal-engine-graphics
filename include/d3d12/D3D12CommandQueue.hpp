#ifndef _D3D12_COMMAND_QUEUE_HPP_
#define _D3D12_COMMAND_QUEUE_HPP_

#include "Graphics/CommandQueue.hpp"
#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;
		class CommandList;

		class D3D12CommandQueue : public CommandQueue
		{
		public:
			D3D12CommandQueue(_In_ Device& DeviceObj, _In_ const CommandType& Type = CommandType::COMMAND_TYPE_GRAPHIC);
			virtual ~D3D12CommandQueue();

			virtual void SubmitCommandLists(_In_ CommandList* CommandLists[], _In_ uint32_t CommandListsCount) override;

		private:
			ID3D12CommandQueue*				_CommandQueue		= nullptr;
			D3D12_COMMAND_LIST_TYPE			_CommandQueueType	= D3D12_COMMAND_LIST_TYPE_DIRECT;
		};
	}
}

#endif

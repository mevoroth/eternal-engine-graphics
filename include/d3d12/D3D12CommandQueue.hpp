#pragma once

#include "Graphics/CommandQueue.hpp"
#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;
		class CommandList;

		class D3D12CommandQueue final : public CommandQueue
		{
		public:
			D3D12CommandQueue(_In_ Device& InDevice, _In_ const CommandType& InType = CommandType::COMMAND_TYPE_GRAPHICS);
			virtual ~D3D12CommandQueue() override final;

			virtual void SubmitCommandLists(_In_ CommandList* InCommandLists[], _In_ uint32_t InCommandListsCount, _In_ GraphicsContext* InContext) override final;

			inline ID3D12CommandQueue* GetD3D12CommandQueue() { return _CommandQueue; }

		private:
			ID3D12CommandQueue*				_CommandQueue		= nullptr;
			D3D12_COMMAND_LIST_TYPE			_CommandQueueType	= D3D12_COMMAND_LIST_TYPE_DIRECT;
		};
	}
}

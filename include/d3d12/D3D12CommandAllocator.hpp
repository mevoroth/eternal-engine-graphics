#pragma once

#include "Graphics/CommandAllocator.hpp"
#include "Graphics/CommandUtils.hpp"

struct ID3D12CommandAllocator;

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class CommandQueue;

		class D3D12CommandAllocator final : public CommandAllocator
		{
		public:
			D3D12CommandAllocator(_In_ Device& InDevice, _In_ const CommandQueue& InCommandQueue);
			virtual ~D3D12CommandAllocator() override final;

			virtual void Reset() override final;

			inline CommandType GetCommandType() const { return _CommandType; }
			inline ID3D12CommandAllocator* GetD3D12CommandAllocator() const { return _CommandAllocator; }

		private:
			CommandType				_CommandType		= CommandType::COMMAND_TYPE_GRAPHICS;
			ID3D12CommandAllocator*	_CommandAllocator	= nullptr;
		};
	}
}

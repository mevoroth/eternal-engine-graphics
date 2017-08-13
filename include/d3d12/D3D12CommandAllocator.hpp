#ifndef _D3D12_COMMAND_ALLOCATOR_HPP_
#define _D3D12_COMMAND_ALLOCATOR_HPP_

#include "Graphics/CommandAllocator.hpp"

struct ID3D12CommandAllocator;

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		enum CommandListType;

		class D3D12CommandAllocator : public CommandAllocator
		{
		public:
			D3D12CommandAllocator(_In_ Device& DeviceObj, _In_ const CommandListType& Type);
			virtual ~D3D12CommandAllocator();

			void Reset();
			inline ID3D12CommandAllocator* GetD3D12CommandAllocator() { return _CommandAllocator; }

		private:
			ID3D12CommandAllocator* _CommandAllocator = nullptr;
		};
	}
}

#endif

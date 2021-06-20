#pragma once

#include "Graphics/CommandList.hpp"

struct ID3D12CommandList;

namespace Eternal
{
	namespace Graphics
	{
		class D3D12CommandList : public CommandList
		{
		public:

			D3D12CommandList(_In_ Device& DeviceObj, _In_ const CommandType& Type);

			inline ID3D12CommandList* GetD3D12CommandList() { return _CommandList; }

		private:
			ID3D12CommandList* _CommandList = nullptr;
		};
	}
}

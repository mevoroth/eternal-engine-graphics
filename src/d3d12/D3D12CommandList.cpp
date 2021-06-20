#include "d3d12/D3D12CommandList.hpp"

namespace Eternal
{
	namespace Graphics
	{
		D3D12CommandList::D3D12CommandList(_In_ Device& DeviceObj, _In_ const CommandType& Type)
			: CommandList(DeviceObj, Type)
		{
		}
	}
}

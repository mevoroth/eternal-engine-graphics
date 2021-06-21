#include "d3d12/D3D12Utils.hpp"
#include "Graphics/CommandUtils.h"

#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		const D3D12_COMMAND_LIST_TYPE D3D12_COMMAND_LIST_TYPES[] =
		{
			D3D12_COMMAND_LIST_TYPE_DIRECT,
			D3D12_COMMAND_LIST_TYPE_COMPUTE,
			D3D12_COMMAND_LIST_TYPE_COPY
		};
		
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(D3D12_COMMAND_LIST_TYPES) == static_cast<int32_t>(CommandType::COMMAND_TYPE_COUNT), "Mismatch between abstraction and d3d12 command types");
	}
}

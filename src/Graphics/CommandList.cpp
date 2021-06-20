#include "Graphics/CommandList.hpp"

namespace Eternal
{
	namespace Graphics
	{
		CommandList::CommandList(_In_ Device& DeviceObj, _In_ const CommandType& Type)
			: _CommandType(Type)
		{
		}
	}
}

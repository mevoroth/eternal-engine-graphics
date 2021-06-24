#include "Graphics/Types/MemoryType.hpp"

namespace Eternal
{
	namespace Graphics
	{
		GraphicsMemoryFlag operator|(const GraphicsMemoryFlag& A, const GraphicsMemoryFlag& B)
		{
			return GraphicsMemoryFlag(int32_t(A) | int32_t(B));
		}
	}
}

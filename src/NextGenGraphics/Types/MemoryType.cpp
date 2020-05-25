#include "NextGenGraphics/Types/MemoryType.hpp"

namespace Eternal
{
	namespace Graphics
	{
		GraphicsMemoryFlag operator|(const GraphicsMemoryFlag& A, const GraphicsMemoryFlag& B)
		{
			return GraphicsMemoryFlag(int(A) | int(B));
		}
	}
}

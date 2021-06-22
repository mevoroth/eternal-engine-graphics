#pragma once

namespace Eternal
{
	namespace Graphics
	{
		enum class GraphicsMemoryFlag
		{
			MEMORY_FLAG_GPU				= 0x1,
			MEMORY_FLAG_MAPPABLE		= 0x2,
			MEMORY_FLAG_MANAGED_CACHE	= 0x4,
			MEMORY_FLAG_CPU_CACHED		= 0x8
		};

		extern GraphicsMemoryFlag operator|(const GraphicsMemoryFlag& A, const GraphicsMemoryFlag& B);
	}
}

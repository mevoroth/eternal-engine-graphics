#pragma once

namespace Eternal
{
	namespace Graphics
	{
		enum class Format
		{
			FORMAT_BGRA8888		= 0,
			FORMAT_RGBA8888		= 1,
			FORMAT_RGBA32323232	= 2,
			FORMAT_R32			= 3,
			FORMAT_RG328		= 4,
			FORMAT_RG3232		= 5,
			FORMAT_D32			= 6,
			FORMAT_COUNT,
			FORMAT_INVALID		= FORMAT_COUNT
		};
	}
}

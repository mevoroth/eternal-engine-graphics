#pragma once

namespace Eternal
{
	namespace Graphics
	{
		enum class Format
		{
			FORMAT_UNKNOWN		= 0,
			FORMAT_BGRA8888,
			FORMAT_RGBA8888,
			FORMAT_RGBA32323232,
			FORMAT_R32,
			FORMAT_RG328,
			FORMAT_RG3232,
			FORMAT_D32,
			FORMAT_COUNT,
			FORMAT_INVALID		= FORMAT_COUNT
		};
	}
}

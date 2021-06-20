#ifndef _INPUT_LAYOUT_HPP_
#define _INPUT_LAYOUT_HPP_

#include "Graphics_deprecated/Format.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class InputLayout
		{
		public:
			enum VertexDataType
			{
				POSITION_T = 0,
				COLOR_T,
				NORMAL_T,
				TANGENT_T,
				BINORMAL_T,
				UV_T,

				// Imgui Format
				IMGUI_POSITION_T,

				VERTEX_DATA_TYPE_COUNT,
				
				// Imgui Format
				IMGUI_UV_T		= UV_T,
				IMGUI_COLOR_T	= COLOR_T
			};
		};

		const Format VERTEX_FORMATS[] =
		{
			FORMAT_RGBA32323232,
			FORMAT_RGBA8888,
			FORMAT_RGBA8888,
			FORMAT_RGBA32323232,
			FORMAT_RGBA32323232,
			FORMAT_RG3232,

			// Imgui Format
			FORMAT_RG3232
		};

		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VERTEX_FORMATS) == InputLayout::VERTEX_DATA_TYPE_COUNT, "Vertex Formats implementation not complete");
	}
}

#endif

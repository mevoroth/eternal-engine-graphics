#ifndef _INPUT_LAYOUT_HPP_
#define _INPUT_LAYOUT_HPP_

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
				UV_T,

				// Imgui Format
				IMGUI_POSITION_T,

				VERTEX_DATA_TYPE_COUNT,
				
				// Imgui Format
				IMGUI_UV_T		= UV_T,
				IMGUI_COLOR_T	= COLOR_T
			};
		};
	}
}

#endif

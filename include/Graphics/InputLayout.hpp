#pragma once

namespace Eternal
{
	namespace Graphics
	{
		enum class Format;

		enum class VertexDataType
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
			IMGUI_UV_T = UV_T,
			IMGUI_COLOR_T = COLOR_T
		};

		class InputLayout
		{
		public:
		};

		const Format& ConvertVertexDataTypeToFormat(_In_ const VertexDataType& InVertexDataType);
	}
}

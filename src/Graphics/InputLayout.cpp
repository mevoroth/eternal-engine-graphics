#include "Graphics/InputLayout.hpp"

#include "Graphics/Format.hpp"

namespace Eternal
{
	namespace Graphics
	{
		static constexpr Format VERTEX_FORMATS[] =
		{
			Format::FORMAT_RGBA32323232,
			Format::FORMAT_RGBA8888,
			Format::FORMAT_RGBA8888,
			Format::FORMAT_RGBA32323232,
			Format::FORMAT_RGBA32323232,
			Format::FORMAT_RG3232,

			// Imgui Format
			Format::FORMAT_RG3232
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VERTEX_FORMATS) == static_cast<int32_t>(VertexDataType::VERTEX_DATA_TYPE_COUNT), "Vertex Formats implementation not complete");

		const Format& ConvertVertexDataTypeToFormat(_In_ const VertexDataType& InVertexDataType)
		{
			return VERTEX_FORMATS[static_cast<int32_t>(InVertexDataType)];
		}
	}
}

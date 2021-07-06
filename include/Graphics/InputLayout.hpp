#pragma once

#include "Graphics/Format.hpp"
#include <vector>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		enum class Format;

		enum class VertexDataType
		{
			VERTEX_DATA_TYPE_POSITION = 0,
			VERTEX_DATA_TYPE_COLOR,
			VERTEX_DATA_TYPE_NORMAL,
			VERTEX_DATA_TYPE_TANGENT,
			VERTEX_DATA_TYPE_BINORMAL,
			VERTEX_DATA_TYPE_UV,

			// Imgui Format
			IMGUI_POSITION_T,

			VERTEX_DATA_TYPE_COUNT,

			// Imgui Format
			IMGUI_UV_T = VERTEX_DATA_TYPE_UV,
			IMGUI_COLOR_T = VERTEX_DATA_TYPE_COLOR
		};

		enum class VertexStreamFrequency
		{
			VERTEX_STREAM_FREQUENCY_VERTEX		= 0,
			VERTEX_STREAM_FREQUENCY_INSTANCE
		};

		struct VertexStreamElement
		{
			Format				VertexFormat	= Format::FORMAT_UNKNOWN;
			VertexDataType		Type			= VertexDataType::VERTEX_DATA_TYPE_UV;
		};

		class VertexStreamBase
		{
		public:

			const vector<VertexStreamElement>& GetElements() const { return _Elements; }
			VertexStreamFrequency GetVertexStreamFrequency() const { return _InstancesCount > 1 ? VertexStreamFrequency::VERTEX_STREAM_FREQUENCY_INSTANCE : VertexStreamFrequency::VERTEX_STREAM_FREQUENCY_INSTANCE; }
			uint32_t GetStreamStride() const { return _StreamStride; }

		protected:
			VertexStreamBase(_In_ const vector<VertexStreamElement>& InElements, _In_ uint32_t InInstancesCount, _In_ uint32_t StreamStride)
				: _Elements(InElements)
				, _InstancesCount(InInstancesCount)
				, _StreamStride(StreamStride)
			{
				ETERNAL_ASSERT(InInstancesCount > 0);
			}

		private:
			vector<VertexStreamElement>	_Elements;
			uint32_t					_InstancesCount	= 0;
			uint32_t					_StreamStride	= 0;
		};

		template<typename VertexStreamType>
		class VertexStream : public VertexStreamBase
		{
		public:
			VertexStream(_In_ const vector<VertexStreamElement>& InElements, _In_ uint32_t InInstancesCount = 1)
				: VertexStreamBase(InElements, InInstancesCount, sizeof(VertexStreamType))
			{
			}
		};

		class InputLayout
		{
		public:
			InputLayout(_In_ const vector<VertexStreamBase>& InVertexStreams)
				: VertexStreams(InVertexStreams)
			{
			}

		private:
			vector<VertexStreamBase> VertexStreams;
		};

		const Format& ConvertVertexDataTypeToFormat(_In_ const VertexDataType& InVertexDataType);
	}
}

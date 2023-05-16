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
			VERTEX_DATA_TYPE_COUNT
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
			VertexStreamFrequency GetVertexStreamFrequency() const { return _VertexStreamFrequency; }
			uint32_t GetStreamStride() const { return _StreamStride; }

		protected:
			VertexStreamBase(_In_ const vector<VertexStreamElement>& InElements, _In_ const VertexStreamFrequency& InVertexStreamFrequency, _In_ uint32_t StreamStride)
				: _Elements(InElements)
				, _VertexStreamFrequency(InVertexStreamFrequency)
				, _StreamStride(StreamStride)
			{
			}

		private:
			vector<VertexStreamElement>	_Elements;
			VertexStreamFrequency		_VertexStreamFrequency = VertexStreamFrequency::VERTEX_STREAM_FREQUENCY_VERTEX;
			uint32_t					_StreamStride	= 0;
		};

		template<typename VertexStreamType>
		class VertexStream : public VertexStreamBase
		{
		public:
			VertexStream(_In_ const vector<VertexStreamElement>& InElements, _In_ const VertexStreamFrequency& InVertexStreamFrequency = VertexStreamFrequency::VERTEX_STREAM_FREQUENCY_VERTEX)
				: VertexStreamBase(InElements, InVertexStreamFrequency, sizeof(VertexStreamType))
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

#ifndef _D3D11_POS_COLOR_VERTEX_BUFFER_HPP_
#define _D3D11_POS_COLOR_VERTEX_BUFFER_HPP_

#include <cstdint>
#include <vector>
#include "Types/Types.hpp"
#include "d3d11/D3D11VertexBuffer.hpp"

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Types;

		class D3D11PosColorVertexBuffer : public D3D11VertexBuffer
		{
		public:
			class PosColorVertex
			{
			public:
				Vector4 Pos;
				uint32_t Color;
			};

			D3D11PosColorVertexBuffer(_In_ vector<D3D11PosColorVertexBuffer::PosColorVertex>& Vertex);

			virtual size_t GetSize() const override;
			virtual size_t GetVerticesCount() const override;

		private:
			static size_t _Size;

			vector<PosColorVertex>& _Vertex;
		};
	}
}

#endif

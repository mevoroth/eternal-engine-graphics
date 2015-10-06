#ifndef _D3D11_POS_UV_NORMAL_VERTEX_BUFFER_HPP_
#define _D3D11_POS_UV_NORMAL_VERTEX_BUFFER_HPP_

#include "d3d11/D3D11VertexBuffer.hpp"

#include <vector>

#include "GraphicsSettings.hpp"
#include "Types/Types.hpp"

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		using namespace Types;

		class D3D11PosUVNormalVertexBuffer : public D3D11VertexBuffer
		{
		public:
			class PosUVNormalVertex
			{
			public:
				Vector4 Pos;
				Vector4 Normal;
				Vector2 UV;
			};

			D3D11PosUVNormalVertexBuffer(vector<D3D11PosUVNormalVertexBuffer::PosUVNormalVertex>& Vertex);

			virtual size_t GetSize() const override;

		private:
			static size_t _Size;

			vector<PosUVNormalVertex>& _Vertex;
		};
	}
}

#endif

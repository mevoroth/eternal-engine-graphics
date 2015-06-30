#ifndef _D3D11_POS_UV_VERTEX_BUFFER_HPP_
#define _D3D11_POS_UV_VERTEX_BUFFER_HPP_

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

		class D3D11PosUVVertexBuffer : public D3D11VertexBuffer
		{
		public:
			class PosUVVertex
			{
			public:
				Vector4 Pos;
				Vector2 UV;
			};

			D3D11PosUVVertexBuffer(vector<D3D11PosUVVertexBuffer::PosUVVertex>& Vertex);

			virtual ID3D11ShaderResourceView* GetD3D11ShaderResourceView()
			{
				throw std::exception("The method or operation is not implemented.");
			}

			virtual size_t GetSize() const override;

		private:
			static size_t _Size;

			vector<PosUVVertex>& _Vertex;
		};
	}
}

#endif

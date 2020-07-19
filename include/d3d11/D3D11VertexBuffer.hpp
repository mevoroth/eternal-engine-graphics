#ifndef _D3D11_VERTEX_BUFFER_HPP_
#define _D3D11_VERTEX_BUFFER_HPP_

#include "Graphics/VertexBuffer.hpp"
#include "D3D11Buffer.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		template<class VertexT>
		class D3D11VertexBuffer
			: public D3D11Buffer
			, public VertexBuffer
		{
		public:
			D3D11VertexBuffer(_In_ vector<VertexT>& Vertices)
				: _Vertices(Vertices)
				, D3D11Buffer(Vertices.size() * sizeof(VertexT), DYNAMIC, WRITE, VERTEX_BUFFER, (void*)&Vertices[0])
			{
			}

			virtual size_t GetSize() const override
			{
				return sizeof(VertexT);
			}
			virtual size_t GetVerticesCount() const override
			{
				return _Vertices.size();
			}
			virtual void* GetNative() override
			{
				return GetD3D11Buffer();
			}

		private:
			vector<VertexT>& _Vertices;
		};
	}
}

#endif

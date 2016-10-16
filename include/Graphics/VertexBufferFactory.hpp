#ifndef _VERTEX_BUFFER_FACTORY_HPP_
#define _VERTEX_BUFFER_FACTORY_HPP_

#include <vector>
#include "d3d11/D3D11VertexBuffer.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		template<class VertexT>
		VertexBuffer* CreateVertexBuffer(_In_ vector<VertexT>& Vertices)
		{
			return new D3D11VertexBuffer<VertexT>(Vertices);
		}
	}
}

#endif

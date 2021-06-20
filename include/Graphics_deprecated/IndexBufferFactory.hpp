#ifndef _INDEX_BUFFER_FACTORY_HPP_
#define _INDEX_BUFFER_FACTORY_HPP_

#include "d3d11/D3D11IndexBuffer.hpp"

namespace Eternal
{
	namespace Graphics
	{
		template<typename IndexT>
		IndexBuffer* CreateIndexBuffer(_In_ vector<IndexT>& Indices)
		{
			return new D3D11IndexBuffer<IndexT>(Indices);
		}
	}
}

#endif

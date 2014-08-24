#ifndef _VERTEX_BUFFER_HPP_
#define _VERTEX_BUFFER_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class VertexBuffer
		{
		public:
			virtual size_t GetSize() const = 0;
		};
	}
}

#endif

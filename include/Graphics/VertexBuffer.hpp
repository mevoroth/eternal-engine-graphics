#ifndef _VERTEX_BUFFER_HPP_
#define _VERTEX_BUFFER_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class VertexBuffer
		{
		public:
			virtual ~VertexBuffer() {}
			virtual size_t GetSize() const = 0;
			virtual size_t GetVerticesCount() const = 0;
		};
	}
}

#endif

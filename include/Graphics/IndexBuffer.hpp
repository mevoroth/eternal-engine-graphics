#ifndef _INDEX_BUFFER_HPP_
#define _INDEX_BUFFER_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		class IndexBuffer
		{
		public:
			virtual ~IndexBuffer() {}
			virtual uint32_t GetCount() const = 0;
		};
	}
}

#endif

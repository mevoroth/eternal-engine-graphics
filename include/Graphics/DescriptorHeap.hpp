#ifndef _DESCRIPTOR_HEAP_HPP_
#define _DESCRIPTOR_HEAP_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		class DescriptorHeap
		{
		public:
			DescriptorHeap(_In_ uint32_t Space);

			virtual ~DescriptorHeap() {}

		private:
			uint32_t _Space	= 0u;
		};
	}
}

#endif

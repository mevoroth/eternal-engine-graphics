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
			DescriptorHeap(_In_ uint32_t Space, _In_ uint32_t Register);

			virtual ~DescriptorHeap() {}

			uint32_t GetSpace() const { return _Space; }
			uint32_t GetRegister() const { return _Register; }

		private:
			uint32_t _Space		= 0u;
			uint32_t _Register	= 0u;
		};
	}
}

#endif

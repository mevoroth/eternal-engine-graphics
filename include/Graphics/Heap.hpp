#ifndef _HEAP_HPP_
#define _HEAP_HPP_

#include <cstdint>
#include <vector>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Heap
		{
		public:
			Heap(_In_ size_t Size, _In_ uint32_t ResourcesCount);
			virtual ~Heap() {}

			uint64_t Pop();
			void Push(_In_ uint64_t Slot);

		private:
			vector<uint64_t>	_ResourcesPool;
			size_t				_Size			= 0ull;
			uint32_t			_ResourcesCount = 0u;
		};
	}
}

#endif

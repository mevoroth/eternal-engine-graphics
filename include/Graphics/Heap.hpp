#ifndef _HEAP_HPP_
#define _HEAP_HPP_

#include <cstdint>
#include <vector>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Device;

		class Heap
		{
		public:
			Heap(_In_ Device& DeviceObj, _In_ uint32_t ResourcesCount);
			virtual ~Heap() {}

			virtual void Initialize(_In_ size_t Size) = 0;

			uint64_t Pop();
			void Push(_In_ uint64_t Slot);

		protected:
			Device& GetDevice() { return _Device; }
			size_t GetResourcesCount() const { return _ResourcesCount; }
			void SetSize(_In_ size_t Size) { _Size = Size; }

		private:
			Device&				_Device;
			vector<uint64_t>	_ResourcesPool;
			size_t				_Size			= 0ull;
			uint32_t			_ResourcesCount = 0u;
		};
	}
}

#endif

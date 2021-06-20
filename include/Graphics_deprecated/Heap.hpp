#ifndef _HEAP_HPP_
#define _HEAP_HPP_

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		enum HeapType
		{
			HEAP_TYPE_BUFFER,
			HEAP_TYPE_TEXTURE,
			HEAP_TYPE_RENDER_TARGET_DEPTH_STENCIL
		};

		class Device;

		class Heap
		{
		public:
			Heap(_In_ Device& DeviceObj, _In_ const HeapType& HeapTypeObj, _In_ uint32_t ResourcesCount);
			virtual ~Heap() {}

			virtual void Initialize(_In_ size_t Size) = 0;
			virtual bool IsInitialized() const = 0;

			uint64_t Pop();
			void Push(_In_ uint64_t Slot);

		protected:
			Device& GetDevice() { return _Device; }
			size_t GetResourcesCount() const { return _ResourcesCount; }
			void SetSize(_In_ size_t Size) { _Size = Size; }
			const HeapType& GetHeapType() const { return _HeapType; }

		private:
			Device&				_Device;
			HeapType			_HeapType;
			vector<uint64_t>	_ResourcesPool;
			size_t				_Size			= 0ull;
			uint32_t			_ResourcesCount = 0u;
		};
	}
}

#endif

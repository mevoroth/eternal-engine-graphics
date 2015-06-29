#ifndef _RESOURCE_HPP_
#define _RESOURCE_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Resource
		{
		public:
			enum CPUAccess
			{
				READ	= 0x1,
				WRITE	= 0x2
			};
			virtual void* Lock(const CPUAccess& LockingMode) = 0;
			virtual void Unlock() = 0;
			void Resize(const size_t& Size);
			inline size_t GetSize()
			{
				return _Size;
			}
		
		private:
			size_t _Size = 0;
		};
	}
}

#endif

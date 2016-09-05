#ifndef _RESOURCE_HPP_
#define _RESOURCE_HPP_

#include <cstdint>
#include "Macros/Macros.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Context;

		class Resource
		{
		public:
			enum Usage
			{
				DEFAULT		= 0,
				IMMUTABLE	= 1,
				DYNAMIC		= 2,
				STAGING		= 3
			};
			enum CPUAccess
			{
				NONE	= 0x0,
				READ	= 0x1,
				WRITE	= 0x2
			};
			enum LockMode
			{
				LOCK_READ			= 0,
				LOCK_WRITE			= 1,
				LOCK_READ_WRITE		= 2,
				LOCK_WRITE_DISCARD	= 3
			};
			struct LockedResource
			{
				void* Data;
				uint32_t RowPitch;
				uint32_t DepthPitch;
			};
			virtual ~Resource() {}
			virtual LockedResource Lock(Context& ContextObj, const LockMode& LockingMode) = 0;
			virtual void Unlock(Context& ContextObj) = 0;
			void Resize(const size_t& Size) { ETERNAL_ASSERT(false); };
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

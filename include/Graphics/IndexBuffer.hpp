#ifndef _INDEX_BUFFER_HPP_
#define _INDEX_BUFFER_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class IndexBuffer
		{
		public:
			virtual ~IndexBuffer() {}
			virtual uint32_t GetCount() const = 0;
			virtual void* GetNative() = 0;
			virtual int GetNativeFormat() = 0;
		};
	}
}

#endif

#ifndef _STRUCTURED_BUFFER_
#define _STRUCTURED_BUFFER_

namespace Eternal
{
	namespace Graphics
	{
		class Resource;

		class StructuredBuffer
		{
		public:
			virtual ~StructuredBuffer() {}

			virtual Resource* GetAsResource() = 0;
		};
	}
}

#endif

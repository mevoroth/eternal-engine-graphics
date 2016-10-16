#ifndef _CLEARABLE_HPP_
#define _CLEARABLE_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Context;

		class Clearable
		{
		public:
			virtual void Clear(_In_ Context* ContextObj) = 0;
			virtual void Clear(_In_ Context* ContextObj, _In_ float Value) = 0;
		};
	}
}

#endif

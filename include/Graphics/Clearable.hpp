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
			virtual void Clear(Context* ContextObj) = 0;
		};
	}
}

#endif

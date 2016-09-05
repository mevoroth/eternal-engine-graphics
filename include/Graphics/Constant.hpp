#ifndef _CONSTANT_HPP_
#define _CONSTANT_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Resource;
		
		class Constant
		{
		public:
			virtual ~Constant() {}
			virtual Resource* GetAsResource() = 0;
		};
	}
}

#endif

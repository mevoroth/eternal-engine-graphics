#ifndef _RESOURCE_HPP_
#define _RESOURCE_HPP_

namespace Eternal
{
	namespace Graphics
	{
		enum ResourceType
		{
			TRANSFER	= 0,
			STRUCTURED,
			BUFFER,
			INDEX,
			VERTEX,
			INDIRECT
		};

		class Resource
		{
		public:
			Resource();
		};
	}
}

#endif

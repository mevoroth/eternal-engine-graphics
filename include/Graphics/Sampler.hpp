#ifndef _SAMPLER_HPP_
#define _SAMPLER_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Sampler
		{
		public:
			enum AddressMode
			{
				WRAP = 1,
				MIRROR = 2,
				CLAMP = 3,
				ADRESSMODE_COUNT
			};
			virtual ~Sampler() {}
		};
	}
}

#endif

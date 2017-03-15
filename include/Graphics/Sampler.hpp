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

		private:
			AddressMode _U			= WRAP;
			AddressMode _V			= WRAP;
			AddressMode _W			= WRAP;
			bool		_MINLinear	= true;
			bool		_MAGLinear	= true;
			bool		_MIPLinear	= false;
		};
	}
}

#endif

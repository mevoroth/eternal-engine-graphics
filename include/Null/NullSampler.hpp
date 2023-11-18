#pragma once

#include "Graphics/Sampler.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class NullSampler : public Sampler
		{
		public:

			NullSampler(_In_ const SamplerCreateInformation& InSamplerCreateInformation)
				: Sampler(InSamplerCreateInformation)
			{
			}

		};
	}
}

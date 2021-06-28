#pragma once

#include "Graphics/Sampler.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class GraphicsContext;

		Sampler* CreateSampler(_In_ GraphicsContext& InContext, _In_ const SamplerCreateInformation& InSamplerCreateInformation);
	}
}

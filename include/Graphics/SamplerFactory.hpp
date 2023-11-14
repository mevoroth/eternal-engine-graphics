#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class GraphicsContext;
		class Sampler;
		struct SamplerCreateInformation;

		Sampler* CreateSampler(_In_ GraphicsContext& InContext, _In_ const SamplerCreateInformation& InSamplerCreateInformation);
		void DestroySampler(_Inout_ Sampler*& InOutSampler);
	}
}

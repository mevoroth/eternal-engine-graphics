#pragma once

#include "Graphics/RootSignature.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class GraphicsContext;

		class NullRootSignature : public RootSignature
		{
		public:

			NullRootSignature()
				: RootSignature()
			{
			}
			NullRootSignature(_In_ const RootSignatureCreateInformation& InRootSignatureCreateInformation)
				: RootSignature(InRootSignatureCreateInformation)
			{
			}

		};
	}
}

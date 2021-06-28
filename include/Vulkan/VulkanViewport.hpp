#pragma once

#include "Graphics/Viewport.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class VulkanViewport : public Viewport
		{
		public:
			VulkanViewport(_In_ int32_t InX, _In_ int32_t InY, _In_ int32_t InWidth, _In_ int32_t InHeight, _In_ bool InIsHeightInverted = false)
				: Viewport(InX, InY, InWidth, InHeight)
				, _InIsHeightInverted(InIsHeightInverted)
			{
			}

			inline int32_t GetInvertedY() const
			{
				ETERNAL_ASSERT(_InIsHeightInverted);
				return GetHeight() + GetY();
			}

			inline int32_t GetInvertedHeight() const
			{
				ETERNAL_ASSERT(_InIsHeightInverted);
				return -GetHeight();
			}

		private:
			bool _InIsHeightInverted = false;
		};
	}
}

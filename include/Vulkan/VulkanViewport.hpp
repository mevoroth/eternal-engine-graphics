#pragma once

#include "Graphics/Viewport.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class VulkanViewport final : public Viewport
		{
		public:
			VulkanViewport(_In_ int32_t InX, _In_ int32_t InY, _In_ int32_t InWidth, _In_ int32_t InHeight, _In_ bool InIsHeightInverted = false)
				: Viewport(InX, InY, InWidth, InHeight)
				, _InIsHeightInverted(InIsHeightInverted)
			{
			}

			inline int32_t GetInvertedY() const
			{
				return (_InIsHeightInverted ? GetHeight() : 0) + GetY();
			}

			inline int32_t GetInvertedHeight() const
			{
				return _InIsHeightInverted ? -GetHeight() : GetHeight();
			}

		private:
			bool _InIsHeightInverted = false;
		};
	}
}

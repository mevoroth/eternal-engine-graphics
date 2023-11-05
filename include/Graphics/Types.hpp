#pragma once

#include "Graphics/Types/DeviceType.hpp"

namespace Eternal
{
	namespace Graphics
	{
		struct RenderSettings
		{
			RenderSettings(
				_In_ const DeviceType& InDriver,
				_In_ uint32_t InWidth,
				_In_ uint32_t InHeight,
				_In_ bool InIsVSync = false,
				_In_ bool InIsWindowed = true
			)
				: Driver(InDriver)
				, Width(InWidth)
				, Height(InHeight)
				, IsVSync(InIsVSync)
				, IsWindowed(InIsWindowed)
			{}

			DeviceType Driver	= DeviceType::DEVICE_TYPE_DEFAULT;
			uint32_t Width		= 1;
			uint32_t Height		= 1;
			bool IsVSync		= false;
			bool IsWindowed		= true;
		};
	}
}

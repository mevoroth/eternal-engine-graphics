#pragma once

#if ETERNAL_ENABLE_D3D12 && ETERNAL_PLATFORM_WINDOWS

#include "Windows/WindowsGraphicsContext.hpp"
#include "d3d12/D3D12GraphicsContext.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D12WindowsGraphicsContext final
			: public WindowsGraphicsContext
			, public D3D12GraphicsContext
		{
		public:

			D3D12WindowsGraphicsContext(_In_ const WindowsGraphicsContextCreateInformation& InWindowsGraphicsContextCreateInformation);

		};
	}
}

#endif

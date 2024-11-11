#if ETERNAL_ENABLE_D3D12 && ETERNAL_PLATFORM_WINDOWS

#include "d3d12/D3D12WindowsGraphicsContext.hpp"

namespace Eternal
{
	namespace Graphics
	{
		D3D12WindowsGraphicsContext::D3D12WindowsGraphicsContext(_In_ const WindowsGraphicsContextCreateInformation& InWindowsGraphicsContextCreateInformation)
			: WindowsGraphicsContext(InWindowsGraphicsContextCreateInformation)
			, D3D12GraphicsContext(InWindowsGraphicsContextCreateInformation, _WindowsOutputDevice)
		{
		}
	}
}

#endif

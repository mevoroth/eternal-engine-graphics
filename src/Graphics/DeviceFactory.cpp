#include "Graphics/DeviceFactory.hpp"

#include "Macros/Macros.hpp"
#include "d3d11/D3D11Device.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Device* CreateDevice(_In_ const DeviceType& DeviceTypeObj, _In_ WNDPROC WindowsProcess, _In_ HINSTANCE hInstance, _In_ int nCmdShow, _In_ const string& Name, _In_ const string& ClassName)
		{
			if (DeviceTypeObj == WINDOWS)
			{
				D3D11Device* D3DDevice = new D3D11Device(hInstance, nCmdShow, Name, ClassName);
				D3DDevice->Create(WindowsProcess);
				return D3DDevice;
			}

			ETERNAL_ASSERT(false);

			return nullptr;
		}
	}
}

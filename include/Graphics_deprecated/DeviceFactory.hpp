#ifndef _DEVICE_FACTORY_HPP_
#define _DEVICE_FACTORY_HPP_

#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

#include <string>

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		class Device;

		//enum DeviceType
		//{
		//	WINDOWS
		//};

		//Device* CreateDevice(_In_ const DeviceType& DeviceTypeObj, _In_ WNDPROC WindowsProcess, _In_ HINSTANCE hInstance, _In_ int nCmdShow, _In_ const string& Name, _In_ const string& ClassName);
	}
}

#endif

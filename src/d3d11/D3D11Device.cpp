#include "d3d11/D3D11Device.hpp"

using namespace Eternal::Graphics;

D3D11Device::D3D11Device(_In_ HINSTANCE hInstance, _In_ int nCmdShow, _In_ const string& Name, _In_ const string& ClassName)
	: _WindowName(Name)
	, _ClassName(ClassName)
	, _hInstance(hInstance)
	, _nCmdShow(nCmdShow)
{
}

void D3D11Device::Create(WNDPROC WindowEventsHandler)
{
	WNDCLASSEX WindowClass;
	
	ZeroMemory(&WindowClass, sizeof(WNDCLASSEX));

	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = WindowEventsHandler;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = _hInstance;
	WindowClass.hIcon = 0;
	WindowClass.hCursor = 0;
	WindowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	WindowClass.lpszMenuName = 0;
	WindowClass.lpszClassName = _GetClassName();

	RegisterClassEx(&WindowClass);

	_WindowHandle = CreateWindowEx(
		WS_EX_APPWINDOW,
		_GetClassName(),
		_GetWindowName(),
		WS_OVERLAPPED,
		0,
		0,
		Device::WIDTH,
		Device::HEIGHT,
		0,
		0,
		_hInstance,
		0
	);

	if (!_WindowHandle)
	{
		DWORD err = GetLastError();
		// LOG
	}
	
	ShowWindow(_WindowHandle, _nCmdShow);
}

inline LPCTSTR D3D11Device::_GetClassName() const
{
	return _ClassName.c_str();
}

inline LPCTSTR D3D11Device::_GetWindowName() const
{
	return _WindowName.c_str();
}

HWND D3D11Device::GetWindow() const
{
	return _WindowHandle;
}

#include "d3d10/D3D10Device.hpp"

using namespace Eternal::Graphics;

D3D10Device::D3D10Device(_In_ HINSTANCE hInstance, _In_ int nCmdShow, _In_ const string& name, _In_ const string& className)
	: _windowName(name)
	, _className(className)
	, _hInstance(hInstance)
	, _nCmdShow(nCmdShow)
{
}

void D3D10Device::Create()
{
	WNDCLASSEX windowClass;

	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = D3D10Device::WindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = _hInstance;
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = _GetClassName();

	RegisterClassEx(&windowClass);

	_windowHandle = CreateWindowEx(
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

	if (!_windowHandle)
	{
		DWORD err = GetLastError();
		// LOG
	}

	ShowWindow(_windowHandle, _nCmdShow);
}

inline LPCTSTR D3D10Device::_GetClassName() const
{
	return _className.c_str();
}

inline LPCTSTR D3D10Device::_GetWindowName() const
{
	return _windowName.c_str();
}

LRESULT D3D10Device::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

HWND D3D10Device::GetWindow() const
{
	return _windowHandle;
}

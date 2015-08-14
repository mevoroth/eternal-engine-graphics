#include "d3d11/D3D11Device.hpp"

using namespace Eternal::Graphics;

D3D11Device::D3D11Device(_In_ HINSTANCE hInstance, _In_ int nCmdShow, _In_ const string& Name, _In_ const string& ClassName)
	: _windowName(Name)
	, _ClassName(ClassName)
	, _hInstance(hInstance)
	, _nCmdShow(nCmdShow)
{
}

void D3D11Device::Create()
{
	WNDCLASSEX WindowClass;
	
	ZeroMemory(&WindowClass, sizeof(WNDCLASSEX));

	WindowClass.cbSize = sizeof(WNDCLASSEX);
	WindowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc = D3D11Device::WindowProc;
	WindowClass.cbClsExtra = 0;
	WindowClass.cbWndExtra = 0;
	WindowClass.hInstance = _hInstance;
	WindowClass.hIcon = 0;
	WindowClass.hCursor = 0;
	WindowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	WindowClass.lpszMenuName = 0;
	WindowClass.lpszClassName = _GetClassName();

	RegisterClassEx(&WindowClass);

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

inline LPCTSTR D3D11Device::_GetClassName() const
{
	return _ClassName.c_str();
}

inline LPCTSTR D3D11Device::_GetWindowName() const
{
	return _windowName.c_str();
}

LRESULT D3D11Device::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
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

HWND D3D11Device::GetWindow() const
{
	return _windowHandle;
}

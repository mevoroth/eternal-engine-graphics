#include "d3d11/D3D11Device.hpp"

using namespace Eternal::Graphics;

D3D11Device::D3D11Device(_In_ HINSTANCE hInstance, _In_ int nCmdShow, _In_ const string& name, _In_ const string& className)
	: _windowName(name)
	, _className(className)
	, _hInstance(hInstance)
	, _nCmdShow(nCmdShow)
{
}

void D3D11Device::Create()
{
	WNDCLASSEX windowClass;
	
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = D3D11Device::WindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = _hInstance;
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = _getClassName();

	RegisterClassEx(&windowClass);

	_windowHandle = CreateWindowEx(
		WS_EX_APPWINDOW,
		_getClassName(),
		_getWindowName(),
		WS_OVERLAPPED,
		0,
		0,
		D3D11DeviceType::WIDTH,
		D3D11DeviceType::HEIGHT,
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

inline LPCTSTR D3D11Device::_getClassName() const
{
	return _className.c_str();
}

inline LPCTSTR D3D11Device::_getWindowName() const
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

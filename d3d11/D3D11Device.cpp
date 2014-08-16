#include "D3D11Device.hpp"

using namespace Eternal::Graphics;

D3D11Device::D3D11Device(HINSTANCE hInstance, int nCmdShow, const string& name, const string& className)
	: _windowName(name)
	, _className(className)
	, _hInstance(hInstance)
	, _nCmdShow(nCmdShow)
{
	create();
}

void D3D11Device::create()
{
	WNDCLASSEX windowClass;
	
	ZeroMemory(&windowClass, sizeof(WNDCLASSEX));

	windowClass.cbSize = sizeof(WNDCLASSEX);
	windowClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	//windowClass.lpfnWndProc = 
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

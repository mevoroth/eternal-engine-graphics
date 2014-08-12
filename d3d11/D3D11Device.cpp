#include "D3D11Device.hpp"

using namespace Eternal::Graphics;

D3D11Device::D3D11Device(const string& name)
	: _windowName(name)
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
	//windowClass.hInstance = 
	windowClass.hIcon = 0;
	windowClass.hCursor = 0;
	windowClass.hbrBackground = (HBRUSH)COLOR_WINDOW;
	windowClass.lpszMenuName = 0;
	windowClass.lpszClassName = _getWindowName();
}

inline LPCTSTR D3D11Device::_getWindowName() const
{
	return _windowName.c_str();
}

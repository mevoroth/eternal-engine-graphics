#include "Window/Window.hpp"

using namespace Eternal::Graphics;

Window::Window(_In_ HINSTANCE hInstance, _In_ int nCmdShow, _In_ const string& Name, _In_ const string& ClassName, _In_ int Width, _In_ int Height)
	: _WindowName(Name)
	, _ClassName(ClassName)
	, _hInstance(hInstance)
	, _nCmdShow(nCmdShow)
	, _Width(Width)
	, _Height(Height)
{
}

void Window::Create(WNDPROC WindowEventsHandler)
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
	WindowClass.lpszClassName = _ClassName.c_str();

	RegisterClassEx(&WindowClass);

	RECT WindowRect = { 0, 0, _Width, _Height };
	BOOL Adjust = AdjustWindowRect(&WindowRect, WS_OVERLAPPEDWINDOW, FALSE);
	ETERNAL_ASSERT(Adjust);

	_WindowHandle = CreateWindowEx(
		WS_EX_APPWINDOW,
		_ClassName.c_str(),
		_WindowName.c_str(),
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		WindowRect.right - WindowRect.left,
		WindowRect.bottom - WindowRect.top,
		0,
		0,
		_hInstance,
		0
	);

	if (!_WindowHandle)
	{
		DWORD err = GetLastError();
		ETERNAL_BREAK();
		// LOG
	}

	ShowWindow(_WindowHandle, _nCmdShow);
}

HWND Window::GetWindowHandler() const
{
	return _WindowHandle;
}

HINSTANCE Window::GetHInstance() const
{
	return _hInstance;
}

#include "Window/Window.hpp"

using namespace Eternal::Graphics;

Window::Window(_In_ const WindowCreateInformation& InWindowCreateInformation)
	: _WindowCreateInformation(InWindowCreateInformation)
{
}

void Window::Create(WNDPROC WindowEventsHandler)
{
	WNDCLASSEX WindowClass;

	ZeroMemory(&WindowClass, sizeof(WNDCLASSEX));

	WindowClass.cbSize			= sizeof(WNDCLASSEX);
	WindowClass.style			= CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	WindowClass.lpfnWndProc		= WindowEventsHandler;
	WindowClass.cbClsExtra		= 0;
	WindowClass.cbWndExtra		= 0;
	WindowClass.hInstance		= GetHInstance();
	WindowClass.hIcon			= 0;
	WindowClass.hCursor			= 0;
	WindowClass.hbrBackground	= (HBRUSH)COLOR_WINDOW;
	WindowClass.lpszMenuName	= 0;
	WindowClass.lpszClassName	= GetClassName().c_str();

	RegisterClassEx(&WindowClass);

	RECT WindowRect = { 0, 0, GetWidth(), GetHeight() };
	BOOL Adjust = AdjustWindowRect(&WindowRect, WS_OVERLAPPEDWINDOW, FALSE);
	ETERNAL_ASSERT(Adjust);

	_WindowHandle = CreateWindowEx(
		WS_EX_APPWINDOW,
		GetClassName().c_str(),
		GetWindowName().c_str(),
		WS_OVERLAPPEDWINDOW,
		0,
		0,
		WindowRect.right - WindowRect.left,
		WindowRect.bottom - WindowRect.top,
		0,
		0,
		GetHInstance(),
		0
	);

	if (!_WindowHandle)
	{
		DWORD err = GetLastError();
		ETERNAL_BREAK();
		// LOG
	}

	ShowWindow(_WindowHandle, GetNCmdShow());
}

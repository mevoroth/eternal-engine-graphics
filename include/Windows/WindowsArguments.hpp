#pragma once

#if ETERNAL_PLATFORM_WINDOWS

#include <Windows.h>

namespace Eternal
{
	namespace Graphics
	{
		struct WindowsArguments
		{
			WindowsArguments(
				_In_ HINSTANCE hInInstance,
				_In_ HINSTANCE hInPrevInstance,
				_In_ LPSTR lpInCmdLine,
				_In_ int nInCmdShow,
				_In_ const char* InName,
				_In_ const char* InClassName,
				_In_ WNDPROC InWindowEventsHandler
			)
				: hInstance(hInInstance)
				, hPrevInstance(hInPrevInstance)
				, lpCmdLine(lpInCmdLine)
				, nCmdShow(nInCmdShow)
				, Name(InName)
				, ClassName(InClassName)
				, WindowEventsHandler(InWindowEventsHandler)
			{
			}

			HINSTANCE hInstance;
			HINSTANCE hPrevInstance;
			LPSTR lpCmdLine;
			int nCmdShow;
			const char* Name;
			const char* ClassName;
			WNDPROC WindowEventsHandler;
		};
	}
}

#endif

#pragma once

#define WIN32_LEAN_AND_MEAN
#define VC_EXTRALEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>

#include "Graphics/Types/DeviceType.hpp"

namespace Eternal
{
	namespace Graphics
	{
		struct RenderSettings
		{
			RenderSettings(uint32_t Width, uint32_t Height, const DeviceType& Driver)
				: Width(Width)
				, Height(Height)
				, Driver(Driver)
			{}

			DeviceType Driver = DeviceType::VULKAN;
			uint32_t Width	= 1;
			uint32_t Height	= 1;
		};

		struct WindowsArguments
		{
			WindowsArguments(
				HINSTANCE hInstance,
				HINSTANCE hPrevInstance,
				LPSTR lpCmdLine,
				int nCmdShow,
				const char* Name,
				const char* ClassName,
				WNDPROC WindowEventsHandler
			)
				: hInstance(hInstance)
				, hPrevInstance(hPrevInstance)
				, lpCmdLine(lpCmdLine)
				, nCmdShow(nCmdShow)
				, Name(Name)
				, ClassName(ClassName)
				, WindowEventsHandler(WindowEventsHandler)
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

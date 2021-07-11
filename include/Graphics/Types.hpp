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
			RenderSettings(
				_In_ const DeviceType& InDriver,
				_In_ uint32_t InWidth,
				_In_ uint32_t InHeight,
				_In_ bool InIsVSync = false,
				_In_ bool InIsWindowed = true
			)
				: Driver(InDriver)
				, Width(InWidth)
				, Height(InHeight)
				, IsVSync(InIsVSync)
				, IsWindowed(InIsWindowed)
			{}

			DeviceType Driver	= DeviceType::VULKAN;
			uint32_t Width		= 1;
			uint32_t Height		= 1;
			bool IsVSync		= false;
			bool IsWindowed		= true;
		};

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

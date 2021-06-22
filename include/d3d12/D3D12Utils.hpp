#pragma once

#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		extern const D3D12_COMMAND_LIST_TYPE D3D12_COMMAND_LIST_TYPES[];

		namespace D3D12
		{
			void VerifySuccess(const HRESULT& HResult);
		}
	}
}

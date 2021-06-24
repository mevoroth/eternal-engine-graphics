#pragma once

#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		extern const D3D12_COMMAND_LIST_TYPE					D3D12_COMMAND_LIST_TYPES[];
		extern const D3D12_LOGIC_OP								D3D12_LOGIC_OPS[];
		extern const D3D12_RENDER_PASS_BEGINNING_ACCESS_TYPE	D3D12_LOAD_OPS[];
		extern const D3D12_RENDER_PASS_ENDING_ACCESS_TYPE		D3D12_STORE_OPS[];
		const D3D12_COLOR_WRITE_ENABLE D3D12_COLOR_WRITE_ENABLE_NONE = D3D12_COLOR_WRITE_ENABLE(0);

		namespace D3D12
		{
			void VerifySuccess(const HRESULT& HResult);
		}
	}
}

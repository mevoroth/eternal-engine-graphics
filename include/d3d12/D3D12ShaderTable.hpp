#pragma once

#if ETERNAL_ENABLE_D3D12

#include "Graphics/ShaderTable.hpp"
#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		class Device;

		class D3D12ShaderTable : public ShaderTable
		{
		public:
			D3D12ShaderTable(_In_ Device& InDevice, _In_ Pipeline& InPipeline);

			D3D12_GPU_VIRTUAL_ADDRESS_RANGE				GetD3D12RayGenerationShaderRecord() const;
			D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE	GetD3D12MissShaderTable() const;
			D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE	GetD3D12HitGroupTable() const;
			D3D12_GPU_VIRTUAL_ADDRESS_RANGE_AND_STRIDE	GetD3D12CallableShaderTable() const;

		private:
			D3D12_GPU_VIRTUAL_ADDRESS					_GetD3D12GPUVirtualAddress() const;
		};
	}
}

#endif

#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class ShaderTable;
		class Device;
		class Pipeline;

		ShaderTable* CreateShaderTable(_In_ Device& InDevice, _In_ Pipeline& InPipeline);
	}
}

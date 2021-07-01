#pragma once

namespace Eternal
{
	namespace Graphics
	{
		struct ConstantBufferViewCreateInformation;
		struct ShaderResourceViewCreateInformation;
		class View;

		View* CreateConstantBufferView(_In_ const ConstantBufferViewCreateInformation& InConstantBufferViewCreateInformation);
		View* CreateShaderResourceView(_In_ const ShaderResourceViewCreateInformation& InShaderResourceViewCreateInformation);
	}
}

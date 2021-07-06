#pragma once

#include "Graphics/MultiBuffered.hpp"
#include "Graphics/Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		struct ConstantBufferViewCreateInformation;
		struct ShaderResourceViewCreateInformation;
		class View;

		View* CreateConstantBufferView(_In_ const ConstantBufferViewCreateInformation& InConstantBufferViewCreateInformation);
		View* CreateShaderResourceView(_In_ const ShaderResourceViewCreateInformation& InShaderResourceViewCreateInformation);
		MultiBuffered<View> CreateMultiBufferedConstantBufferView(_In_ MultiBuffered<Resource>& InResources, _In_ const ConstantBufferViewCreateInformation& InConstantBufferViewCreateInformation);
	}
}

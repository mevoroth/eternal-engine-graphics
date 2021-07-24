#pragma once

#include "Graphics/MultiBuffered.hpp"
#include "Graphics/Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		struct ConstantBufferViewCreateInformation;
		struct ShaderResourceViewCreateInformation;
		struct RenderTargetViewCreateInformation;
		struct UnorderedAccessViewCreateInformation;
		struct DepthStencilViewCreateInformation;
		class View;

		View* CreateConstantBufferView(_In_ const ConstantBufferViewCreateInformation& InConstantBufferViewCreateInformation);
		View* CreateShaderResourceView(_In_ const ShaderResourceViewCreateInformation& InShaderResourceViewCreateInformation);
		View* CreateRenderTargetView(_In_ const RenderTargetViewCreateInformation& InRenderTargetViewCreateInformation);
		View* CreateUnorderedAccessView(_In_ const UnorderedAccessViewCreateInformation& InUnorderedAccessViewCreateInformation);
		View* CreateDepthStencilView(_In_ const DepthStencilViewCreateInformation& InDepthStencilViewCreateInformation);
		MultiBuffered<View>* CreateMultiBufferedConstantBufferView(_In_ MultiBuffered<Resource>& InResources, _In_ const ConstantBufferViewCreateInformation& InConstantBufferViewCreateInformation);
		void DestroyView(_Inout_ View*& InOutView);
		void DestroyMultiBufferedView(_Inout_ MultiBuffered<View>*& InOutMultiBufferedView);
	}
}

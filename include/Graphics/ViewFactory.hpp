#pragma once

#include "Graphics/MultiBuffered.hpp"
#include "Graphics/Resource.hpp"
#include "Graphics/View.hpp"

namespace Eternal
{
	namespace Graphics
	{
		struct ConstantBufferViewCreateInformation;
		struct ShaderResourceViewCreateInformation;
		struct RenderTargetViewCreateInformation;
		struct UnorderedAccessViewCreateInformation;
		struct DepthStencilViewCreateInformation;
		class GraphicsContext;

		View* CreateConstantBufferView(_In_ const ConstantBufferViewCreateInformation& InConstantBufferViewCreateInformation, _In_ void* InViewPlacementMemory = nullptr);
		View* CreateShaderResourceView(_In_ const ShaderResourceViewCreateInformation& InShaderResourceViewCreateInformation);
		View* CreateRenderTargetView(_In_ const RenderTargetViewCreateInformation& InRenderTargetViewCreateInformation);
		View* CreateUnorderedAccessView(_In_ const UnorderedAccessViewCreateInformation& InUnorderedAccessViewCreateInformation);
		View* CreateDepthStencilView(_In_ const DepthStencilViewCreateInformation& InDepthStencilViewCreateInformation);
		MultiBuffered<View>* CreateMultiBufferedConstantBufferView(_In_ MultiBuffered<Resource>& InResources, _In_ const ConstantBufferViewCreateInformation& InConstantBufferViewCreateInformation);
		MultiBuffered<View>* CreateMultiBufferedShaderResourceView(_In_ MultiBuffered<Resource>& InResources, _In_ const ShaderResourceViewCreateInformation& InShaderResourceViewCreateInformation);
		void DestroyView(_Inout_ View*& InOutView);
		void DestroyMultiBufferedView(_Inout_ MultiBuffered<View>*& InOutMultiBufferedView);
		size_t GetViewSize(_In_ GraphicsContext& InContext);
	}
}

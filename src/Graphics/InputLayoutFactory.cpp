#include "Graphics/InputLayoutFactory.hpp"

#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "d3d12/D3D12InputLayout.hpp"
#include "Vulkan/VulkanInputLayout.hpp"
#if ETERNAL_USE_PRIVATE
#include "Graphics/InputLayoutFactoryPrivate.hpp"
#endif

namespace Eternal
{
	namespace Graphics
	{
		InputLayout* CreateInputLayout(_In_ GraphicsContext& InContext, _In_ const vector<VertexStreamBase>& InVertexStreams)
		{
			switch (InContext.GetDevice().GetDeviceType())
			{
#if ETERNAL_ENABLE_D3D12
			case DeviceType::DEVICE_TYPE_D3D12:
				return new D3D12InputLayout(InVertexStreams);
#endif
#if ETERNAL_ENABLE_VULKAN
			case DeviceType::DEVICE_TYPE_VULKAN:
				return new VulkanInputLayout(InVertexStreams);
#endif
			default:
#if ETERNAL_USE_PRIVATE
				return CreateInputLayoutPrivate(InContext, InVertexStreams);
#endif
			}
			ETERNAL_BREAK();
			return nullptr;
		}

		void DestroyInputLayout(_Inout_ InputLayout*& InOutInputLayout)
		{
			delete InOutInputLayout;
			InOutInputLayout = nullptr;
		}
	}
}

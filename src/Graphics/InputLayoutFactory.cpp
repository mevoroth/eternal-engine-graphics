#include "Graphics/InputLayoutFactory.hpp"

#include "Graphics/GraphicsContext.hpp"
#include "Graphics/Types/DeviceType.hpp"
#include "Graphics/Device.hpp"
#include "d3d12/D3D12InputLayout.hpp"
#include "Vulkan/VulkanInputLayout.hpp"

namespace Eternal
{
	namespace Graphics
	{
		InputLayout* CreateInputLayout(_In_ GraphicsContext& InContext, _In_ const vector<VertexStreamBase>& InVertexStreams)
		{
			switch (InContext.GetDevice().GetDeviceType())
			{
#ifdef ETERNAL_ENABLE_D3D12
			case DeviceType::D3D12:
				return new D3D12InputLayout(InVertexStreams);
#endif

			case DeviceType::VULKAN:
				return new VulkanInputLayout(InVertexStreams);

			default:
				ETERNAL_BREAK();
				return nullptr;
			}
		}

		void DestroyInputLayout(_Inout_ InputLayout*& InOutInputLayout)
		{
			delete InOutInputLayout;
			InOutInputLayout = nullptr;
		}
	}
}

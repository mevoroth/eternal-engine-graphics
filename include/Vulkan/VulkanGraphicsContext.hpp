#pragma once

#include "Graphics/GraphicsContext.hpp"
#include "Vulkan/VulkanHeader.hpp"
#include "Bit/BitField.hpp"
#include <array>
#include <vector>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		using namespace Eternal::Bit;

		class VulkanGraphicsContext : public GraphicsContext
		{
		public:
			VulkanGraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation);
			~VulkanGraphicsContext();

			virtual void UpdateGraphicsContext() override;

			void AllocateConstantHandles(_In_ uint32_t ConstantCount, _Out_ vector<Handle>& OutHandles);
			void ReleaseConstantHandles(_Inout_ vector<Handle>& InOutHandles);

			vk::Semaphore& GetCurrentFrameSemaphore() { return _AcquireFrameSemaphores[GetCurrentFrameIndex()]; }

		private:

			std::array<vk::Semaphore, FrameBufferingCount>	_AcquireFrameSemaphores;
			DynamicBitField<>								_ConstantHandles;
		};
	}
}

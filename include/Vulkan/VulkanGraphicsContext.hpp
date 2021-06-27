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

			void AllocateConstantHandles(_In_ uint32_t ConstantCount, _Out_ vector<Handle>& OutHandles);
			void ReleaseConstantHandles(_Inout_ vector<Handle>& InOutHandles);

			vk::Semaphore& GetCurrentFrameSemaphore() { return _AcquireFrameSemaphores[GetCurrentFrameIndex()]; }
			vk::Semaphore& GetNextFrameSemaphore() { return _AcquireFrameSemaphores[(GetCurrentFrameIndex() + 1) % _AcquireFrameSemaphores.size()]; }

		private:

			std::array<vk::Semaphore, FrameBufferingCount>	_AcquireFrameSemaphores;
			DynamicBitField<>								_ConstantHandles;
		};
	}
}

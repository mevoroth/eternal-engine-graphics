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

		class VulkanDescriptorTable;

		class VulkanGraphicsContext final : public GraphicsContext
		{
		public:

			static constexpr uint32_t MaxDescriptorSetsCount				= 32768 * FrameBufferingCount;

			static constexpr uint32_t MaxSamplersDescriptorCount			= MaxDescriptorSetsCount * 2;
			static constexpr uint32_t MaxSampledImageDescriptorCount		= MaxDescriptorSetsCount * 8;
			static constexpr uint32_t MaxStorageImageDescriptorCount		= MaxDescriptorSetsCount / 4;
			static constexpr uint32_t MaxUniformTexelBufferDescriptorCount	= MaxDescriptorSetsCount;
			static constexpr uint32_t MaxStorageTexelBufferDescriptorCount	= MaxDescriptorSetsCount / 4;
			static constexpr uint32_t MaxUniformBufferDescriptorCount		= MaxDescriptorSetsCount * 2;
			static constexpr uint32_t MaxStorageBufferDescriptorCount		= MaxDescriptorSetsCount / 8;
			
			static constexpr uint32_t MaxShaderResourcesCountPerShader		= 16;
			static constexpr uint32_t MaxConstantBuffersCountPerShader		= 16;
			static constexpr uint32_t MaxSamplersCountPerShader				= 16;
			static constexpr uint32_t MaxUnorderedAccessesCountPerShader	= 16;

			static constexpr uint32_t ShaderRegisterShaderResourcesOffset	= 0;
			static constexpr uint32_t ShaderRegisterConstantBuffersOffset	= ShaderRegisterShaderResourcesOffset + MaxShaderResourcesCountPerShader;
			static constexpr uint32_t ShaderRegisterUnorderedAccessesOffset	= ShaderRegisterConstantBuffersOffset + MaxConstantBuffersCountPerShader;
			static constexpr uint32_t ShaderRegisterSamplersOffset			= ShaderRegisterUnorderedAccessesOffset + MaxUnorderedAccessesCountPerShader;

			VulkanGraphicsContext(_In_ const GraphicsContextCreateInformation& CreateInformation);
			~VulkanGraphicsContext();

			virtual void ResetFrameStates() override final;

			void AllocateConstantHandles(_In_ uint32_t ConstantCount, _Out_ vector<Handle>& OutHandles);
			void ReleaseConstantHandles(_Inout_ vector<Handle>& InOutHandles);
			inline void RegisterVulkanDescriptorTable(_In_ VulkanDescriptorTable* InDescriptorTable) { _DescriptorTables.push_back(InDescriptorTable); }
			inline void UnregisterVulkanDescriptorTable(_In_ VulkanDescriptorTable* InDescriptorTable) { std::remove(_DescriptorTables.begin(), _DescriptorTables.end(), InDescriptorTable); }

			const vk::DescriptorPool& GetVulkanDescriptorPool() const { return _DescriptorPool; }
			vk::Semaphore& GetCurrentFrameSemaphore() { return _AcquireFrameSemaphores[GetCurrentFrameIndex()]; }
			vk::Semaphore& GetNextFrameSemaphore() { return _AcquireFrameSemaphores[(GetCurrentFrameIndex() + 1) % _AcquireFrameSemaphores.size()]; }

		private:

			std::array<vk::Semaphore, FrameBufferingCount>	_AcquireFrameSemaphores;
			DynamicHandlePool<>								_ConstantHandles;
			vk::DescriptorPool								_DescriptorPool;
			std::vector<VulkanDescriptorTable*>				_DescriptorTables;
		};
	}
}

#pragma once

#include "Bit/BitField.hpp"
#include <vulkan/vulkan.hpp>
#include <array>
#include <vector>

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Bit;

		class GraphicsContext;
		class VulkanDescriptorTable;

		class VulkanGraphicsContext
		{
		public:
			
			static constexpr uint32_t MaxDescriptorSetsCount				= 32768 * GraphicsContext::FrameBufferingCount;

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
			
			VulkanGraphicsContext(_In_ VulkanGraphicsContext& InContext) = delete;
			VulkanGraphicsContext(_In_ GraphicsContext& InContext);
			virtual ~VulkanGraphicsContext();

			Device& GetDevice();
			void ResetFrameStates();

			void AllocateConstantHandles(_In_ uint32_t ConstantCount, _Out_ vector<Handle>& OutHandles);
			void ReleaseConstantHandles(_Inout_ vector<Handle>& InOutHandles);
			inline void RegisterVulkanDescriptorTable(_In_ VulkanDescriptorTable* InDescriptorTable) { _DescriptorTables.push_back(InDescriptorTable); }
			inline void UnregisterVulkanDescriptorTable(_In_ VulkanDescriptorTable* InDescriptorTable)
			{
				std::vector<VulkanDescriptorTable*>::iterator DescriptorTableIterator = std::find(_DescriptorTables.begin(), _DescriptorTables.end(), InDescriptorTable);
				_DescriptorTables.erase(DescriptorTableIterator);
			}

			const vk::DescriptorPool& GetVulkanDescriptorPool() const { return _DescriptorPool; }
			vk::Semaphore& GetCurrentFrameSemaphore() { return _AcquireFrameSemaphores[_GraphicsContext.GetCurrentFrameIndex()]; }
			vk::Semaphore& GetNextFrameSemaphore() { return _AcquireFrameSemaphores[(_GraphicsContext.GetCurrentFrameIndex() + 1) % _AcquireFrameSemaphores.size()]; }

		private:

			GraphicsContext&												_GraphicsContext;

			std::array<vk::Semaphore, GraphicsContext::FrameBufferingCount>	_AcquireFrameSemaphores;
			DynamicHandlePool<>												_ConstantHandles;
			vk::DescriptorPool												_DescriptorPool;
			std::vector<VulkanDescriptorTable*>								_DescriptorTables;
		};
	}
}

#pragma once

#include "Graphics/DescriptorTable.hpp"

#if ETERNAL_ENABLE_VULKAN

#include "Graphics/RootSignature.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Vulkan/VulkanHeader.hpp"
#include <array>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class GraphicsContext;
		class VulkanGraphicsContext;
		class VulkanRootSignature;

		class VulkanDescriptorTable : public DescriptorTable
		{
		public:
			static constexpr uint32_t MaxWriteDescriptorSets = 256;

			VulkanDescriptorTable(_In_ GraphicsContext& InContext, _In_ const RootSignature* InRootSignature);
			VulkanDescriptorTable(_In_ GraphicsContext& InContext, _In_ const RootSignatureDescriptorTable& InRootSignatureDescriptorTable, _In_ const RootSignature& InRootSignature, _In_ uint32_t InSubDescriptorTableIndex);
			~VulkanDescriptorTable();

			void Reset(_In_ uint32_t FrameIndex);
			void Commit(_In_ GraphicsContext& InContext, _In_ const vector<RootSignatureParameter>& InRootSignatureParameters);
			void Commit(_In_ GraphicsContext& InContext, _In_ const RootSignatureDescriptorTable& InRootSignatureDescriptorTable);

			const vk::DescriptorSet& GetVulkanDescriptorSet(_In_ uint32_t FrameIndex) const { return _UsedDescriptorSets[FrameIndex].back(); }

		private:

			const vk::DescriptorSet& AllocateVulkanDescriptorSet(_In_ uint32_t FrameIndex);
			void _AllocateVulkanDescriptorSet(_Out_ vk::DescriptorSet& OutDescriptorSet);

			VulkanGraphicsContext&													_Context;
			const vk::DescriptorSetLayout&											_DescriptorSetLayout;
			array<vector<vk::DescriptorSet>, GraphicsContext::FrameBufferingCount>	_DescriptorSets;
			array<vector<vk::DescriptorSet>, GraphicsContext::FrameBufferingCount>	_UsedDescriptorSets;
		};
	}
}

#endif

#pragma once

#include "Graphics/DescriptorTable.hpp"
#include "Graphics/RootSignature.hpp"
#include "Graphics/GraphicsContext.hpp"
#include "Vulkan/VulkanHeader.hpp"
#include <array>

namespace Eternal
{
	namespace Graphics
	{
		class GraphicsContext;
		class Device;
		class VulkanRootSignature;

		class VulkanDescriptorTable : public DescriptorTable
		{
		public:
			static constexpr uint32_t MaxWriteDescriptorSets = 256;

			VulkanDescriptorTable(_In_ GraphicsContext& InContext, _In_ const RootSignature* InRootSignature);
			VulkanDescriptorTable(_In_ GraphicsContext& InContext, _In_ const RootSignatureDescriptorTable& InRootSignatureDescriptorTable, _In_ const RootSignature& InRootSignature, _In_ uint32_t InSubDescriptorTableIndex);

			void Commit(_In_ GraphicsContext& InContext, _In_ const vector<RootSignatureParameter>& InRootSignatureParameters) const;
			void Commit(_In_ GraphicsContext& InContext, _In_ const RootSignatureDescriptorTable& InRootSignatureDescriptorTable) const;

			inline const vk::DescriptorSet& GetVulkanDescriptorSet(_In_ uint32_t FrameIndex) const { return _DescriptorSets[FrameIndex]; }

		private:
			std::array<vk::DescriptorSet, GraphicsContext::FrameBufferingCount> _DescriptorSets;
		};
	}
}

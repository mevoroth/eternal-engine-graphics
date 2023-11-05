#pragma once

#if ETERNAL_ENABLE_VULKAN

#include "Graphics/RootSignature.hpp"
#include "Graphics/ShaderType.hpp"
#include "Vulkan/VulkanHeader.hpp"
#include "Bit/BitField.hpp"
#include <vector>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		using namespace Eternal::Bit;

		class GraphicsContext;
		class Device;

		static constexpr uint32_t ByteTo32Bits = 4;

		class VulkanRootSignature final : public RootSignature
		{
		public:

			VulkanRootSignature(_In_ GraphicsContext& InContext);
			VulkanRootSignature(_In_ GraphicsContext& InContext, _In_ const RootSignatureCreateInformation& InRootSignatureCreateInformation);
			virtual ~VulkanRootSignature() override final;

			inline const vk::PipelineLayout& GetVulkanPipelineLayout() const { return _PipelineLayout; }
			inline const vector<vk::PushConstantRange>& GetVulkanPushConstantRanges() const { return _PushConstantRanges; }
			inline const vector<vk::DescriptorSetLayout>& GetVulkanDescriptorSetLayouts() const { return _DescriptorSetLayouts; }

		private:
			void _CreateDescriptorSetLayout(_In_ const RootSignatureDescriptorTable& InDescriptorTable);

			GraphicsContext&				_Context;
			vk::PipelineLayout				_PipelineLayout;
			vector<vk::DescriptorSetLayout>	_DescriptorSetLayouts;
			vector<vk::PushConstantRange>	_PushConstantRanges;
			vector<Handle>					_ConstantHandles;
		};
	}
}

#endif

#ifndef _VULKAN_PIPELINE_HPP_
#define _VULKAN_PIPELINE_HPP_

struct VkDescriptorSetLayout_T;
struct VkPipelineLayout_T;
struct VkRenderPass_T;

namespace Eternal
{
	namespace Graphics
	{
		class VulkanDevice;

		class VulkanPipeline
		{
		public:
			VulkanPipeline(_In_ VulkanDevice& Device);

			VkPipelineLayout_T* GetPipelineLayout();
			VkRenderPass_T* GetRenderPass();

		private:
			VkDescriptorSetLayout_T* _DescriptorSetLayout = nullptr;
			VkPipelineLayout_T* _PipelineLayout = nullptr;
			VkRenderPass_T* _RenderPass = nullptr;
		};
	}
}

#endif

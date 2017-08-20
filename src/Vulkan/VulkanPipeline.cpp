#include "Vulkan/VulkanPipeline.hpp"

#include <vector>
#include <vulkan/vulkan.h>

#include "Macros/Macros.hpp"
#include "Graphics/Viewport.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanRootSignature.hpp"
#include "Vulkan/VulkanRenderPass.hpp"
#include "Vulkan/VulkanShader.hpp"
#include "Vulkan/VulkanInputLayout.hpp"

using namespace Eternal::Graphics;

VulkanPipeline::VulkanPipeline(
	_In_ Device& DeviceObj,
	_In_ RootSignature& RootSignatureObj,
	_In_ InputLayout& InputLayoutObj,
	_In_ RenderPass& RenderPassObj,
	_In_ Shader& VS,
	_In_ Shader& PS,
	_In_ const DepthTest& DepthTestObj,
	_In_ const StencilTest& StencilTestObj,
	_In_ const Viewport& ViewportObj
)
{
	VkDevice& VkDeviceObj = static_cast<VulkanDevice&>(DeviceObj).GetVulkanDevice();
	VkPipelineCacheCreateInfo PipelineCacheInfo;
	PipelineCacheInfo.sType				= VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
	PipelineCacheInfo.pNext				= nullptr;
	PipelineCacheInfo.flags				= 0;
	PipelineCacheInfo.pInitialData		= nullptr;
	PipelineCacheInfo.initialDataSize	= 0;

	VkPipelineCache _PipelineCache;
	VkResult Result = vkCreatePipelineCache(VkDeviceObj, &PipelineCacheInfo, nullptr, &_PipelineCache);
	ETERNAL_ASSERT(!Result);
	
	std::vector<VkPipelineShaderStageCreateInfo> ShaderStages;
	ShaderStages.resize(2);
	
	ShaderStages[0].sType	= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	ShaderStages[0].stage	= VK_SHADER_STAGE_VERTEX_BIT;
	ShaderStages[0].module	= static_cast<VulkanShader&>(VS).GetVulkanShader();
	ShaderStages[0].pName	= "VS";

	ShaderStages[1].sType	= VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	ShaderStages[1].stage	= VK_SHADER_STAGE_FRAGMENT_BIT;
	ShaderStages[1].module	= static_cast<VulkanShader&>(PS).GetVulkanShader();
	ShaderStages[1].pName	= "PS";

	const vector<VkVertexInputAttributeDescription>& VertexInputAttributeDescriptions	= static_cast<VulkanInputLayout&>(InputLayoutObj).GetVertexInputAttributeDescriptions();
	const vector<VkVertexInputBindingDescription>& VertexInputBindingDescriptions		= static_cast<VulkanInputLayout&>(InputLayoutObj).GetVertexInputBindingDescriptions();

	VkPipelineVertexInputStateCreateInfo InputLayoutStateInfo;
	InputLayoutStateInfo.sType								= VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	InputLayoutStateInfo.pNext								= nullptr;
	InputLayoutStateInfo.flags								= 0;
	InputLayoutStateInfo.vertexAttributeDescriptionCount	= VertexInputAttributeDescriptions.size();
	InputLayoutStateInfo.pVertexAttributeDescriptions		= VertexInputAttributeDescriptions.data();
	InputLayoutStateInfo.vertexBindingDescriptionCount		= VertexInputBindingDescriptions.size();
	InputLayoutStateInfo.pVertexBindingDescriptions			= VertexInputBindingDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo InputAssemblyStateInfo;
	InputAssemblyStateInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	InputAssemblyStateInfo.pNext					= nullptr;
	InputAssemblyStateInfo.flags					= 0;
	InputAssemblyStateInfo.topology					= VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	InputAssemblyStateInfo.primitiveRestartEnable	= VK_FALSE;

	VkPipelineRasterizationStateCreateInfo RasterizationStateInfo;
	RasterizationStateInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	RasterizationStateInfo.pNext					= nullptr;
	RasterizationStateInfo.flags					= 0;
	RasterizationStateInfo.depthClampEnable			= VK_FALSE;
	RasterizationStateInfo.rasterizerDiscardEnable	= VK_FALSE;
	RasterizationStateInfo.polygonMode				= VK_POLYGON_MODE_FILL;
	RasterizationStateInfo.cullMode					= VK_CULL_MODE_BACK_BIT;
	RasterizationStateInfo.frontFace				= VK_FRONT_FACE_COUNTER_CLOCKWISE;
	RasterizationStateInfo.depthBiasEnable			= VK_FALSE;
	RasterizationStateInfo.depthBiasConstantFactor	= 0.0f;
	RasterizationStateInfo.depthBiasClamp			= 0.0f;
	RasterizationStateInfo.depthBiasSlopeFactor		= 0.0f;
	RasterizationStateInfo.lineWidth				= 1.0f;

	VkViewport VulkanViewport;
	VulkanViewport.x		= (float)ViewportObj.GetX();
	VulkanViewport.y		= (float)ViewportObj.GetY();
	VulkanViewport.width	= (float)ViewportObj.GetWidth();
	VulkanViewport.height	= (float)ViewportObj.GetHeight();
	VulkanViewport.minDepth = 0.0f;
	VulkanViewport.maxDepth = 1.0f;

	VkRect2D VulkanScissor;
	VulkanScissor.offset.x		= ViewportObj.GetX();
	VulkanScissor.offset.y		= ViewportObj.GetY();
	VulkanScissor.extent.width	= ViewportObj.GetWidth();
	VulkanScissor.extent.height = ViewportObj.GetHeight();

	VkPipelineViewportStateCreateInfo ViewportStateInfo;
	ViewportStateInfo.sType			= VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	ViewportStateInfo.pNext			= nullptr;
	ViewportStateInfo.flags			= 0;
	ViewportStateInfo.viewportCount	= 1;
	ViewportStateInfo.pViewports	= &VulkanViewport;
	ViewportStateInfo.scissorCount	= 1;
	ViewportStateInfo.pScissors		= &VulkanScissor;

	VkPipelineMultisampleStateCreateInfo MultiSampleStateInfo;
	MultiSampleStateInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	MultiSampleStateInfo.pNext					= nullptr;
	MultiSampleStateInfo.flags					= 0;
	MultiSampleStateInfo.rasterizationSamples	= VK_SAMPLE_COUNT_1_BIT;
	MultiSampleStateInfo.sampleShadingEnable	= VK_FALSE;
	MultiSampleStateInfo.minSampleShading		= 0.0f;
	MultiSampleStateInfo.pSampleMask			= nullptr;
	MultiSampleStateInfo.alphaToCoverageEnable	= VK_FALSE;
	MultiSampleStateInfo.alphaToOneEnable		= VK_FALSE;

	VkPipelineDepthStencilStateCreateInfo DepthStencilStateInfo;
	DepthStencilStateInfo.sType					= VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	DepthStencilStateInfo.pNext					= nullptr;
	DepthStencilStateInfo.flags					= 0;
	DepthStencilStateInfo.depthTestEnable		= VK_FALSE;
	DepthStencilStateInfo.depthWriteEnable		= VK_FALSE;
	DepthStencilStateInfo.depthCompareOp		= VK_COMPARE_OP_ALWAYS;
	DepthStencilStateInfo.depthBoundsTestEnable	= VK_FALSE;
	DepthStencilStateInfo.stencilTestEnable		= VK_FALSE;
	DepthStencilStateInfo.front.failOp			= VK_STENCIL_OP_KEEP;
	DepthStencilStateInfo.front.passOp			= VK_STENCIL_OP_KEEP;
	DepthStencilStateInfo.front.depthFailOp		= VK_STENCIL_OP_KEEP;
	DepthStencilStateInfo.front.compareOp		= VK_COMPARE_OP_ALWAYS;
	DepthStencilStateInfo.front.compareMask		= 0x0;
	DepthStencilStateInfo.front.writeMask		= 0x0;
	DepthStencilStateInfo.front.reference		= 0x0;
	DepthStencilStateInfo.back.failOp			= VK_STENCIL_OP_KEEP;
	DepthStencilStateInfo.back.passOp			= VK_STENCIL_OP_KEEP;
	DepthStencilStateInfo.back.depthFailOp		= VK_STENCIL_OP_KEEP;
	DepthStencilStateInfo.back.compareOp		= VK_COMPARE_OP_ALWAYS;
	DepthStencilStateInfo.back.compareMask		= 0x0;
	DepthStencilStateInfo.back.writeMask		= 0x0;
	DepthStencilStateInfo.back.reference		= 0x0;
	DepthStencilStateInfo.minDepthBounds		= 0.0f;
	DepthStencilStateInfo.maxDepthBounds		= 1.0f;

	VkPipelineColorBlendAttachmentState ColorBlendAttachmentState;
	ColorBlendAttachmentState.blendEnable			= VK_TRUE;
	ColorBlendAttachmentState.srcColorBlendFactor	= VK_BLEND_FACTOR_ONE;
	ColorBlendAttachmentState.dstColorBlendFactor	= VK_BLEND_FACTOR_ZERO;
	ColorBlendAttachmentState.colorBlendOp			= VK_BLEND_OP_ADD;
	ColorBlendAttachmentState.srcAlphaBlendFactor	= VK_BLEND_FACTOR_ONE;
	ColorBlendAttachmentState.dstAlphaBlendFactor	= VK_BLEND_FACTOR_ZERO;
	ColorBlendAttachmentState.alphaBlendOp			= VK_BLEND_OP_ADD;
	ColorBlendAttachmentState.colorWriteMask		= VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

	VkPipelineColorBlendStateCreateInfo ColorBlendStateInfo;
	ColorBlendStateInfo.sType			= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	ColorBlendStateInfo.pNext			= nullptr;
	ColorBlendStateInfo.flags			= 0;
	ColorBlendStateInfo.logicOpEnable	= VK_FALSE;
	ColorBlendStateInfo.logicOp			= VK_LOGIC_OP_CLEAR;
	ColorBlendStateInfo.pAttachments	= &ColorBlendAttachmentState;
	ColorBlendStateInfo.attachmentCount = 1;

	VkGraphicsPipelineCreateInfo PipelineInfo;
	PipelineInfo.sType					= VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	PipelineInfo.pNext					= nullptr;
	PipelineInfo.flags					= 0;
	PipelineInfo.stageCount				= ShaderStages.size();
	PipelineInfo.pStages				= ShaderStages.data();
	PipelineInfo.pVertexInputState		= &InputLayoutStateInfo;
	PipelineInfo.pInputAssemblyState	= &InputAssemblyStateInfo;
	PipelineInfo.pTessellationState		= nullptr;
	PipelineInfo.pViewportState			= &ViewportStateInfo;
	PipelineInfo.pRasterizationState	= &RasterizationStateInfo;
	PipelineInfo.pMultisampleState		= &MultiSampleStateInfo;
	PipelineInfo.pDepthStencilState		= &DepthStencilStateInfo;
	PipelineInfo.pColorBlendState		= &ColorBlendStateInfo;
	PipelineInfo.pDynamicState			= nullptr;
	PipelineInfo.layout					= static_cast<VulkanRootSignature&>(RootSignatureObj).GetPipelineLayout();
	PipelineInfo.renderPass				= static_cast<VulkanRenderPass&>(RenderPassObj).GetVulkanRenderPass();
	PipelineInfo.subpass				= 0;
	PipelineInfo.basePipelineHandle		= nullptr;
	PipelineInfo.basePipelineIndex		= 0;

	Result = vkCreateGraphicsPipelines(VkDeviceObj, _PipelineCache, 1, &PipelineInfo, nullptr, &_Pipeline);
	ETERNAL_ASSERT(!Result);
}

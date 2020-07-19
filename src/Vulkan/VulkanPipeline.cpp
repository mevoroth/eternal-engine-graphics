#include "Vulkan/VulkanPipeline.hpp"

#include "Graphics/Viewport.hpp"
#include "Graphics/DepthStencil.hpp"
#include "Graphics/BlendState.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanRootSignature.hpp"
#include "Vulkan/VulkanRenderPass.hpp"
#include "Vulkan/VulkanShader.hpp"
#include "Vulkan/VulkanInputLayout.hpp"

using namespace Eternal::Graphics;

VulkanPipeline::VulkanPipeline(Device& InDevice, const PipelineCreateInformation& InCreateInformation)
	: Pipeline(InCreateInformation)
{
	using namespace Vulkan;

	vk::Device& Device = static_cast<VulkanDevice&>(InDevice).GetVulkanDevice();
	
	vk::PipelineCacheCreateInfo PipelineCacheInfo(
		vk::PipelineCacheCreateFlagBits(),
		0, nullptr
	);
	vk::PipelineCache _PipelineCache;
	VerifySuccess(Device.createPipelineCache(&PipelineCacheInfo, nullptr, &_PipelineCache));

	vk::PipelineShaderStageCreateInfo ShaderStages[] =
	{
		vk::PipelineShaderStageCreateInfo(
			vk::PipelineShaderStageCreateFlagBits(),
			vk::ShaderStageFlagBits::eVertex,
			static_cast<VulkanShader&>(InCreateInformation.VS).GetVulkanShader(),
			"VS"
		),
		vk::PipelineShaderStageCreateInfo(
			vk::PipelineShaderStageCreateFlagBits(),
			vk::ShaderStageFlagBits::eFragment,
			static_cast<VulkanShader&>(InCreateInformation.PS).GetVulkanShader(),
			"PS"
		)
	};

	VulkanInputLayout& InputLayout = static_cast<VulkanInputLayout&>(InCreateInformation.InputLayout);
	const vector<vk::VertexInputAttributeDescription>& VertexInputAttributeDescriptions	= static_cast<VulkanInputLayout&>(InputLayout).GetVertexInputAttributeDescriptions();
	const vector<vk::VertexInputBindingDescription>& VertexInputBindingDescriptions		= static_cast<VulkanInputLayout&>(InputLayout).GetVertexInputBindingDescriptions();

	vk::PipelineVertexInputStateCreateInfo InputLayoutStateInfo(
		vk::PipelineVertexInputStateCreateFlagBits(),
		uint32_t(VertexInputBindingDescriptions.size()),
		VertexInputBindingDescriptions.data(),
		uint32_t(VertexInputAttributeDescriptions.size()),
		VertexInputAttributeDescriptions.data()
	);

	vk::PipelineInputAssemblyStateCreateInfo InputAssemblyStateInfo(
		vk::PipelineInputAssemblyStateCreateFlagBits(),
		vk::PrimitiveTopology::eTriangleList
	);

	vk::PipelineTessellationStateCreateInfo TessellationStateInfo;

	vk::PipelineRasterizationStateCreateInfo RasterizationStateInfo = vk::PipelineRasterizationStateCreateInfo(
		vk::PipelineRasterizationStateCreateFlagBits(),
		/*depthClampEnable_=*/ false,
		/*rasterizerDiscardEnable_=*/false,
		vk::PolygonMode::eFill,
		vk::CullModeFlagBits::eBack
	).setLineWidth(1.0f);

	const Viewport& Viewport = InCreateInformation.RenderPass.GetViewport();

	vk::Viewport ViewportInfo(
		float(Viewport.GetX()),
		float(Viewport.GetY()),
		float(Viewport.GetWidth()),
		float(Viewport.GetHeight()),
		0.0f, 1.0f
	);

	vk::Rect2D Scissor = ConvertViewportToRect2D(Viewport);

	vk::PipelineViewportStateCreateInfo ViewportStateInfo(
		vk::PipelineViewportStateCreateFlagBits(),
		1, &ViewportInfo,
		1, &Scissor
	);

	vk::PipelineMultisampleStateCreateInfo MultiSampleStateInfo;

	const DepthTest& DepthTest		= InCreateInformation.DepthStencil.GetDepthTest();
	const StencilTest& StencilTest	= InCreateInformation.DepthStencil.GetStencilTest();

	vk::PipelineDepthStencilStateCreateInfo DepthStencilStateInfo(
		vk::PipelineDepthStencilStateCreateFlags(),
		DepthTest.IsEnabled(),
		DepthTest.IsWriteEnabled(),
		vk::CompareOp(DepthTest.GetComparisonOperator()),
		/*depthBoundsTestEnable_=*/false,
		StencilTest.IsEnabled(),
		Vulkan::CreateStencilOpState(StencilTest, StencilTest.GetFront()),
		Vulkan::CreateStencilOpState(StencilTest, StencilTest.GetBack()),
		0.0f, 1.0f
	);

	std::vector<vk::PipelineColorBlendAttachmentState> ColorBlendAttachmentStates;
	ColorBlendAttachmentStates.resize(InCreateInformation.RenderPass.GetRenderTargets().size());
	
	for (int TargetIndex = 0; TargetIndex < InCreateInformation.RenderPass.GetRenderTargets().size(); ++TargetIndex)
	{
		const BlendState& CurrentBlendState = InCreateInformation.RenderPass.GetBlendStates()[TargetIndex];

		ColorBlendAttachmentStates[TargetIndex] = vk::PipelineColorBlendAttachmentState(
			CurrentBlendState.IsEnabled(),
			vk::BlendFactor(CurrentBlendState.GetSrc()),
			vk::BlendFactor(CurrentBlendState.GetDest()),
			vk::BlendOp(CurrentBlendState.GetBlendOp()),
			vk::BlendFactor(CurrentBlendState.GetSrcAlpha()),
			vk::BlendFactor(CurrentBlendState.GetDestAlpha()),
			vk::BlendOp(CurrentBlendState.GetBlendAlphaOp()),
			vk::ColorComponentFlagBits(CurrentBlendState.GetBlendChannel())
		);
	}

	vk::PipelineColorBlendStateCreateInfo ColorBlendStateInfo(
		vk::PipelineColorBlendStateCreateFlagBits(),
		InCreateInformation.RenderPass.GetLogicBlend().IsEnabled(),
		Vulkan::ConvertLogicOpToVulkanLogicOp(InCreateInformation.RenderPass.GetLogicBlend().GetLogicOp()),
		uint32_t(ColorBlendAttachmentStates.size()),
		ColorBlendAttachmentStates.data()
	);

	vk::PipelineDynamicStateCreateInfo DynamicStates;

	vk::GraphicsPipelineCreateInfo PipelineInfo(
		vk::PipelineCreateFlagBits(),
		ETERNAL_ARRAYSIZE(ShaderStages),
		ShaderStages,
		&InputLayoutStateInfo,
		&InputAssemblyStateInfo,
		&TessellationStateInfo,
		&ViewportStateInfo,
		&RasterizationStateInfo,
		&MultiSampleStateInfo,
		&DepthStencilStateInfo,
		&ColorBlendStateInfo,
		&DynamicStates,
		static_cast<VulkanRootSignature&>(InCreateInformation.RootSignature).GetPipelineLayout(),
		static_cast<VulkanRenderPass&>(InCreateInformation.RenderPass).GetVulkanRenderPass(),
		0,
		nullptr, 0
	);

	VerifySuccess(Device.createGraphicsPipelines(
		_PipelineCache,
		1, &PipelineInfo,
		nullptr,
		&_Pipeline
	));
}

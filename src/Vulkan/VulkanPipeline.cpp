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

VulkanPipeline::VulkanPipeline(
	_In_ Device& InDevice,
	_In_ const PipelineCreateInformation& InPipelineCreateInformation
)
	: Pipeline(InPipelineCreateInformation)
{
	using namespace Eternal::Graphics::Vulkan;

	vk::Device& Device = static_cast<VulkanDevice&>(InDevice).GetVulkanDevice();
	
	//vk::PipelineCacheCreateInfo PipelineCacheInfo(
	//	vk::PipelineCacheCreateFlagBits(),
	//	0, nullptr
	//);
	//vk::PipelineCache _PipelineCache;
	//VerifySuccess(Device.createPipelineCache(&PipelineCacheInfo, nullptr, &_PipelineCache));

	vk::PipelineShaderStageCreateInfo ShaderStages[] =
	{
		vk::PipelineShaderStageCreateInfo(
			vk::PipelineShaderStageCreateFlagBits(),
			vk::ShaderStageFlagBits::eVertex,
			static_cast<VulkanShader&>(InPipelineCreateInformation.VS).GetVulkanShader(),
			"VS"
		),
		vk::PipelineShaderStageCreateInfo(
			vk::PipelineShaderStageCreateFlagBits(),
			vk::ShaderStageFlagBits::eFragment,
			static_cast<VulkanShader&>(InPipelineCreateInformation.PS).GetVulkanShader(),
			"PS"
		)
	};

	VulkanInputLayout& InputLayout = static_cast<VulkanInputLayout&>(InPipelineCreateInformation.PipelineInputLayout);
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

	const Viewport& Viewport = InPipelineCreateInformation.PipelineRenderPass.GetViewport();

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

	const DepthTest& DepthTest		= InPipelineCreateInformation.PipelineDepthStencil.GetDepthTest();
	const StencilTest& StencilTest	= InPipelineCreateInformation.PipelineDepthStencil.GetStencilTest();

	vk::PipelineDepthStencilStateCreateInfo DepthStencilStateInfo(
		vk::PipelineDepthStencilStateCreateFlags(),
		DepthTest.IsEnabled(),
		DepthTest.IsWriteEnabled(),
		ConvertComparisonFunctionToVulkanComparisonOperator(DepthTest.GetComparisonFunction()),
		/*depthBoundsTestEnable_=*/false,
		StencilTest.IsEnabled(),
		CreateVulkanStencilOperatorState(StencilTest, StencilTest.GetFront()),
		CreateVulkanStencilOperatorState(StencilTest, StencilTest.GetBack()),
		0.0f, 1.0f
	);

	std::vector<vk::PipelineColorBlendAttachmentState> ColorBlendAttachmentStates;
	ColorBlendAttachmentStates.resize(InPipelineCreateInformation.PipelineRenderPass.GetRenderTargets().size());
	
	for (int TargetIndex = 0; TargetIndex < InPipelineCreateInformation.PipelineRenderPass.GetRenderTargets().size(); ++TargetIndex)
	{
		const BlendState& CurrentBlendState = InPipelineCreateInformation.PipelineRenderPass.GetRenderTargets()[TargetIndex].RenderTargetBlendState;

		ColorBlendAttachmentStates[TargetIndex] = CreateVulkanPipelineColorBlendStateAttachmentState(
			InPipelineCreateInformation.PipelineRenderPass.GetRenderTargets()[TargetIndex].RenderTargetBlendState
		);
	}

	vk::PipelineColorBlendStateCreateInfo ColorBlendStateInfo(
		vk::PipelineColorBlendStateCreateFlagBits(),
		InPipelineCreateInformation.PipelineRenderPass.GetLogicBlend().IsEnabled(),
		ConvertLogicOperatorToVulkanLogicOperator(InPipelineCreateInformation.PipelineRenderPass.GetLogicBlend().GetLogicOperator()),
		static_cast<uint32_t>(ColorBlendAttachmentStates.size()),
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
		static_cast<VulkanRootSignature&>(InPipelineCreateInformation.PipelineRootSignature).GetPipelineLayout(),
		static_cast<VulkanRenderPass&>(InPipelineCreateInformation.PipelineRenderPass).GetVulkanRenderPass(),
		0,
		nullptr, 0
	);

	VerifySuccess(Device.createGraphicsPipelines(
		nullptr,
		1, &PipelineInfo,
		nullptr,
		&_Pipeline
	));
}

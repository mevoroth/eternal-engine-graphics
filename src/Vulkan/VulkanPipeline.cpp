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
#include "Vulkan/VulkanViewport.hpp"

namespace Eternal
{
	namespace Graphics
	{
		VulkanPipeline::VulkanPipeline(
			_In_ Device& InDevice,
			_In_ const GraphicsPipelineCreateInformation& InPipelineCreateInformation
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
					static_cast<VulkanShader*>(InPipelineCreateInformation.VS)->GetVulkanShader(),
					"VS"
				),
				vk::PipelineShaderStageCreateInfo(
					vk::PipelineShaderStageCreateFlagBits(),
					vk::ShaderStageFlagBits::eFragment,
					static_cast<VulkanShader*>(InPipelineCreateInformation.PS)->GetVulkanShader(),
					"PS"
				)
			};

			VulkanInputLayout* InputLayout = static_cast<VulkanInputLayout*>(InPipelineCreateInformation.PipelineInputLayout);
			const vector<vk::VertexInputAttributeDescription>& VertexInputAttributeDescriptions	= InputLayout->GetVertexInputAttributeDescriptions();
			const vector<vk::VertexInputBindingDescription>& VertexInputBindingDescriptions		= InputLayout->GetVertexInputBindingDescriptions();

			vk::PipelineVertexInputStateCreateInfo InputLayoutStateInfo(
				vk::PipelineVertexInputStateCreateFlagBits(),
				uint32_t(VertexInputBindingDescriptions.size()),
				VertexInputBindingDescriptions.size() > 0 ? VertexInputBindingDescriptions.data() : nullptr,
				uint32_t(VertexInputAttributeDescriptions.size()),
				VertexInputAttributeDescriptions.size() > 0 ? VertexInputAttributeDescriptions.data() : nullptr
			);

			vk::PipelineInputAssemblyStateCreateInfo InputAssemblyStateInfo(
				vk::PipelineInputAssemblyStateCreateFlagBits(),
				ConvertPrimitiveTopologyToVulkanPrimitiveTopology(InPipelineCreateInformation.PipelinePrimitiveTopology)
			);

			vk::PipelineTessellationStateCreateInfo TessellationStateInfo;

			const Rasterizer& InRasterizer = InPipelineCreateInformation.PipelineRasterizer;
			const bool DepthBiasEnabled = InRasterizer.GetDepthBias() != 0 || InRasterizer.GetDepthBiasClamp() != 0.0f || InRasterizer.GetDepthBiasSlopeScale() != 0.0f;

			vk::PipelineRasterizationStateCreateInfo RasterizationStateInfo = vk::PipelineRasterizationStateCreateInfo(
				vk::PipelineRasterizationStateCreateFlagBits(),
				/*depthClampEnable_=*/ !InRasterizer.GetDepthClip(),
				/*rasterizerDiscardEnable_=*/false,
				ConvertFillModeToVulkanPolygonMode(InRasterizer.GetFillMode()),
				ConvertCullModeToVulkanCullModeFlags(InRasterizer.GetCullMode()),
				ConvertFrontFaceToVulkanFrontFace(InRasterizer.GetFrontFace()),
				DepthBiasEnabled,
				static_cast<float>(InRasterizer.GetDepthBias()),
				InRasterizer.GetDepthBiasClamp(),
				InRasterizer.GetDepthBiasSlopeScale(),
				1.0f
			);

			const VulkanViewport& Viewport = static_cast<const VulkanViewport&>(InPipelineCreateInformation.PipelineRenderPass->GetViewport());

			vk::Viewport ViewportInfo(
				static_cast<float>(Viewport.GetX()),
				static_cast<float>(Viewport.GetInvertedY()),
				static_cast<float>(Viewport.GetWidth()),
				static_cast<float>(Viewport.GetInvertedHeight()),
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
			ColorBlendAttachmentStates.resize(InPipelineCreateInformation.PipelineRenderPass->GetRenderTargets().size());
	
			for (int TargetIndex = 0; TargetIndex < InPipelineCreateInformation.PipelineRenderPass->GetRenderTargets().size(); ++TargetIndex)
			{
				const BlendState& CurrentBlendState = InPipelineCreateInformation.PipelineRenderPass->GetRenderTargets()[TargetIndex].RenderTargetBlendState;

				ColorBlendAttachmentStates[TargetIndex] = CreateVulkanPipelineColorBlendStateAttachmentState(
					InPipelineCreateInformation.PipelineRenderPass->GetRenderTargets()[TargetIndex].RenderTargetBlendState
				);
			}

			vk::PipelineColorBlendStateCreateInfo ColorBlendStateInfo(
				vk::PipelineColorBlendStateCreateFlagBits(),
				InPipelineCreateInformation.PipelineRenderPass->GetLogicBlend().IsEnabled(),
				ConvertLogicOperatorToVulkanLogicOperator(InPipelineCreateInformation.PipelineRenderPass->GetLogicBlend().GetLogicOperator()),
				static_cast<uint32_t>(ColorBlendAttachmentStates.size()),
				ColorBlendAttachmentStates.data()
			);

			vk::DynamicState DynamicStates[] =
			{
				vk::DynamicState::eViewport,
				vk::DynamicState::eScissor,
				vk::DynamicState::eLineWidth,
				vk::DynamicState::eDepthBias,
				vk::DynamicState::eBlendConstants,
				vk::DynamicState::eDepthBounds,
				vk::DynamicState::eStencilCompareMask,
				vk::DynamicState::eStencilWriteMask,
				vk::DynamicState::eStencilReference
			};

			vk::PipelineDynamicStateCreateInfo DynamicStateInfos(
				vk::PipelineDynamicStateCreateFlagBits(),
				ETERNAL_ARRAYSIZE(DynamicStates),
				DynamicStates
			);

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
				&DynamicStateInfos,
				static_cast<VulkanRootSignature&>(InPipelineCreateInformation.PipelineRootSignature).GetVulkanPipelineLayout(),
				static_cast<VulkanRenderPass*>(InPipelineCreateInformation.PipelineRenderPass)->GetVulkanRenderPass(),
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

		VulkanPipeline::VulkanPipeline(
			_In_ Device& InDevice,
			_In_ const ComputePipelineCreateInformation& InPipelineCreateInformation
		)
			: Pipeline(InPipelineCreateInformation)
		{
			using namespace Eternal::Graphics::Vulkan;

			vk::Device& Device = static_cast<VulkanDevice&>(InDevice).GetVulkanDevice();

			vk::PipelineShaderStageCreateInfo ShaderStageInfo(
				vk::PipelineShaderStageCreateFlagBits(),
				vk::ShaderStageFlagBits::eCompute,
				static_cast<VulkanShader*>(InPipelineCreateInformation.CS)->GetVulkanShader(),
				"CS"
			);

			vk::ComputePipelineCreateInfo PipelineInfo(
				vk::PipelineCreateFlagBits(),
				ShaderStageInfo,
				static_cast<VulkanRootSignature&>(InPipelineCreateInformation.PipelineRootSignature).GetVulkanPipelineLayout()
			);

			VerifySuccess(Device.createComputePipelines(
				nullptr,
				1, &PipelineInfo,
				nullptr,
				&_Pipeline
			));
		}

		VulkanPipeline::VulkanPipeline(
			_In_ Device& InDevice,
			_In_ const MeshPipelineCreateInformation& InPipelineCreateInformation
		)
			: Pipeline(InPipelineCreateInformation)
		{
			ETERNAL_BREAK();
		}
	}
}

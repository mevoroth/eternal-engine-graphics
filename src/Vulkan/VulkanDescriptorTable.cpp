#include "Vulkan/VulkanDescriptorTable.hpp"

#include "Vulkan/VulkanGraphicsContext.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanRootSignature.hpp"
#include "Vulkan/VulkanView.hpp"
#include "Vulkan/VulkanSampler.hpp"
#include "Vulkan/VulkanResource.hpp"
#include "Vulkan/VulkanUtils.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Graphics::Vulkan;

		VulkanDescriptorTable::VulkanDescriptorTable(_In_ GraphicsContext& InContext, _In_ const RootSignature* InRootSignature)
			: DescriptorTable(InRootSignature)
		{
			const vector<vk::DescriptorSetLayout>& DescriptorSetLayouts = static_cast<const VulkanRootSignature&>(*InRootSignature).GetVulkanDescriptorSetLayouts();
			ETERNAL_ASSERT(DescriptorSetLayouts.size() > 0);

			const vk::DescriptorPool& Pool = static_cast<VulkanGraphicsContext&>(InContext).GetVulkanDescriptorPool();
			vk::DescriptorSetLayout UsedDescriptorSetLayouts[] = { DescriptorSetLayouts[0], DescriptorSetLayouts[0] };

			vk::DescriptorSetAllocateInfo DescriptorSetsInfo(
				Pool,
				2,
				UsedDescriptorSetLayouts
			);
			VerifySuccess(
				static_cast<VulkanDevice&>(InContext.GetDevice()).GetVulkanDevice().allocateDescriptorSets(
					&DescriptorSetsInfo,
					_DescriptorSets.data()
				)
			);
		}

		VulkanDescriptorTable::VulkanDescriptorTable(_In_ GraphicsContext& InContext, _In_ const RootSignatureDescriptorTable& InRootSignatureDescriptorTable, _In_ const RootSignature& InRootSignature, _In_ uint32_t InSubDescriptorTableIndex)
			: DescriptorTable(InRootSignatureDescriptorTable)
		{
			const vector<vk::DescriptorSetLayout>& DescriptorSetLayouts = static_cast<const VulkanRootSignature&>(InRootSignature).GetVulkanDescriptorSetLayouts();
			ETERNAL_ASSERT(DescriptorSetLayouts.size() > 0 && InSubDescriptorTableIndex < DescriptorSetLayouts.size());

			const vk::DescriptorPool& Pool = static_cast<VulkanGraphicsContext&>(InContext).GetVulkanDescriptorPool();
			vk::DescriptorSetLayout UsedDescriptorSetLayouts[] = { DescriptorSetLayouts[InSubDescriptorTableIndex], DescriptorSetLayouts[InSubDescriptorTableIndex] };

			vk::DescriptorSetAllocateInfo DescriptorSetsInfo(
				Pool,
				2,
				UsedDescriptorSetLayouts
			);
			VerifySuccess(
				static_cast<VulkanDevice&>(InContext.GetDevice()).GetVulkanDevice().allocateDescriptorSets(
					&DescriptorSetsInfo,
					_DescriptorSets.data()
				)
			);
		}

		void VulkanDescriptorTable::Commit(_In_ GraphicsContext& InContext, _In_ const vector<RootSignatureParameter>& InRootSignatureParameters) const
		{
			array<vk::WriteDescriptorSet, MaxWriteDescriptorSets> WriteDescriptorSets;
			WriteDescriptorSets.fill(vk::WriteDescriptorSet());

			const vector<DescriptorTableResource>& Resources = GetResources();

			array<vk::DescriptorImageInfo, MaxWriteDescriptorSets> DescriptorImageInfos;
			array<vk::DescriptorBufferInfo, MaxWriteDescriptorSets> DescriptorBufferInfos;

			RegisterIndicesPerTypePerShaderContainer RegisterIndicesPerType;
			for (uint32_t ShaderTypeIndex = 0; ShaderTypeIndex < ShaderTypeCount; ++ShaderTypeIndex)
				RegisterIndicesPerType[ShaderTypeIndex].fill(0);

			uint32_t ImageCount = 0;
			uint32_t BufferCount = 0;
			uint32_t DescriptorSetsCount = 0;
			for (uint32_t ParameterIndex = 0; ParameterIndex < InRootSignatureParameters.size(); ++ParameterIndex)
			{
				vk::WriteDescriptorSet& CurrentWriteDescriptorSet	= WriteDescriptorSets[DescriptorSetsCount++];
				vk::DescriptorImageInfo* VKDescriptorImageInfo		= nullptr;
				vk::DescriptorBufferInfo* VkDescriptorBufferInfo	= nullptr;
				const vk::BufferView* VkDescriptorTexelBufferView	= nullptr;

				switch (InRootSignatureParameters[ParameterIndex].Parameter)
				{
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_TEXTURE:
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_RW_TEXTURE:
				{
					VKDescriptorImageInfo				= static_cast<vk::DescriptorImageInfo*>(&DescriptorImageInfos[ImageCount++]);
					VKDescriptorImageInfo->imageView	= static_cast<const VulkanView*>(Resources[ParameterIndex].ResourceView)->GetVulkanImageView();
					VKDescriptorImageInfo->imageLayout	= ConvertTransitionStateToVulkanImageLayout(Resources[ParameterIndex].ResourceView->GetResourceTransition());
				} break;
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_STRUCTURED_BUFFER:
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_RW_STRUCTURED_BUFFER:
				{
					VkDescriptorTexelBufferView			= static_cast<const vk::BufferView*>(&static_cast<const VulkanView*>(Resources[ParameterIndex].ResourceView)->GetVulkanBufferView());
				} break;
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_SAMPLER:
				{
					VKDescriptorImageInfo				= static_cast<vk::DescriptorImageInfo*>(&DescriptorImageInfos[ImageCount++]);
					VKDescriptorImageInfo->sampler		= static_cast<const VulkanSampler*>(Resources[ParameterIndex].ResourceSampler)->GetVulkanSampler();
				} break;
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_CONSTANT_BUFFER:
				{
					const ViewMetaData& MetaData		= Resources[ParameterIndex].ResourceView->GetViewMetaData();

					VkDescriptorBufferInfo				= static_cast<vk::DescriptorBufferInfo*>(&DescriptorBufferInfos[BufferCount++]);
					VkDescriptorBufferInfo->buffer		= static_cast<const VulkanResource&>(Resources[ParameterIndex].ResourceView->GetResource()).GetVulkanBuffer();
					VkDescriptorBufferInfo->offset		= MetaData.ConstantBufferView.BufferOffset;
					VkDescriptorBufferInfo->range		= MetaData.ConstantBufferView.BufferSize;
				} break;
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_BUFFER:
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_RW_BUFFER:
				{
					const ViewMetaData& MetaData		= Resources[ParameterIndex].ResourceView->GetViewMetaData();
					
					VkDescriptorBufferInfo				= static_cast<vk::DescriptorBufferInfo*>(&DescriptorBufferInfos[BufferCount++]);
					VkDescriptorBufferInfo->buffer		= static_cast<const VulkanResource&>(Resources[ParameterIndex].ResourceView->GetResource()).GetVulkanBuffer();
					VkDescriptorBufferInfo->offset		= MetaData.ShaderResourceViewBuffer.FirstElement;
					VkDescriptorBufferInfo->range		= MetaData.ShaderResourceViewBuffer.NumElements * MetaData.ShaderResourceViewBuffer.StructureByteStride;
				} break;
				}

				uint32_t Offset = ConvertHLSLRegisterTypeToVulkanShaderBindingBase(
					ConvertRootSignatureParameterTypeToHLSLRegisterType(InRootSignatureParameters[ParameterIndex].Parameter)
				);
				uint32_t RegisterType = ConvertRootSignatureParameterTypeToHLSLRegisterTypeUInt(InRootSignatureParameters[ParameterIndex].Parameter);
				
				CurrentWriteDescriptorSet = vk::WriteDescriptorSet(
					GetVulkanDescriptorSet(InContext.GetCurrentFrameIndex()),
					Offset + RegisterIndicesPerType[static_cast<int32_t>(InRootSignatureParameters[ParameterIndex].Access)][RegisterType]++,
					0, 1,
					ConvertRootSignatureParameterTypeToVulkanDescriptorType(InRootSignatureParameters[ParameterIndex].Parameter),
					VKDescriptorImageInfo,
					VkDescriptorBufferInfo,
					VkDescriptorTexelBufferView
				);
			}

			static_cast<VulkanDevice&>(InContext.GetDevice()).GetVulkanDevice().updateDescriptorSets(
				DescriptorSetsCount, WriteDescriptorSets.data(),
				0, nullptr
			);
		}

		void VulkanDescriptorTable::Commit(_In_ GraphicsContext& InContext, _In_ const RootSignatureDescriptorTable& InRootSignatureDescriptorTable) const
		{
			array<vk::WriteDescriptorSet, MaxWriteDescriptorSets> WriteDescriptorSets;
			WriteDescriptorSets.fill(vk::WriteDescriptorSet());

			const vector<DescriptorTableResource>& Resources = GetResources();

			array<vk::DescriptorImageInfo, MaxWriteDescriptorSets> DescriptorImageInfos;
			array<vk::DescriptorBufferInfo, MaxWriteDescriptorSets> DescriptorBufferInfos;

			RegisterIndicesPerTypePerShaderContainer RegisterIndicesPerType;
			for (uint32_t ShaderTypeIndex = 0; ShaderTypeIndex < ShaderTypeCount; ++ShaderTypeIndex)
				RegisterIndicesPerType[ShaderTypeIndex].fill(0);

			const vector<RootSignatureDescriptorTableParameter>& InParameters = InRootSignatureDescriptorTable.Parameters;

			uint32_t ImageCount = 0;
			uint32_t BufferCount = 0;
			uint32_t DescriptorSetsCount = 0;
			for (uint32_t ParameterIndex = 0; ParameterIndex < InParameters.size(); ++ParameterIndex)
			{
				for (uint32_t DescriptorIndex = 0; DescriptorIndex < InParameters[ParameterIndex].DescriptorsCount; ++DescriptorIndex)
				{
					vk::WriteDescriptorSet& CurrentWriteDescriptorSet	= WriteDescriptorSets[DescriptorSetsCount];
					vk::DescriptorImageInfo* VKDescriptorImageInfo		= nullptr;
					vk::DescriptorBufferInfo* VkDescriptorBufferInfo	= nullptr;
					const vk::BufferView* VkDescriptorTexelBufferView	= nullptr;

					switch (InParameters[ParameterIndex].Parameter)
					{
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_TEXTURE:
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_RW_TEXTURE:
					{
						VKDescriptorImageInfo				= static_cast<vk::DescriptorImageInfo*>(&DescriptorImageInfos[ImageCount++]);
						VKDescriptorImageInfo->imageView	= static_cast<const VulkanView*>(Resources[DescriptorSetsCount].ResourceView)->GetVulkanImageView();
						VKDescriptorImageInfo->imageLayout	= ConvertTransitionStateToVulkanImageLayout(Resources[DescriptorSetsCount].ResourceView->GetResourceTransition());
					} break;
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_STRUCTURED_BUFFER:
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_RW_STRUCTURED_BUFFER:
					{
						VkDescriptorTexelBufferView			= static_cast<const vk::BufferView*>(&static_cast<const VulkanView*>(Resources[DescriptorSetsCount].ResourceView)->GetVulkanBufferView());
					} break;
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_SAMPLER:
					{
						VKDescriptorImageInfo->sampler		= static_cast<const VulkanSampler*>(Resources[DescriptorSetsCount].ResourceSampler)->GetVulkanSampler();
					} break;
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_CONSTANT_BUFFER:
					{
						const ViewMetaData& MetaData		= Resources[DescriptorSetsCount].ResourceView->GetViewMetaData();

						VkDescriptorBufferInfo->buffer		= static_cast<const VulkanResource&>(Resources[DescriptorSetsCount].ResourceView->GetResource()).GetVulkanBuffer();
						VkDescriptorBufferInfo->offset		= MetaData.ConstantBufferView.BufferOffset;
						VkDescriptorBufferInfo->range		= MetaData.ConstantBufferView.BufferSize;
					} break;
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_BUFFER:
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_RW_BUFFER:
					{
						const ViewMetaData& MetaData		= Resources[DescriptorSetsCount].ResourceView->GetViewMetaData();

						VkDescriptorBufferInfo->buffer		= static_cast<const VulkanResource&>(Resources[DescriptorSetsCount].ResourceView->GetResource()).GetVulkanBuffer();
						VkDescriptorBufferInfo->offset		= MetaData.ShaderResourceViewBuffer.FirstElement;
						VkDescriptorBufferInfo->range		= MetaData.ShaderResourceViewBuffer.NumElements * MetaData.ShaderResourceViewBuffer.StructureByteStride;
					} break;
					}

					uint32_t Offset = ConvertHLSLRegisterTypeToVulkanShaderBindingBase(
						ConvertRootSignatureParameterTypeToHLSLRegisterType(InParameters[ParameterIndex].Parameter)
					);
					uint32_t RegisterType = ConvertRootSignatureParameterTypeToHLSLRegisterTypeUInt(InParameters[ParameterIndex].Parameter);

					CurrentWriteDescriptorSet = vk::WriteDescriptorSet(
						GetVulkanDescriptorSet(InContext.GetCurrentFrameIndex()),
						RegisterIndicesPerType[static_cast<int32_t>(InParameters[ParameterIndex].Access)][RegisterType]++,
						0, 1,
						ConvertRootSignatureParameterTypeToVulkanDescriptorType(InParameters[ParameterIndex].Parameter),
						VKDescriptorImageInfo,
						VkDescriptorBufferInfo,
						VkDescriptorTexelBufferView
					);
				}
			}

			static_cast<VulkanDevice&>(InContext.GetDevice()).GetVulkanDevice().updateDescriptorSets(
				DescriptorSetsCount, WriteDescriptorSets.data(),
				0, nullptr
			);
		}
	}
}

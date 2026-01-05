#if ETERNAL_ENABLE_VULKAN

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
			, _Context(VulkanGraphicsContextCast(InContext))
			, _DescriptorSetLayout(static_cast<const VulkanRootSignature&>(*InRootSignature).GetVulkanDescriptorSetLayouts()[0])
		{
			for (uint32_t FrameIndex = 0; FrameIndex < GraphicsContext::FrameBufferingCount; ++FrameIndex)
			{
				_DescriptorSets[FrameIndex].resize(1);
				_AllocateVulkanDescriptorSet(_DescriptorSets[FrameIndex].back());
			}
			_Context.RegisterVulkanDescriptorTable(this);
		}

		VulkanDescriptorTable::VulkanDescriptorTable(_In_ GraphicsContext& InContext, _In_ const RootSignatureDescriptorTable& InRootSignatureDescriptorTable, _In_ const RootSignature& InRootSignature, _In_ uint32_t InSubDescriptorTableIndex)
			: DescriptorTable(InRootSignatureDescriptorTable)
			, _Context(VulkanGraphicsContextCast(InContext))
			, _DescriptorSetLayout(static_cast<const VulkanRootSignature&>(InRootSignature).GetVulkanDescriptorSetLayouts()[InSubDescriptorTableIndex])
		{
			for (uint32_t FrameIndex = 0; FrameIndex < GraphicsContext::FrameBufferingCount; ++FrameIndex)
			{
				_DescriptorSets[FrameIndex].resize(1);
				_AllocateVulkanDescriptorSet(_DescriptorSets[FrameIndex].back());
			}
			_Context.RegisterVulkanDescriptorTable(this);
		}

		VulkanDescriptorTable::~VulkanDescriptorTable()
		{
			vk::Device& VkDevice = static_cast<VulkanDevice&>(_Context.GetDevice()).GetVulkanDevice();

			_Context.UnregisterVulkanDescriptorTable(this);
			for (uint32_t FrameIndex = 0; FrameIndex < GraphicsContext::FrameBufferingCount; ++FrameIndex)
			{
				VkDevice.freeDescriptorSets(
					_Context.GetVulkanDescriptorPool(),
					static_cast<uint32_t>(_DescriptorSets[FrameIndex].size()),
					_DescriptorSets[FrameIndex].data()
				);
				VkDevice.freeDescriptorSets(
					_Context.GetVulkanDescriptorPool(),
					static_cast<uint32_t>(_UsedDescriptorSets[FrameIndex].size()),
					_UsedDescriptorSets[FrameIndex].data()
				);
			}
		}

		void VulkanDescriptorTable::Reset(_In_ uint32_t FrameIndex)
		{
			_DescriptorSets[FrameIndex].insert(_DescriptorSets[FrameIndex].end(), _UsedDescriptorSets[FrameIndex].begin(), _UsedDescriptorSets[FrameIndex].end());
			_UsedDescriptorSets[FrameIndex].clear();
			
			GetResourcesDirtyFlags().Reset();
			GetConstantsDirtyFlags().Reset();
		}

		void VulkanDescriptorTable::Commit(_In_ GraphicsContext& InContext, _In_ const vector<RootSignatureParameter>& InRootSignatureParameters)
		{
			ETERNAL_PROFILER(INFO)();
			array<vk::WriteDescriptorSet, MaxWriteDescriptorSets> WriteDescriptorSets;
			WriteDescriptorSets.fill(vk::WriteDescriptorSet());

			const vector<DescriptorTableResource>& Resources	= GetResources();
			ResourcesDirtyFlagsType& ResourcesDirtyFlags		= GetResourcesDirtyFlags();

			array<vk::DescriptorImageInfo, MaxWriteDescriptorSets> DescriptorImageInfos;
			array<vk::DescriptorBufferInfo, MaxWriteDescriptorSets> DescriptorBufferInfos;

			RegisterIndicesPerTypePerShaderContainer RegisterIndicesPerType;
			for (uint32_t ShaderTypeIndex = 0; ShaderTypeIndex < ShaderTypeCount; ++ShaderTypeIndex)
				RegisterIndicesPerType[ShaderTypeIndex].fill(0);

			uint32_t ImageCount = 0;
			uint32_t BufferCount = 0;
			uint32_t DescriptorSetsCount = 0;
			uint32_t IsSetCount = 0;

			for (uint32_t ParameterIndex = 0; ParameterIndex < InRootSignatureParameters.size(); ++ParameterIndex)
			{
				bool IsSet = ResourcesDirtyFlags.IsSet(ParameterIndex);
				IsSetCount += IsSet ? 1 : 0;

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
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_STRUCTURED_BUFFER:
				{
					const ViewMetaData& MetaData		= Resources[ParameterIndex].ResourceView->GetViewMetaData();

					VkDescriptorBufferInfo				= static_cast<vk::DescriptorBufferInfo*>(&DescriptorBufferInfos[BufferCount++]);
					VkDescriptorBufferInfo->buffer		= static_cast<const VulkanResource&>(Resources[ParameterIndex].ResourceView->GetResource()).GetVulkanBuffer();
					VkDescriptorBufferInfo->offset		= MetaData.ConstantBufferView.BufferElementOffset * Resources[ParameterIndex].ResourceView->GetResource().GetBufferStride();
					VkDescriptorBufferInfo->range		= MetaData.ConstantBufferView.BufferSize > 0 ? MetaData.ConstantBufferView.BufferSize : VK_WHOLE_SIZE;
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
				uint32_t StageOffset = static_cast<uint32_t>(InRootSignatureParameters[ParameterIndex].Access) * (VulkanGraphicsContext::ShaderRegisterSamplersOffset + VulkanGraphicsContext::MaxSamplersCountPerShader);
				uint32_t RegisterType = ConvertRootSignatureParameterTypeToHLSLRegisterTypeUInt(InRootSignatureParameters[ParameterIndex].Parameter);
				
				CurrentWriteDescriptorSet = vk::WriteDescriptorSet(
					vk::DescriptorSet(),
					Offset + StageOffset + RegisterIndicesPerType[static_cast<int32_t>(InRootSignatureParameters[ParameterIndex].Access)][RegisterType]++,
					0, 1,
					ConvertRootSignatureParameterTypeToVulkanDescriptorType(InRootSignatureParameters[ParameterIndex].Parameter),
					VKDescriptorImageInfo,
					VkDescriptorBufferInfo,
					VkDescriptorTexelBufferView
				);

				if (IsSet)
					ResourcesDirtyFlags.Unset(ParameterIndex);
			}

			if (IsSetCount > 0)
			{
				ETERNAL_PROFILER(INFO)("UpdateDescriptorSets");
				const vk::DescriptorSet& CurrentDescriptorSet = AllocateVulkanDescriptorSet(InContext.GetCurrentFrameIndex());

				for (uint32_t DescriptorSetIndex = 0; DescriptorSetIndex < DescriptorSetsCount; ++DescriptorSetIndex)
					WriteDescriptorSets[DescriptorSetIndex].setDstSet(CurrentDescriptorSet);

				static_cast<VulkanDevice&>(InContext.GetDevice()).GetVulkanDevice().updateDescriptorSets(
					DescriptorSetsCount, WriteDescriptorSets.data(),
					0, nullptr
				);
			}
		}

		void VulkanDescriptorTable::Commit(_In_ GraphicsContext& InContext, _In_ const RootSignatureDescriptorTable& InRootSignatureDescriptorTable)
		{
			ETERNAL_PROFILER(INFO)();
			array<vk::WriteDescriptorSet, MaxWriteDescriptorSets> WriteDescriptorSets;
			WriteDescriptorSets.fill(vk::WriteDescriptorSet());

			const vector<DescriptorTableResource>& Resources	= GetResources();
			ResourcesDirtyFlagsType& ResourcesDirtyFlags		= GetResourcesDirtyFlags();

			array<vk::DescriptorImageInfo, MaxWriteDescriptorSets> DescriptorImageInfos;
			array<vk::DescriptorBufferInfo, MaxWriteDescriptorSets> DescriptorBufferInfos;

			RegisterIndicesPerTypePerShaderContainer RegisterIndicesPerType;
			for (uint32_t ShaderTypeIndex = 0; ShaderTypeIndex < ShaderTypeCount; ++ShaderTypeIndex)
				RegisterIndicesPerType[ShaderTypeIndex].fill(0);

			const vector<RootSignatureDescriptorTableParameter>& InParameters = InRootSignatureDescriptorTable.Parameters;

			uint32_t ImageCount = 0;
			uint32_t BufferCount = 0;
			uint32_t DescriptorsCount = 0;
			uint32_t WriteDescriptorsCount = 0;
			uint32_t IsSetCount = 0;

			for (uint32_t ParameterIndex = 0; ParameterIndex < InParameters.size(); ++ParameterIndex)
			{
				for (uint32_t DescriptorIndex = 0; DescriptorIndex < InParameters[ParameterIndex].DescriptorsCount; ++DescriptorIndex)
				{
					IsSetCount += ResourcesDirtyFlags.IsSet(ParameterIndex) ? 1 : 0;

					vk::WriteDescriptorSet& CurrentWriteDescriptorSet	= WriteDescriptorSets[WriteDescriptorsCount++];
					vk::DescriptorImageInfo* VKDescriptorImageInfo		= nullptr;
					vk::DescriptorBufferInfo* VkDescriptorBufferInfo	= nullptr;
					const vk::BufferView* VkDescriptorTexelBufferView	= nullptr;

					switch (InParameters[ParameterIndex].Parameter)
					{
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_TEXTURE:
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_RW_TEXTURE:
					{
						VKDescriptorImageInfo				= static_cast<vk::DescriptorImageInfo*>(&DescriptorImageInfos[ImageCount++]);
						VKDescriptorImageInfo->imageView	= static_cast<const VulkanView*>(Resources[DescriptorsCount].ResourceView)->GetVulkanImageView();
						VKDescriptorImageInfo->imageLayout	= ConvertTransitionStateToVulkanImageLayout(Resources[DescriptorsCount].ResourceView->GetResourceTransition());
					} break;
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_STRUCTURED_BUFFER:
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_RW_STRUCTURED_BUFFER:
					{
						VkDescriptorTexelBufferView			= static_cast<const vk::BufferView*>(&static_cast<const VulkanView*>(Resources[DescriptorsCount].ResourceView)->GetVulkanBufferView());
					} break;
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_SAMPLER:
					{
						VKDescriptorImageInfo				= static_cast<vk::DescriptorImageInfo*>(&DescriptorImageInfos[ImageCount++]);
						VKDescriptorImageInfo->sampler		= static_cast<const VulkanSampler*>(Resources[DescriptorsCount].ResourceSampler)->GetVulkanSampler();
					} break;
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_CONSTANT_BUFFER:
					{
						const ViewMetaData& MetaData		= Resources[DescriptorsCount].ResourceView->GetViewMetaData();
						
						VkDescriptorBufferInfo				= static_cast<vk::DescriptorBufferInfo*>(&DescriptorBufferInfos[BufferCount++]);
						VkDescriptorBufferInfo->buffer		= static_cast<const VulkanResource&>(Resources[DescriptorsCount].ResourceView->GetResource()).GetVulkanBuffer();
						VkDescriptorBufferInfo->offset		= MetaData.ConstantBufferView.BufferElementOffset * Resources[DescriptorsCount].ResourceView->GetResource().GetBufferStride();
						VkDescriptorBufferInfo->range		= MetaData.ConstantBufferView.BufferSize;
					} break;
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_BUFFER:
					case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_RW_BUFFER:
					{
						const ViewMetaData& MetaData		= Resources[DescriptorsCount].ResourceView->GetViewMetaData();
						
						VkDescriptorBufferInfo				= static_cast<vk::DescriptorBufferInfo*>(&DescriptorBufferInfos[BufferCount++]);
						VkDescriptorBufferInfo->buffer		= static_cast<const VulkanResource&>(Resources[DescriptorsCount].ResourceView->GetResource()).GetVulkanBuffer();
						VkDescriptorBufferInfo->offset		= MetaData.ShaderResourceViewBuffer.FirstElement;
						VkDescriptorBufferInfo->range		= MetaData.ShaderResourceViewBuffer.NumElements * MetaData.ShaderResourceViewBuffer.StructureByteStride;
					} break;
					}

					uint32_t Offset = ConvertHLSLRegisterTypeToVulkanShaderBindingBase(
						ConvertRootSignatureParameterTypeToHLSLRegisterType(InParameters[ParameterIndex].Parameter)
					);
					uint32_t RegisterType = ConvertRootSignatureParameterTypeToHLSLRegisterTypeUInt(InParameters[ParameterIndex].Parameter);

					CurrentWriteDescriptorSet = vk::WriteDescriptorSet(
						vk::DescriptorSet(),
						RegisterIndicesPerType[static_cast<int32_t>(InParameters[ParameterIndex].Access)][RegisterType]++,
						0, 1,
						ConvertRootSignatureParameterTypeToVulkanDescriptorType(InParameters[ParameterIndex].Parameter),
						VKDescriptorImageInfo,
						VkDescriptorBufferInfo,
						VkDescriptorTexelBufferView
					);

					ResourcesDirtyFlags.Unset(DescriptorsCount);
					++DescriptorsCount;
				}
			}

			if (IsSetCount > 0)
			{
				ETERNAL_PROFILER(INFO)("UpdateDescriptorSets");
				const vk::DescriptorSet& CurrentDescriptorSet = AllocateVulkanDescriptorSet(InContext.GetCurrentFrameIndex());

				for (uint32_t DescriptorSetIndex = 0; DescriptorSetIndex < WriteDescriptorsCount; ++DescriptorSetIndex)
					WriteDescriptorSets[DescriptorSetIndex].setDstSet(CurrentDescriptorSet);

				static_cast<VulkanDevice&>(InContext.GetDevice()).GetVulkanDevice().updateDescriptorSets(
					WriteDescriptorsCount, WriteDescriptorSets.data(),
					0, nullptr
				);
			}
		}

		const vk::DescriptorSet& VulkanDescriptorTable::AllocateVulkanDescriptorSet(_In_ uint32_t FrameIndex)
		{
			if (_DescriptorSets[FrameIndex].size() > 0)
			{
				_UsedDescriptorSets[FrameIndex].push_back(_DescriptorSets[FrameIndex].back());
				_DescriptorSets[FrameIndex].pop_back();
			}
			else
			{
				_UsedDescriptorSets[FrameIndex].push_back(vk::DescriptorSet());
				_AllocateVulkanDescriptorSet(_UsedDescriptorSets[FrameIndex].back());
			}
			return _UsedDescriptorSets[FrameIndex].back();
		}

		void VulkanDescriptorTable::_AllocateVulkanDescriptorSet(_Out_ vk::DescriptorSet& OutDescriptorSet)
		{
			vk::DescriptorSetAllocateInfo DescriptorSetsInfo(
				_Context.GetVulkanDescriptorPool(),
				1,
				&_DescriptorSetLayout
			);

			VulkanDevice& VkDevice = static_cast<VulkanDevice&>(_Context.GetDevice());
			VerifySuccess(
				VkDevice.GetVulkanDevice().allocateDescriptorSets(
					&DescriptorSetsInfo,
					&OutDescriptorSet
				)
			);
		}
	}
}

#endif

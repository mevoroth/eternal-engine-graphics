#include "Vulkan/VulkanRootSignature.hpp"

#include "Graphics/GraphicsContext.hpp"
#include "Graphics/ShaderType.hpp"
#include "Vulkan/VulkanUtils.hpp"
#include "Vulkan/VulkanSampler.hpp"
#include "Vulkan/VulkanDevice.hpp"
#include "Vulkan/VulkanGraphicsContext.hpp"

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		VulkanRootSignature::VulkanRootSignature(GraphicsContext& InContext)
			: RootSignature()
			, _Context(InContext)
		{
			using namespace Vulkan;

			_DescriptorSetLayouts.resize(1);

			vk::Device& VkDevice = static_cast<VulkanDevice&>(InContext.GetDevice()).GetVulkanDevice();
			vk::DescriptorSetLayoutCreateInfo DescriptorSetLayoutInfo;
			VerifySuccess(
				VkDevice.createDescriptorSetLayout(
					&DescriptorSetLayoutInfo,
					nullptr,
					&_DescriptorSetLayouts[0]
				)
			);

			vk::PipelineLayoutCreateInfo PipelineLayoutInfo;
			VerifySuccess(
				VkDevice.createPipelineLayout(
					&PipelineLayoutInfo,
					nullptr,
					&_PipelineLayout
				)
			);
		}

		VulkanRootSignature::VulkanRootSignature(_In_ GraphicsContext& InContext, _In_ const RootSignatureCreateInformation& InRootSignatureCreateInformation)
			: RootSignature(InRootSignatureCreateInformation)
			, _Context(InContext)
		{
			using namespace Eternal::Graphics::Vulkan;

			vk::Device& VkDevice = static_cast<VulkanDevice&>(InContext.GetDevice()).GetVulkanDevice();
			
			const vector<RootSignatureParameter>& InParameters			= InRootSignatureCreateInformation.Parameters;
			const vector<RootSignatureStaticSampler>& InStaticSamplers	= InRootSignatureCreateInformation.StaticSamplers;

			RegisterIndicesPerTypePerShaderContainer RegisterIndicesPerType;
			for (uint32_t TypeIndex = 0; TypeIndex < ShaderTypeCount; ++TypeIndex)
				RegisterIndicesPerType[TypeIndex].fill(0);

			_DescriptorSetLayouts.reserve(
				InParameters.size()
				+ (InStaticSamplers.size() > 0 ? 1 : 0)
			);
			_DescriptorSetLayouts.push_back(vk::DescriptorSetLayout());
			vk::DescriptorSetLayout& RootDescriptorSetLayout = _DescriptorSetLayouts.back();

			vector<vk::DescriptorSetLayoutBinding> RootBindings;
			RootBindings.reserve(InParameters.size());

			for (uint32_t ParameterIndex = 0; ParameterIndex < InParameters.size(); ++ParameterIndex)
			{
				const RootSignatureParameterType& CurrentParameterType	= InParameters[ParameterIndex].Parameter;
				const RootSignatureAccess& CurrentAccess				= InParameters[ParameterIndex].Access;

				switch (InParameters[ParameterIndex].Parameter)
				{
				case RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE:
				{
					_CreateDescriptorSetLayout(InParameters[ParameterIndex].DescriptorTable);
				} break;
				default:
				{
					uint32_t Offset = ConvertHLSLRegisterTypeToVulkanShaderBindingBase(
						ConvertRootSignatureParameterTypeToHLSLRegisterType(CurrentParameterType)
					);

					RootBindings.push_back(
						vk::DescriptorSetLayoutBinding(
							Offset + RegisterIndicesPerType[static_cast<int32_t>(CurrentAccess)][ConvertRootSignatureParameterTypeToHLSLRegisterTypeUInt(CurrentParameterType)]++,
							ConvertRootSignatureParameterTypeToVulkanDescriptorType(InParameters[ParameterIndex].Parameter),
							1,
							ConvertRootSignatureAccessToShaderStageFlags(CurrentAccess),
							nullptr
						)
					);
				} break;
				}
			}

			vector<vk::DescriptorSetLayoutBinding> SamplerBindings;
			if (InStaticSamplers.size() > 0)
			{
				SamplerBindings.resize(InStaticSamplers.size());

				RegisterIndicesContainer SamplerRegisterIndex;
				SamplerRegisterIndex.fill(0);

				for (uint32_t SamplerIndex = 0; SamplerIndex < InStaticSamplers.size(); ++SamplerIndex)
				{
					const RootSignatureAccess& CurrentAccess = InStaticSamplers[SamplerIndex].Access;

					SamplerBindings[SamplerIndex] = vk::DescriptorSetLayoutBinding(
						SamplerRegisterIndex[static_cast<int32_t>(CurrentAccess)]++,
						vk::DescriptorType::eSampler,
						1,
						ConvertRootSignatureAccessToShaderStageFlags(CurrentAccess),
						&static_cast<VulkanSampler*>(InStaticSamplers[SamplerIndex].StaticSampler)->GetVulkanSampler()
					);
				}
			}

			vk::DescriptorSetLayoutCreateInfo ParametersDescriptorSetLayoutCreateInfo(
				vk::DescriptorSetLayoutCreateFlagBits(),
				static_cast<uint32_t>(RootBindings.size()),
				RootBindings.data()
			);

			VerifySuccess(
				VkDevice.createDescriptorSetLayout(
					&ParametersDescriptorSetLayoutCreateInfo,
					nullptr,
					&RootDescriptorSetLayout
				)
			);

			if (InStaticSamplers.size() > 0)
			{
				_DescriptorSetLayouts.push_back(vk::DescriptorSetLayout());

				vk::DescriptorSetLayoutCreateInfo StaticSamplersDescriptorSetLayoutCreateInfo(
					vk::DescriptorSetLayoutCreateFlagBits(),
					static_cast<uint32_t>(SamplerBindings.size()),
					SamplerBindings.data()
				);

				VerifySuccess(
					VkDevice.createDescriptorSetLayout(
						&StaticSamplersDescriptorSetLayoutCreateInfo,
						nullptr,
						&_DescriptorSetLayouts.back()
					)
				);
			}

			uint32_t ConstantCount = 0;
			const vector<RootSignatureConstants>& InConstants = InRootSignatureCreateInformation.Constants;
			for (uint32_t ConstantIndex = 0; ConstantIndex < InConstants.size(); ++ConstantIndex)
				ConstantCount += InConstants[ConstantIndex].Count;

			_PushConstantRanges.reserve(ConstantCount);
			if (ConstantCount > 0)
			{
				bool IsConstantRangeOpen = false;

				// Initialize a new entry
				auto OpenConstantRange = [&IsConstantRangeOpen](const RootSignatureAccess& InAccess, const Handle& InHandle, uint32_t& OutRangeCount, vector<vk::PushConstantRange>& InOutConstantRanges) -> vk::PushConstantRange*
				{
					InOutConstantRanges.push_back(vk::PushConstantRange());
					vk::PushConstantRange& CurrentConstantRange	= InOutConstantRanges.back();
					CurrentConstantRange.stageFlags				= ConvertRootSignatureAccessToShaderStageFlags(InAccess);
					CurrentConstantRange.offset					= static_cast<uint32_t>(InHandle * ByteTo32Bits);
					OutRangeCount								= 1;

#ifdef ETERNAL_DEBUG
					ETERNAL_ASSERT(!IsConstantRangeOpen);
					IsConstantRangeOpen = true;
#endif

					return &CurrentConstantRange;
				};

				// Finalize current entry
				auto CloseConstantRange = [&IsConstantRangeOpen](uint32_t InRangeCount, vk::PushConstantRange& InOutConstantRange)
				{
					InOutConstantRange.size						= InRangeCount * ByteTo32Bits;
#ifdef ETERNAL_DEBUG
					ETERNAL_ASSERT(IsConstantRangeOpen);
					IsConstantRangeOpen = false;
#endif
				};

				static_cast<VulkanGraphicsContext&>(InContext).AllocateConstantHandles(ConstantCount, _ConstantHandles);
				uint32_t HandleIndexOffset = 0;
				for (uint32_t ConstantIndex = 0; ConstantIndex < InConstants.size(); ++ConstantIndex)
				{
					const RootSignatureAccess& CurrentAccess	= InConstants[ConstantIndex].Access;
					const uint32_t CurrentConstantCount			= InConstants[ConstantIndex].Count;
					Handle& PreviousHandle						= _ConstantHandles[HandleIndexOffset];

					uint32_t RangeCount = 0;
					vk::PushConstantRange* CurrentConstantRange = OpenConstantRange(
						CurrentAccess,
						PreviousHandle,
						RangeCount,
						_PushConstantRanges
					);

					for (uint32_t ConstantIndex = 1; ConstantIndex < CurrentConstantCount; ++ConstantIndex)
					{
						uint32_t CurrentHandleIndex = (HandleIndexOffset + ConstantIndex);
						ETERNAL_ASSERT(CurrentHandleIndex < _ConstantHandles.size());
						bool IsContigous = _ConstantHandles[CurrentHandleIndex].IsContigous(PreviousHandle);
						PreviousHandle = _ConstantHandles[CurrentHandleIndex];
						if (_ConstantHandles[CurrentHandleIndex].IsContigous(PreviousHandle))
							++RangeCount;
						else
						{
							CloseConstantRange(RangeCount, *CurrentConstantRange);

							CurrentConstantRange = OpenConstantRange(
								CurrentAccess,
								PreviousHandle,
								RangeCount,
								_PushConstantRanges
							);
						}
					}
					HandleIndexOffset += CurrentConstantCount;

					CloseConstantRange(RangeCount, *CurrentConstantRange);
				}

				ETERNAL_ASSERT(!IsConstantRangeOpen); // Ensure we don't have half filled constant range
			}

			vk::PipelineLayoutCreateInfo PipelineLayoutInfo(
				vk::PipelineLayoutCreateFlagBits(),
				static_cast<uint32_t>(_DescriptorSetLayouts.size()),
				_DescriptorSetLayouts.data(),
				static_cast<uint32_t>(_PushConstantRanges.size()),
				_PushConstantRanges.data()
			);
			VerifySuccess(
				VkDevice.createPipelineLayout(
					&PipelineLayoutInfo,
					nullptr,
					&_PipelineLayout
				)
			);
		}

		VulkanRootSignature::~VulkanRootSignature()
		{
			VulkanGraphicsContext& VkGraphicsContext = static_cast<VulkanGraphicsContext&>(_Context);
			vk::Device& VkDevice = static_cast<VulkanDevice&>(_Context.GetDevice()).GetVulkanDevice();
			VkGraphicsContext.ReleaseConstantHandles(_ConstantHandles);
			for (uint32_t DescriptorSetLayoutIndex = 0; DescriptorSetLayoutIndex < _DescriptorSetLayouts.size(); ++DescriptorSetLayoutIndex)
				VkDevice.destroyDescriptorSetLayout(_DescriptorSetLayouts[DescriptorSetLayoutIndex]);
			VkDevice.destroyPipelineLayout(_PipelineLayout);
		}

		void VulkanRootSignature::_CreateDescriptorSetLayout(_In_ const RootSignatureDescriptorTable& InDescriptorTable)
		{
			using namespace Eternal::Graphics::Vulkan;

			vk::Device& VkDevice = static_cast<VulkanDevice&>(_Context.GetDevice()).GetVulkanDevice();

			RegisterIndicesContainer RegisterIndices;
			RegisterIndices.fill(0);

			const vector<RootSignatureDescriptorTableParameter>& InParameters = InDescriptorTable.Parameters;
			vector<vk::DescriptorSetLayoutBinding> Bindings;
			Bindings.resize(InParameters.size());
			
			vector<vector<vk::Sampler>> Samplers;
			Samplers.reserve(2);

			for (uint32_t DescriptorTableIndex = 0; DescriptorTableIndex < InParameters.size(); ++DescriptorTableIndex)
			{
				ETERNAL_ASSERT(InParameters[DescriptorTableIndex].Parameter != RootSignatureParameterType::ROOT_SIGNATURE_PARAMETER_DESCRIPTOR_TABLE);

				const RootSignatureAccess& CurrentAccess				= InParameters[DescriptorTableIndex].Access;
				const RootSignatureParameterType& CurrentParameterType	= InParameters[DescriptorTableIndex].Parameter;

				uint32_t DescriptorsCount = InParameters[DescriptorTableIndex].DescriptorsCount;

				Bindings[DescriptorTableIndex] = vk::DescriptorSetLayoutBinding(
					RegisterIndices[static_cast<int32_t>(CurrentAccess)],
					ConvertRootSignatureParameterTypeToVulkanDescriptorType(CurrentParameterType),
					DescriptorsCount,
					ConvertRootSignatureAccessToShaderStageFlags(CurrentAccess),
					nullptr
				);

				RegisterIndices[static_cast<int32_t>(CurrentAccess)] += DescriptorsCount;
			}

			_DescriptorSetLayouts.push_back(vk::DescriptorSetLayout());

			vk::DescriptorSetLayoutCreateInfo DescriptorSetLayoutCreateInfo(
				vk::DescriptorSetLayoutCreateFlagBits(),
				static_cast<uint32_t>(Bindings.size()),
				Bindings.data()
			);

			VerifySuccess(
				VkDevice.createDescriptorSetLayout(
					&DescriptorSetLayoutCreateInfo,
					nullptr,
					&_DescriptorSetLayouts.back()
				)
			);
		}
	}
}

#include "Vulkan/VulkanUtils.hpp"

#include <algorithm>
#include <vulkan/vulkan.hpp>
#include "Graphics/RootSignature.hpp"
#include "Graphics/Viewport.hpp"

namespace Eternal
{
	namespace Graphics
	{
		namespace Vulkan
		{
			static constexpr vk::LogicOp VULKAN_LOGIC_OPS[] =
			{
				vk::LogicOp::eClear,
				vk::LogicOp::eSet,
				vk::LogicOp::eCopy,
				vk::LogicOp::eCopyInverted,
				vk::LogicOp::eNoOp,
				vk::LogicOp::eInvert,
				vk::LogicOp::eAnd,
				vk::LogicOp::eNand,
				vk::LogicOp::eOr,
				vk::LogicOp::eNor,
				vk::LogicOp::eXor,
				vk::LogicOp::eEquivalent,
				vk::LogicOp::eAndReverse,
				vk::LogicOp::eAndInverted,
				vk::LogicOp::eOrReverse,
				vk::LogicOp::eOrInverted
			};

			void VerifySuccess(const vk::Result& VulkanResult)
			{
				ETERNAL_ASSERT(VulkanResult == vk::Result::eSuccess);
			}

			vk::StencilOpState CreateStencilOpState(const StencilTest& InStencilTest, const StencilTest::FaceOperator& InFaceOperator)
			{
				return vk::StencilOpState(
					vk::StencilOp(InFaceOperator.Fail),
					vk::StencilOp(InFaceOperator.Pass),
					vk::StencilOp(InFaceOperator.FailDepth),
					vk::CompareOp(InFaceOperator.ComparisonOp),
					uint32_t(InStencilTest.GetReadMask()),
					uint32_t(InStencilTest.GetWriteMask()),
					uint32_t(InStencilTest.GetReference())
				);
			}

			vk::LogicOp ConvertLogicOpToVulkanLogicOp(const LogicOp& InLogicOp)
			{
				return VULKAN_LOGIC_OPS[int(InLogicOp)];
			}

			vk::ShaderStageFlagBits ConvertRootSignatureAccessToShaderStageFlags(_In_ const RootSignatureAccess& InRootSignatureAccess)
			{
				int RootSignatureAccessInt = int(InRootSignatureAccess);
				return vk::ShaderStageFlagBits(
					int(RootSignatureAccessInt & int(RootSignatureAccess::ROOT_SIGNATURE_ACCESS_VS) ? int(vk::ShaderStageFlagBits::eVertex)					: 0) |
					int(RootSignatureAccessInt & int(RootSignatureAccess::ROOT_SIGNATURE_ACCESS_HS) ? int(vk::ShaderStageFlagBits::eTessellationControl)	: 0) |
					int(RootSignatureAccessInt & int(RootSignatureAccess::ROOT_SIGNATURE_ACCESS_DS) ? int(vk::ShaderStageFlagBits::eTessellationEvaluation)	: 0) |
					int(RootSignatureAccessInt & int(RootSignatureAccess::ROOT_SIGNATURE_ACCESS_GS) ? int(vk::ShaderStageFlagBits::eGeometry)				: 0) |
					int(RootSignatureAccessInt & int(RootSignatureAccess::ROOT_SIGNATURE_ACCESS_PS) ? int(vk::ShaderStageFlagBits::eFragment)				: 0)
				);
			}

			vk::Rect2D ConvertViewportToRect2D(_In_ const Viewport& InViewport)
			{
				return vk::Rect2D(
					vk::Offset2D(InViewport.GetX(), InViewport.GetY()),
					vk::Extent2D(InViewport.GetWidth(), InViewport.GetHeight())
				);
			}

			vk::MemoryPropertyFlagBits ConvertGraphicsMemoryFlagsToMemoryPropertyFlags(_In_ const GraphicsMemoryFlag& InMemoryFlags)
			{
				int VulkanMemoryPropertyFlags = 0x0;
				int MemoryFlagsInt = int(InMemoryFlags);

				if (MemoryFlagsInt & int(GraphicsMemoryFlag::MEMORY_FLAG_GPU))
					VulkanMemoryPropertyFlags |= int(vk::MemoryPropertyFlagBits::eDeviceLocal);

				if (MemoryFlagsInt & int(GraphicsMemoryFlag::MEMORY_FLAG_MAPPABLE))
					VulkanMemoryPropertyFlags |= int(vk::MemoryPropertyFlagBits::eHostVisible);

				if (MemoryFlagsInt & int(GraphicsMemoryFlag::MEMORY_FLAG_MANAGED_CACHE))
					VulkanMemoryPropertyFlags |= int(vk::MemoryPropertyFlagBits::eHostCoherent);

				if (MemoryFlagsInt & int(GraphicsMemoryFlag::MEMORY_FLAG_CPU_CACHED))
					VulkanMemoryPropertyFlags |= int(vk::MemoryPropertyFlagBits::eHostCached);

				return vk::MemoryPropertyFlagBits(VulkanMemoryPropertyFlags);
			}

			void VerifyMemoryPropertyFlags(const vk::MemoryPropertyFlagBits& Flags)
			{
				vk::MemoryPropertyFlagBits AllowedMemoryProperties[] =
				{
					vk::MemoryPropertyFlagBits(),
					vk::MemoryPropertyFlagBits(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
					vk::MemoryPropertyFlagBits(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT),
					vk::MemoryPropertyFlagBits(VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
					vk::MemoryPropertyFlagBits(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
					vk::MemoryPropertyFlagBits(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
					vk::MemoryPropertyFlagBits(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT),
					vk::MemoryPropertyFlagBits(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_CACHED_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT),
					vk::MemoryPropertyFlagBits(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | VK_MEMORY_PROPERTY_LAZILY_ALLOCATED_BIT)
				};
				vk::MemoryPropertyFlagBits* EndAllowedMemoryProperties = AllowedMemoryProperties + ETERNAL_ARRAYSIZE(AllowedMemoryProperties);
				ETERNAL_ASSERT(std::find(AllowedMemoryProperties, EndAllowedMemoryProperties, Flags) != EndAllowedMemoryProperties);
			}
		}
	}
}

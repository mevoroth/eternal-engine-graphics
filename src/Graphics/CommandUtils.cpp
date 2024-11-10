#include "Graphics/CommandUtils.hpp"

#include "Graphics/View.hpp"
#include "Graphics/Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		TransitionFlags operator|(const TransitionFlags& InLeftTransitionFlags, const TransitionFlags& InRightTransitionFlags)
		{
			return static_cast<TransitionFlags>(
				static_cast<uint32_t>(InLeftTransitionFlags) | static_cast<uint32_t>(InRightTransitionFlags)
			);
		}

		TransitionFlags operator&(const TransitionFlags& InLeftTransitionFlags, const TransitionFlags& InRightTransitionFlags)
		{
			return static_cast<TransitionFlags>(
				static_cast<uint32_t>(InLeftTransitionFlags) & static_cast<uint32_t>(InRightTransitionFlags)
			);
		}

		TransitionFlags& operator&=(TransitionFlags& InOutTransitionFlags, const TransitionFlags& InOtherTransitionFlags)
		{
			InOutTransitionFlags = static_cast<TransitionFlags>(
				static_cast<uint32_t>(InOutTransitionFlags) & static_cast<uint32_t>(InOtherTransitionFlags)
			);
			return InOutTransitionFlags;
		}

		TransitionFlags operator~(const TransitionFlags& InTransitionFlags)
		{
			return static_cast<TransitionFlags>(
				~static_cast<uint32_t>(InTransitionFlags)
			);
		}
		
		//////////////////////////////////////////////////////////////////////////
		// ResourceSubResource

		ResourceSubResource::ResourceSubResource(_In_ uint16_t InArraySlice, _In_ uint8_t InMipSlice)
			: ArraySlice(InArraySlice)
			, MipSlice(InMipSlice)
		{
		}

		bool ResourceSubResource::IsWhole() const
		{
			return ArraySlice == InvalidArraySlice
				&& ArraySize == InvalidArraySlice
				&& MipSlice == InvalidMipLevel
				&& MipLevels == InvalidMipLevel;
		}

		bool ResourceSubResource::ArraySizeNeedsResolve() const
		{
			return ArraySize == InvalidArraySlice;
		}

		bool ResourceSubResource::MipLevelsNeedsResolve() const
		{
			return MipLevels == InvalidMipLevel;
		}

		//////////////////////////////////////////////////////////////////////////
		// ResourceTransition

		ResourceTransition::ResourceTransition( _In_ View* InView, _In_ const TransitionState& InAfter, _In_ const CommandType& InBeforeCommandType /* = CommandType::COMMAND_TYPE_GRAPHIC */, _In_ const CommandType& InAfterCommandType /* = CommandType::COMMAND_TYPE_GRAPHIC */ )
			: Flags(TransitionFlags::TRANSITION_FLAGS_NEEDS_RESOLVE)
			, ViewToTransition(InView)
			, After(InAfter)
			, BeforeCommandType(InBeforeCommandType)
			, AfterCommandType(InAfterCommandType)
		{
		}

		ResourceTransition::ResourceTransition( _In_ Resource* InResource, _In_ const TransitionState& InAfter, _In_ const CommandType& InBeforeCommandType /* = CommandType::COMMAND_TYPE_GRAPHIC */, _In_ const CommandType& InAfterCommandType /* = CommandType::COMMAND_TYPE_GRAPHIC */ )
			: Flags(TransitionFlags::TRANSITION_FLAGS_NEEDS_RESOLVE | TransitionFlags::TRANSITION_FLAGS_RESOURCE)
			, ResourceToTransition(InResource)
			, After(InAfter)
			, BeforeCommandType(InBeforeCommandType)
			, AfterCommandType(InAfterCommandType)
		{
		}

		ResourceTransition::ResourceTransition( _In_ const ResourceViewSubResource& InViewSubResource, _In_ const TransitionState& InAfter, _In_ const CommandType& InBeforeCommandType /* = CommandType::COMMAND_TYPE_GRAPHICS */, _In_ const CommandType& InAfterCommandType /* = CommandType::COMMAND_TYPE_GRAPHICS */ )
			: SubResource(InViewSubResource.SubResource)
			, Flags(TransitionFlags::TRANSITION_FLAGS_NEEDS_RESOLVE)
			, ViewToTransition(InViewSubResource.ResourceView)
			, After(InAfter)
			, BeforeCommandType(InBeforeCommandType)
			, AfterCommandType(InAfterCommandType)
		{
		}

		const Resource& ResourceTransition::GetResource() const
		{
			return IsResource() ? *ResourceToTransition : ViewToTransition->GetResource();
		}

		Resource& ResourceTransition::GetResource()
		{
			return IsResource() ? *ResourceToTransition : ViewToTransition->GetResource();
		}

		bool ResourceTransition::IsResource() const
		{
			return (Flags & TransitionFlags::TRANSITION_FLAGS_RESOURCE) != TransitionFlags::TRANSITION_FLAGS_NONE;
		}

		const TransitionState& ResourceTransition::GetBefore() const
		{
			return ((Flags & TransitionFlags::TRANSITION_FLAGS_NEEDS_RESOLVE) != TransitionFlags::TRANSITION_FLAGS_NONE) ? GetResource().GetResourceState() : Before;
		}

		const TransitionState& ResourceTransition::GetBefore(_In_ uint32_t InSubResourceIndex) const
		{
			return ((Flags & TransitionFlags::TRANSITION_FLAGS_NEEDS_RESOLVE) != TransitionFlags::TRANSITION_FLAGS_NONE) ? GetResource().GetResourceState(InSubResourceIndex) : Before;
		}

		const TransitionState& ResourceTransition::GetAfter() const
		{
			return After;
		}

		//////////////////////////////////////////////////////////////////////////
		// Position3D

		const Position3D Position3D::Zero;

		uint32_t Position3D::GetArraySlice(_In_ const ResourceDimension& InResourceDimension) const
		{
			switch (InResourceDimension)
			{
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D_ARRAY:
				return Y;

			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D_ARRAY:
				return Z;

			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE_ARRAY:
				return Z / 6;

			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_3D:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE:
			default:
				return 0;
			}
		}

		uint32_t Position3D::GetPlaneSlice(_In_ const ResourceDimension& InResourceDimension) const
		{
			(void)InResourceDimension;
			return 0;
		}
	}
}

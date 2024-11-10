#pragma once

namespace Eternal
{
	namespace Graphics
	{
		enum class TextureResourceUsage;
		enum class BufferResourceUsage;
		enum class ResourceDimension;

		class View;
		class Resource;

		//////////////////////////////////////////////////////////////////////////
		// Command type

		enum class CommandType
		{
			COMMAND_TYPE_GRAPHICS	= 0,
			COMMAND_TYPE_COMPUTE,
			COMMAND_TYPE_COPY,
			COMMAND_TYPE_COUNT
		};

		//////////////////////////////////////////////////////////////////////////
		// Resource transition

		enum class TransitionState
		{
			TRANSITION_UNDEFINED							= 0x0,
			TRANSITION_INDIRECT								= 0x1,
			TRANSITION_INDEX_READ							= 0x2,
			TRANSITION_VERTEX_BUFFER_READ					= 0x4,
			TRANSITION_CONSTANT_BUFFER_READ					= 0x8,
			TRANSITION_NON_PIXEL_SHADER_READ				= 0x10,
			TRANSITION_PIXEL_SHADER_READ					= 0x20,
			TRANSITION_SHADER_WRITE							= 0x40,
			TRANSITION_RENDER_TARGET						= 0x80,
			TRANSITION_DEPTH_STENCIL_READ					= 0x100,
			TRANSITION_DEPTH_STENCIL_WRITE					= 0x200,
			TRANSITION_COPY_READ							= 0x400,
			TRANSITION_COPY_WRITE							= 0x800,
			TRANSITION_RESOLVE_DESTINATION					= 0x1000,
			TRANSITION_RESOLVE_SOURCE						= 0x2000,
			TRANSITION_CPU_READ								= 0x4000,
			TRANSITION_CPU_WRITE							= 0x8000,
			TRANSITION_PRESENT								= 0x10000,
			TRANSITION_PREINITIALIZED						= 0x20000,
			TRANSITION_RAYTRACING_ACCELERATION_STRUCTURE	= 0x40000,

			TRANSITION_GENERIC_READ							= (TRANSITION_INDIRECT
															| TRANSITION_INDEX_READ
															| TRANSITION_VERTEX_BUFFER_READ
															| TRANSITION_CONSTANT_BUFFER_READ
															| TRANSITION_NON_PIXEL_SHADER_READ
															| TRANSITION_PIXEL_SHADER_READ
															| TRANSITION_COPY_READ)
		};
		
		enum class TransitionFlags
		{
			TRANSITION_FLAGS_NONE			= 0x0,
			TRANSITION_FLAGS_NEEDS_RESOLVE	= 0x1,
			TRANSITION_FLAGS_RESOURCE		= 0x2
		};

		enum class ResourcePlane : uint8_t
		{
			RESOURCE_PLANE_COLOR = 0x0,
			RESOURCE_PLANE_DEPTH = 0x0,
			RESOURCE_PLANE_STENCIL = 0x1
		};

		inline constexpr TransitionState operator|(_In_ const TransitionState& InLeftTransitionState, _In_ const TransitionState& InRightTransitionState)
		{
			return static_cast<TransitionState>(
				static_cast<uint32_t>(InLeftTransitionState) | static_cast<uint32_t>(InRightTransitionState)
			);
		}

		inline TransitionState operator&(_In_ const TransitionState& InLeftTransitionState, _In_ const TransitionState& InRightTransitionState)
		{
			return static_cast<TransitionState>(
				static_cast<uint32_t>(InLeftTransitionState) & static_cast<uint32_t>(InRightTransitionState)
			);
		}

		inline TransitionState operator~(_In_ const TransitionState& InTransitionState)
		{
			return static_cast<TransitionState>(
				~static_cast<uint32_t>(InTransitionState)
			);
		}

		inline TransitionState operator<<(_In_ const TransitionState& InTransitionState, _In_ uint32_t BitCount)
		{
			return static_cast<TransitionState>(
				static_cast<uint32_t>(InTransitionState) << BitCount
			);
		}

		inline TransitionState operator>>(_In_ const TransitionState& InTransitionState, _In_ uint32_t BitCount)
		{
			return static_cast<TransitionState>(
				static_cast<uint32_t>(InTransitionState) >> BitCount
			);
		}

		TransitionFlags operator|(const TransitionFlags& InLeftTransitionFlags, const TransitionFlags& InRightTransitionFlags);
		TransitionFlags operator&(const TransitionFlags& InLeftTransitionFlags, const TransitionFlags& InRightTransitionFlags);
		TransitionFlags& operator&=(TransitionFlags& InOutTransitionFlags, const TransitionFlags& InOtherTransitionFlags);
		TransitionFlags operator~(const TransitionFlags& InTransitionFlags);
		constexpr TextureResourceUsage operator|(_In_ const TextureResourceUsage& InLeftResourceUsage, _In_ const TextureResourceUsage& InRightResourceUsage)
		{
			return static_cast<TextureResourceUsage>(
				static_cast<uint32_t>(InLeftResourceUsage) | static_cast<uint32_t>(InRightResourceUsage)
			);
		}
		constexpr TextureResourceUsage operator&(_In_ const TextureResourceUsage& InLeftResourceUsage, _In_ const TextureResourceUsage& InRightResourceUsage)
		{
			return static_cast<TextureResourceUsage>(
				static_cast<uint32_t>(InLeftResourceUsage) & static_cast<uint32_t>(InRightResourceUsage)
			);
		}

		constexpr BufferResourceUsage operator|(_In_ const BufferResourceUsage& InLeftResourceUsage, _In_ const BufferResourceUsage& InRightResourceUsage)
		{
			return static_cast<BufferResourceUsage>(
				static_cast<uint32_t>(InLeftResourceUsage) | static_cast<uint32_t>(InRightResourceUsage)
			);
		}
		constexpr BufferResourceUsage operator&(_In_ const BufferResourceUsage& InLeftResourceUsage, _In_ const BufferResourceUsage& InRightResourceUsage)
		{
			return static_cast<BufferResourceUsage>(
				static_cast<uint32_t>(InLeftResourceUsage) & static_cast<uint32_t>(InRightResourceUsage)
			);
		}
		inline BufferResourceUsage operator>>(_In_ const BufferResourceUsage& InResourceUsage, _In_ uint32_t BitCount)
		{
			return static_cast<BufferResourceUsage>(
				static_cast<uint32_t>(InResourceUsage) >> BitCount
			);
		}
		inline BufferResourceUsage operator<<(_In_ const BufferResourceUsage& InResourceUsage, _In_ uint32_t BitCount)
		{
			return static_cast<BufferResourceUsage>(
				static_cast<uint32_t>(InResourceUsage) << BitCount
			);
		}

		inline bool OnlyHasFlags(_In_ const TransitionState& InTransitionState, _In_ const TransitionState& InFilterState)
		{
			TransitionState InTransitionStateIncludingInFilterState = (InTransitionState & InFilterState);
			TransitionState InTransitionStateExcludingInFilterState = (InTransitionState & ~InFilterState);

			return InTransitionStateIncludingInFilterState != TransitionState::TRANSITION_UNDEFINED
				&& InTransitionStateExcludingInFilterState == TransitionState::TRANSITION_UNDEFINED;
		}
		
		struct ResourceSubResource
		{
			static constexpr uint8_t InvalidMipLevel = static_cast<uint8_t>(~0);
			static constexpr uint64_t InvalidArraySlice = static_cast<uint16_t>(~0);

			ResourceSubResource(_In_ uint16_t InArraySlice, _In_ uint8_t InMipSlice);
			ResourceSubResource() {}

			bool IsWhole() const;
			bool ArraySizeNeedsResolve() const;
			bool MipLevelsNeedsResolve() const;

			uint16_t ArraySlice	= InvalidArraySlice;
			uint16_t ArraySize	= InvalidArraySlice;
			uint8_t MipSlice	= InvalidMipLevel;
			uint8_t MipLevels	= InvalidMipLevel;
			ResourcePlane Plane	= ResourcePlane::RESOURCE_PLANE_COLOR;
		};

		struct ResourceViewSubResource
		{
			ResourceSubResource	SubResource;
			View*				ResourceView = nullptr;
		};

		struct ResourceTransition
		{
			ResourceTransition(
				_In_ View* InView,
				_In_ const TransitionState& InBefore,
				_In_ const TransitionState& InAfter,
				_In_ const CommandType& InBeforeCommandType = CommandType::COMMAND_TYPE_GRAPHICS,
				_In_ const CommandType& InAfterCommandType = CommandType::COMMAND_TYPE_GRAPHICS
			)
				: ViewToTransition(InView)
				, Before(InBefore)
				, After(InAfter)
				, BeforeCommandType(InBeforeCommandType)
				, AfterCommandType(InAfterCommandType)
			{
			}

			ResourceTransition(
				_In_ View* InView,
				_In_ const TransitionState& InAfter,
				_In_ const CommandType& InBeforeCommandType = CommandType::COMMAND_TYPE_GRAPHICS,
				_In_ const CommandType& InAfterCommandType = CommandType::COMMAND_TYPE_GRAPHICS
			);

			ResourceTransition(
				_In_ Resource* InResource,
				_In_ const TransitionState& InAfter,
				_In_ const CommandType& InBeforeCommandType = CommandType::COMMAND_TYPE_GRAPHICS,
				_In_ const CommandType& InAfterCommandType = CommandType::COMMAND_TYPE_GRAPHICS
			);

			ResourceTransition(
				_In_ const ResourceViewSubResource& InViewSubResource,
				_In_ const TransitionState& InAfter,
				_In_ const CommandType& InBeforeCommandType = CommandType::COMMAND_TYPE_GRAPHICS,
				_In_ const CommandType& InAfterCommandType = CommandType::COMMAND_TYPE_GRAPHICS
			);

			ResourceTransition() {}

			const Resource& GetResource() const;
			Resource& GetResource();
			const TransitionState& GetBefore() const;
			const TransitionState& GetBefore(_In_ uint32_t InSubResourceIndex) const;
			const TransitionState& GetAfter() const;
			bool IsResource() const;

			ResourceSubResource SubResource;
			TransitionFlags Flags				= TransitionFlags::TRANSITION_FLAGS_NONE;
			union
			{
				void* Dummy						= nullptr;
				View* ViewToTransition;
				Resource* ResourceToTransition;
			};
			TransitionState Before				= TransitionState::TRANSITION_UNDEFINED;
			TransitionState After				= TransitionState::TRANSITION_UNDEFINED;
			CommandType		BeforeCommandType	= CommandType::COMMAND_TYPE_GRAPHICS;
			CommandType		AfterCommandType	= CommandType::COMMAND_TYPE_GRAPHICS;
		};

		//////////////////////////////////////////////////////////////////////////
		// Copy

		struct Position3D
		{
			static const Position3D Zero;

			uint32_t X	= 0;
			uint32_t Y	= 0;
			uint32_t Z	= 0;

			uint32_t GetArraySlice(_In_ const ResourceDimension& InResourceDimension) const;
			uint32_t GetPlaneSlice(_In_ const ResourceDimension& InResourceDimension) const;
		};

		struct Extent3D
		{
			Extent3D(_In_ uint32_t InWidth = 1, _In_ uint32_t InHeight = 1, _In_ uint32_t InDepth = 1)
				: Width(InWidth)
				, Height(InHeight)
				, Depth(InDepth)
			{
			}

			uint32_t Width	= 1;
			uint32_t Height	= 1;
			uint32_t Depth	= 1;
		};

		struct TextureRegion
		{
			TextureRegion(
				_In_ const Extent3D& InExtent,
				_In_ uint32_t InDestinationMIPIndex = 0,
				_In_ uint32_t InSourceMIPIndex = 0,
				_In_ const Position3D& InDestinationPosition = Position3D::Zero,
				_In_ const Position3D& InSourcePosition = Position3D::Zero
			)
				: Destination(InDestinationPosition)
				, DestinationMIPIndex(InDestinationMIPIndex)
				, Source(InSourcePosition)
				, SourceMIPIndex(InSourceMIPIndex)
				, Extent(InExtent)
			{
			}

			Position3D	Destination;
			uint32_t	DestinationMIPIndex	= 0;
			Position3D	Source;
			uint32_t	SourceMIPIndex		= 0;
			Extent3D	Extent;
		};

		struct BufferRegion
		{
			BufferRegion(
				_In_ uint32_t InSize,
				_In_ uint32_t InDestinationOffset = 0,
				_In_ uint32_t InSourceOffset = 0
			)
				: DestinationOffset(InDestinationOffset)
				, SourceOffset(InSourceOffset)
				, Size(InSize)
			{
			}

			uint32_t DestinationOffset	= 0;
			uint32_t SourceOffset		= 0;
			uint32_t Size				= 0;
		};

		struct BufferFromTextureRegion
		{
			BufferFromTextureRegion(
				_In_ uint32_t InDestinationSize,
				_In_ const Extent3D& InSourceExtent,
				_In_ const Position3D& InSourcePosition = Position3D::Zero,
				_In_ const uint32_t InDestinationOffset = 0,
				_In_ const uint32_t InSourceMIPIndex = 0
			)
				: Source(InSourcePosition)
				, SourceMIPIndex(InSourceMIPIndex)
				, SourceExtent(InSourceExtent)
				, DestinationOffset(InDestinationOffset)
				, DestinationSize(InDestinationSize)
			{
			}

			Position3D	Source;
			uint32_t	SourceMIPIndex		= 0;
			Extent3D	SourceExtent;
			uint32_t	DestinationOffset	= 0;
			uint32_t	DestinationSize		= 0;
		};

		struct TextureFromBufferRegion
		{
			TextureFromBufferRegion(
				_In_ const Extent3D& InDestinationExtent,
				_In_ uint32_t InSourceSize,
				_In_ const Position3D& InDestinationPosition = Position3D::Zero,
				_In_ const uint32_t InSourceOffset = 0,
				_In_ const uint32_t InDestinationMIPIndex = 0
			)
				: Destination(InDestinationPosition)
				, DestinationMIPIndex(InDestinationMIPIndex)
				, DestinationExtent(InDestinationExtent)
				, SourceOffset(InSourceOffset)
				, SourceSize(InSourceSize)
			{
			}

			Position3D	Destination;
			uint32_t	DestinationMIPIndex	= 0;
			Extent3D	DestinationExtent;
			uint32_t	SourceOffset		= 0;
			uint32_t	SourceSize			= 0;
		};

		union CopyRegion
		{
			CopyRegion(_In_ const BufferRegion& InBufferRegion)
				: Buffer(InBufferRegion)
			{
			}

			CopyRegion(_In_ const TextureRegion& InTextureRegion)
				: Texture(InTextureRegion)
			{
			}

			CopyRegion(_In_ const BufferFromTextureRegion& InBufferFromTextureRegion)
				: BufferFromTexture(InBufferFromTextureRegion)
			{
			}

			CopyRegion(_In_ const TextureFromBufferRegion& InTextureFromBuffer)
				: TextureFromBuffer(InTextureFromBuffer)
			{
			}

			TextureRegion Texture;
			BufferRegion Buffer;
			BufferFromTextureRegion BufferFromTexture;
			TextureFromBufferRegion TextureFromBuffer;
		};

		//////////////////////////////////////////////////////////////////////////
		// Index/Vertex buffer
		enum class IndexBufferType
		{
			INDEX_BUFFER_TYPE_16BITS = 0,
			INDEX_BUFFER_TYPE_32BITS
		};

		struct VertexBufferParameters
		{
			VertexBufferParameters(_In_ uint32_t InOffset)
				: Offset(InOffset)
			{
			}

			uint32_t Offset = 0;
		};

		//////////////////////////////////////////////////////////////////////////
		// Scissor
		struct ScissorRectangle
		{
			int32_t Left	= 0;
			int32_t Top		= 0;
			int32_t Right	= 0;
			int32_t Bottom	= 0;
		};
	}
}

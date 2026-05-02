#include "Graphics/Resource.hpp"

namespace Eternal
{
	namespace Graphics
	{
		constexpr uint32_t ClearValue::ComponentCount;
		constexpr uint8_t ClearValue::DefaultClearValueStencil;
		std::vector<ClearValue> ClearValue::ClearValues;

		//////////////////////////////////////////////////////////////////////////

		ClearValue ClearValue::DefaultClearValueZero()
		{
			return ClearValue(0.0f, 0.0f, 0.0f, 0.0f);
		}

		ClearValue ClearValue::DefaultClearValueOne()
		{
			return ClearValue(1.0f, 1.0f, 1.0f, 1.0f);
		}

		ClearValue ClearValue::DefaultClearValueDepth()
		{
			return ETERNAL_USE_REVERSED_Z ? ClearValue(0.0f) : ClearValue(1.0f);
		}

		void ClearValue::InitializeClearValues()
		{
			ClearValues.reserve(ClearValuesPoolSize);

			ClearValues.push_back(DefaultClearValueZero());
			ClearValues.push_back(DefaultClearValueOne());
		}

		uint8_t ClearValue::FindOrAddClearValue(_In_ const ClearValue& InClearValue)
		{
			for (uint8_t ClearValueIndex = 0; ClearValueIndex < ClearValues.size(); ++ClearValueIndex)
			{
				const ClearValue& CurrentClearValue = ClearValues[ClearValueIndex];
				if (CurrentClearValue == InClearValue)
					return ClearValueIndex;
			}
			ETERNAL_ASSERT(ClearValues.size() < ClearValuesPoolSize);
			ClearValues.push_back(InClearValue);
			return static_cast<uint8_t>(ClearValues.size() - 1);
		}

		bool ClearValue::operator==(const ClearValue& InOther) const
		{
			for (uint32_t ComponentIndex = 0; ComponentIndex < ComponentCount; ++ComponentIndex)
			{
				if (RawClearValue[ComponentIndex] != InOther.RawClearValue[ComponentIndex])
					return false;
			}
			return true;
		}

		bool ClearValue::operator!=(const ClearValue& InOther) const
		{
			return !(*this == InOther);
		}

		//////////////////////////////////////////////////////////////////////////

		constexpr ResourceType operator|(_In_ const ResourceType& InLeftResourceType, _In_ const ResourceType& InRightResourceType)
		{
			return static_cast<ResourceType>(
				static_cast<uint32_t>(InLeftResourceType) | static_cast<uint32_t>(InRightResourceType)
			);
		}

		ResourceType operator&(_In_ const ResourceType& InLeftResourceType, _In_ const ResourceType& InRightResourceType)
		{
			return static_cast<ResourceType>(
				static_cast<uint32_t>(InLeftResourceType) & static_cast<uint32_t>(InRightResourceType)
			);
		}

		//////////////////////////////////////////////////////////////////////////

		std::unordered_set<Resource*> Resource::ResourceHistory;

		Resource::Resource(_In_ const ResourceCreateInformation& InResourceCreateInformation, _In_ const ResourceType& InResourceType)
			: _ResourceType(InResourceType)
			, _ResourceCreateInformation(InResourceCreateInformation)
		{
			const TextureCreateInformation& InTextureInformation = InResourceCreateInformation.TextureInformation;

			if ((GetResourceType() & ResourceType::RESOURCE_TYPE_TEXTURE) == ResourceType::RESOURCE_TYPE_TEXTURE)
			{
				uint32_t SubResourcesCount = GetMIPLevels() * InTextureInformation.DepthOrArraySize;
				GetSubResourceStates().resize(SubResourcesCount);
				for (uint32_t SubResourceIndex = 0; SubResourceIndex < SubResourcesCount; ++SubResourceIndex)
					GetSubResourceStates()[SubResourceIndex] = GetResourceState();
			}

			ETERNAL_ASSERT(_ResourceCreateInformation.Name.size() > 0);
			ResourceHistory.insert(this);
		}

		Resource::~Resource()
		{
			ResourceHistory.erase(this);
		}

		ResourceType Resource::GetResourceType() const
		{
			static constexpr ResourceType Texture_Buffer_Type = (ResourceType::RESOURCE_TYPE_BUFFER | ResourceType::RESOURCE_TYPE_TEXTURE);
			ResourceType FundamentalType = _ResourceType & Texture_Buffer_Type;
			ETERNAL_ASSERT(FundamentalType != Texture_Buffer_Type); // Cannot be both
			return FundamentalType;
		} 

		const ClearValue& Resource::GetClearValue() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			return ClearValue::ClearValues[_ResourceCreateInformation.TextureInformation.ClearValueIndex];
		}

		void Resource::SetResourceState(_In_ const TransitionState& InTransitionState)
		{
			_ResourceCreateInformation.ResourceState = InTransitionState;
			for (uint32_t SubResource = 0; SubResource < _SubResourceStates.size(); ++SubResource)
				_SubResourceStates[SubResource] = InTransitionState;
		}

		void Resource::SetSubResourceState(_In_ uint32_t InSubResourceIndex, _In_ const TransitionState& InTransitionState)
		{
			ETERNAL_ASSERT(InSubResourceIndex < _SubResourceStates.size());
			_SubResourceStates[InSubResourceIndex] = InTransitionState;
			_IsSubResourceStatesDirty = true;
		}

		const TransitionState& Resource::GetResourceState(_In_ uint32_t InSubResourceIndex) const
		{
			ETERNAL_ASSERT(InSubResourceIndex < _SubResourceStates.size());
			return _SubResourceStates[InSubResourceIndex];
		}

		uint32_t Resource::GetSubResourceStatesCount() const
		{
			return _SubResourceStates.size();
		}

		const ResourceDimension& Resource::GetResourceDimension() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			return _ResourceCreateInformation.TextureInformation.Dimension;
		}

		uint32_t Resource::GetWidth() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			return _ResourceCreateInformation.TextureInformation.Width;
		}

		uint32_t Resource::GetHeight() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			return _ResourceCreateInformation.TextureInformation.Height;
		}

		uint32_t Resource::GetDepth() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			switch (GetResourceDimension())
			{
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE_ARRAY:
				return 6;
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_3D:
				return _ResourceCreateInformation.TextureInformation.DepthOrArraySize;
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D_ARRAY:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D_ARRAY:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D:
			default:
				return 1;
			}
		}

		uint32_t Resource::GetMIPLevels() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			return _ResourceCreateInformation.TextureInformation.MIPLevels;
		}

		uint32_t Resource::GetArraySize() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			switch (GetResourceDimension())
			{
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D_ARRAY:
				return _ResourceCreateInformation.TextureInformation.Height;
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D_ARRAY:
				return _ResourceCreateInformation.TextureInformation.DepthOrArraySize;
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE_ARRAY:
				return _ResourceCreateInformation.TextureInformation.DepthOrArraySize / 6;
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_3D:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE:
			default:
				return 1;
				break;
			}
		}

		uint32_t Resource::GetDepthOrArraySize() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE);
			switch (GetResourceDimension())
			{
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D_ARRAY:
				return _ResourceCreateInformation.TextureInformation.Height;
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D_ARRAY:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE_ARRAY:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_3D:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE:
				return _ResourceCreateInformation.TextureInformation.DepthOrArraySize;
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D:
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D:
			default:
				return 1;
				break;
			}
		}

		uint32_t Resource::GetBufferSize() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_BUFFER);
			return _ResourceCreateInformation.BufferInformation.Stride * _ResourceCreateInformation.BufferInformation.ElementCount;
		}

		uint32_t Resource::GetBufferStride() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_BUFFER);
			return _ResourceCreateInformation.BufferInformation.Stride;
		}

		uint32_t Resource::GetElementCount() const
		{
			ETERNAL_ASSERT(GetResourceType() == ResourceType::RESOURCE_TYPE_BUFFER);
			return _ResourceCreateInformation.BufferInformation.ElementCount;
		}

		const Format& Resource::GetFormat() const
		{
			return GetResourceType() == ResourceType::RESOURCE_TYPE_TEXTURE ? _ResourceCreateInformation.TextureInformation.ResourceFormat : _ResourceCreateInformation.BufferInformation.ResourceFormat;
		}

		const std::string& Resource::GetResourceName() const
		{
			return _ResourceCreateInformation.Name;
		}

		bool Resource::IsBackBuffer() const
		{
			return GetRawResourceType() == ResourceType::RESOURCE_TYPE_BACK_BUFFER;
		}
	}
}

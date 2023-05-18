#include "Graphics/Texture.hpp"
#include "Graphics/Resource.hpp"
#include "Graphics/View.hpp"
#include "Graphics/ResourceFactory.hpp"
#include "Graphics/ViewFactory.hpp"

namespace Eternal
{
	namespace Graphics
	{
		static constexpr ViewShaderResourceType VIEW_SHADER_RESOURCE_TYPES[] =
		{
			ViewShaderResourceType::VIEW_SHADER_RESOURCE_UNKNOWN,
			ViewShaderResourceType::VIEW_SHADER_RESOURCE_BUFFER,
			ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_1D,
			ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_1D_ARRAY,
			ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_2D,
			ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_2D_ARRAY,
			ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_3D,
			ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_CUBE,
			ViewShaderResourceType::VIEW_SHADER_RESOURCE_TEXTURE_CUBE_ARRAY
		};
		ETERNAL_STATIC_ASSERT(ETERNAL_ARRAYSIZE(VIEW_SHADER_RESOURCE_TYPES) == static_cast<uint32_t>(ResourceDimension::RESOURCE_DIMENSION_COUNT), "Mismatch between resource dimension and view shader resource types");

		ViewShaderResourceType ConvertResourceDimensionToViewShaderResourceType(_In_ const ResourceDimension& InResourceDimension)
		{
			return VIEW_SHADER_RESOURCE_TYPES[static_cast<uint32_t>(InResourceDimension)];
		}

		Texture::Texture(_In_ GraphicsContext& InContext, _In_ const TextureResourceCreateInformation& InTextureResourceCreateInformation)
		{
			_Texture = CreateTexture(InTextureResourceCreateInformation);
			
			ViewMetaData MetaData;
			switch (InTextureResourceCreateInformation.TextureInformation.Dimension)
			{
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D:
			{
				MetaData.ShaderResourceViewTexture1D.MipLevels			= InTextureResourceCreateInformation.TextureInformation.MIPLevels;
			} break;
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_1D_ARRAY:
			{
				MetaData.ShaderResourceViewTexture1DArray.MipLevels		= InTextureResourceCreateInformation.TextureInformation.MIPLevels;
				MetaData.ShaderResourceViewTexture1DArray.ArraySize		= InTextureResourceCreateInformation.TextureInformation.DepthOrArraySize;
			} break;
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D:
			{
				MetaData.ShaderResourceViewTexture2D.MipLevels			= InTextureResourceCreateInformation.TextureInformation.MIPLevels;
			} break;
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_2D_ARRAY:
			{
				MetaData.ShaderResourceViewTexture2DArray.MipLevels		= InTextureResourceCreateInformation.TextureInformation.MIPLevels;
				MetaData.ShaderResourceViewTexture2DArray.ArraySize		= InTextureResourceCreateInformation.TextureInformation.DepthOrArraySize;
			} break;
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_3D:
			{
				MetaData.ShaderResourceViewTexture3D.MipLevels			= InTextureResourceCreateInformation.TextureInformation.MIPLevels;
			} break;
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE:
			{
				MetaData.ShaderResourceViewTextureCube.MipLevels		= InTextureResourceCreateInformation.TextureInformation.MIPLevels;
			} break;
			case ResourceDimension::RESOURCE_DIMENSION_TEXTURE_CUBE_ARRAY:
			{
				MetaData.ShaderResourceViewTextureCubeArray.MipLevels	= InTextureResourceCreateInformation.TextureInformation.MIPLevels;
				MetaData.ShaderResourceViewTextureCubeArray.NumCubes	= InTextureResourceCreateInformation.TextureInformation.DepthOrArraySize / 6;
			} break;
			}
			ShaderResourceViewCreateInformation TextureShaderResourceViewCreateInformation(
				InContext,
				_Texture,
				MetaData,
				ConvertDepthStencilFormatToFormat(InTextureResourceCreateInformation.TextureInformation.ResourceFormat),
				ConvertResourceDimensionToViewShaderResourceType(InTextureResourceCreateInformation.TextureInformation.Dimension)
			);
			_ShaderResourceView = CreateShaderResourceView(TextureShaderResourceViewCreateInformation);
		}

		Texture::~Texture()
		{
			DestroyView(_ShaderResourceView);
			DestroyResource(_Texture);
		}
	}
}

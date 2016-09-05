#ifndef _D3D11_TEXTURE_HPP_
#define _D3D11_TEXTURE_HPP_

#include <d3d11.h>
#include "Graphics/Texture.hpp"
#include "d3d11/D3D11Resource.hpp"
#include "d3d11/ID3D11ShaderResource.hpp"

struct ID3D11ShaderResourceView;
struct ID3D11Texture2D;

namespace Eternal
{
	namespace Graphics
	{
		struct D3D11TextureFormatInformation
		{
			DXGI_FORMAT D3D11Format;
			size_t Size;
		};

		class D3D11Texture
			: public Texture
			, public D3D11Resource
			, public ID3D11ShaderResource
		{
		public:
			D3D11Texture(_In_ const Format& FormatObj, _In_ const Usage& UsageObj, _In_ const CPUAccess& CPUMode, _In_ uint32_t Width, _In_ uint32_t Height, _In_ const void* Data = nullptr);
			D3D11Texture(_In_ ID3D11Texture2D* TextureObj);
			virtual ~D3D11Texture();
			inline virtual ID3D11ShaderResourceView* GetD3D11ShaderResourceView() override
			{
				return _ShaderResourceView;
			}

			static const D3D11TextureFormatInformation& GetD3D11TextureFormatInformation(const Format& FormatObj);

		protected:
			D3D11Texture();
			ID3D11ShaderResourceView* _ShaderResourceView = nullptr;
		};
	}
}

#endif

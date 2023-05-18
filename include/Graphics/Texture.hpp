#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class GraphicsContext;
		class Resource;
		class View;
		struct TextureResourceCreateInformation;

		class Texture
		{
		public:

			Texture(_In_ GraphicsContext& InContext, _In_ const TextureResourceCreateInformation& InTextureResourceCreateInformation);
			~Texture();

			Resource& GetTexture() { return *_Texture; }
			View* GetShaderResourceView() { return _ShaderResourceView; }

		protected:
			Resource* _Texture			= nullptr;

		private:
			View* _ShaderResourceView	= nullptr;
		};
	}
}

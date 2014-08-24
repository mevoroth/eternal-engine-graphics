#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Texture
		{
		private:
			int _width;
			int _height;
		public:
			Texture(_In_ int width = 1, _In_ int height = 1);
			inline int GetWidth() const;
			inline int GetHeight() const;
			void SetWidth(_In_ int width);
			void SetHeight(_In_ int height);
		};
	}
}

#endif

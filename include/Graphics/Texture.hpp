#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Texture
		{
		public:
			enum FilterMode
			{
				POINT,

			};
			enum AddressMode
			{
				WRAP,
				MIRROR,
				CLAMP
			};
		private:
			int _Width;
			int _Height;
			AddressMode U;
			AddressMode V;

		public:
			Texture(_In_ int Width = 1, _In_ int Height = 1);
			inline int GetWidth() const;
			inline int GetHeight() const;
			void SetWidth(_In_ int Width);
			void SetHeight(_In_ int Height);


		};
	}
}

#endif

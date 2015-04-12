#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include <cassert>
using namespace std;

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
				LINEAR
			};
			enum AddressMode
			{
				WRAP = 0,
				MIRROR = 1,
				CLAMP = 2,
				ADRESSMODE_COUNT
			};
		private:
			int _Width;
			int _Height;
			AddressMode _U = WRAP;
			AddressMode _V = WRAP;

		public:
			Texture(_In_ int Width = 1, _In_ int Height = 1);
			inline int GetWidth() const
			{
				return _Width;
			}
			inline int GetHeight() const
			{
				return _Height;
			}
			inline void SetWidth(_In_ int Width)
			{
				assert(Width > 0);
				_Width = Width;
			}
			inline void SetHeight(_In_ int Height)
			{
				assert(Height > 0);
				_Height = Height;
			}
			inline AddressMode GetUAddressMode() const
			{
				return _U;
			}
			inline AddressMode GetVAddressMode() const
			{
				return _V;
			}
		};
	}
}

#endif

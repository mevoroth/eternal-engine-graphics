#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include "Macros/Macros.hpp"
using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		class Texture
		{
		public:

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
				ETERNAL_ASSERT(Width > 0);
				_Width = Width;
			}
			inline void SetHeight(_In_ int Height)
			{
				ETERNAL_ASSERT(Height > 0);
				_Height = Height;
			}

		private:
			int _Width;
			int _Height;
		};
	}
}

#endif

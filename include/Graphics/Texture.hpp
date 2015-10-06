#ifndef _TEXTURE_HPP_
#define _TEXTURE_HPP_

#include <cstdint>

#include "Macros/Macros.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Texture
		{
		public:
			enum Format
			{
				BGRA8888 = 0
			};

			Texture(_In_ const Format& FormatObj = BGRA8888, _In_ uint32_t Width = 1, _In_ uint32_t Height = 1);
			virtual ~Texture() {}
			inline uint32_t GetWidth() const
			{
				return _Width;
			}
			inline uint32_t GetHeight() const
			{
				return _Height;
			}

			inline Format GetFormat() const
			{
				return _Format;
			}
			inline void SetWidth(_In_ uint32_t Width)
			{
				ETERNAL_ASSERT(Width > 0);
				_Width = Width;
			}
			inline void SetHeight(_In_ uint32_t Height)
			{
				ETERNAL_ASSERT(Height > 0);
				_Height = Height;
			}

		private:
			uint32_t _Width;
			uint32_t _Height;
			Format _Format;
		};
	}
}

#endif

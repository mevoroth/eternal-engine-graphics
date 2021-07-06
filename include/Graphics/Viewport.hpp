#pragma once

namespace Eternal
{
	namespace Graphics
	{
		class Viewport
		{
		public:
			Viewport(_In_ int32_t InX, _In_ int32_t InY, _In_ int32_t InWidth, _In_ int32_t InHeight);

			inline int32_t GetX() const { return _X; }
			inline int32_t GetY() const { return _Y; }
			inline int32_t GetWidth() const { return _Width; }
			inline int32_t GetHeight() const { return _Height; }

		private:
			int32_t _X		= 0;
			int32_t _Y		= 0;
			int32_t _Width	= 0;
			int32_t _Height	= 0;
		};
	}
}

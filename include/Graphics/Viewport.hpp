#ifndef _VIEWPORT_HPP_
#define _VIEWPORT_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Viewport
		{
		public:
			Viewport(_In_ int X, _In_ int Y, _In_ int Width, _In_ int Height);

			inline int GetX() const
			{
				return _X;
			}
			inline int GetY() const
			{
				return _Y;
			}
			inline int GetWidth() const
			{
				return _Width;
			}
			inline int GetHeight() const
			{
				return _Height;
			}

		private:
			int _X;
			int _Y;
			int _Width;
			int _Height;
		};
	}
}

#endif

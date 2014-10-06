#ifndef _VIEWPORT_HPP_
#define _VIEWPORT_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Viewport
		{
		private:
			int _x;
			int _y;
			int _width;
			int _height;
		protected:
			inline int X() const
			{
				return _x;
			}
			inline int Y() const
			{
				return _y;
			}
			inline int Width() const
			{
				return _width;
			}
			inline int Height() const
			{
				return _height;
			}
		public:
			Viewport(_In_ int x, _In_ int y, _In_ int width, _In_ int height);
			virtual void Apply() = 0;
		};
	}
}

#endif

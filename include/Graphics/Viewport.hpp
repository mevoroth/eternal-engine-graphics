#ifndef _VIEWPORT_HPP_
#define _VIEWPORT_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Context;

		class Viewport
		{
		public:
			Viewport(_In_ int X, _In_ int Y, _In_ int Width, _In_ int Height);
			//virtual void Apply(Context* DrawContext) = 0;

			inline int X() const
			{
				return _X;
			}
			inline int Y() const
			{
				return _Y;
			}
			inline int Width() const
			{
				return _Width;
			}
			inline int Height() const
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

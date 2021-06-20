#ifndef _VIEW_HPP_
#define _VIEW_HPP_

namespace Eternal
{
	namespace Graphics
	{
		enum Format;
		class RenderTarget;

		enum TextureView
		{
			TEXTURE_VIEW_TYPE_1D,
			TEXTURE_VIEW_TYPE_1DARRAY,
			TEXTURE_VIEW_TYPE_2D,
			TEXTURE_VIEW_TYPE_2DARRAY,
			TEXTURE_VIEW_TYPE_3D,
			TEXTURE_VIEW_TYPE_CUBE,
			TEXTURE_VIEW_TYPE_CUBEARRAY,
		};

		class View
		{
		public:
			View(_In_ const Format& FormatObj);
			virtual ~View() {}
			//virtual RenderTarget& GetAsRenderTarget() = 0;
			const Format& GetFormat() const { return _Format; }

		private:
			Format	_Format;
		};
	}
}

#endif

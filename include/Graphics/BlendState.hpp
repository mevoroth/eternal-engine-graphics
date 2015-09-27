#ifndef _BLEND_STATE_HPP_
#define _BLEND_STATE_HPP_

namespace Eternal
{
	namespace Graphics
	{
		class Context;

		class BlendState
		{
		public:
			enum Blend
			{
				ZERO			= 0,
				ONE				= 1,

				// SRC
				SRC_COLOR		= 2,
				INV_SRC_COLOR	= 3,
				SRC_ALPHA		= 4,
				INV_SRC_ALPHA	= 5,

				// DEST
				DEST_ALPHA		= 6,
				INV_DEST_ALPHA	= 7,
				DEST_COLOR		= 8,
				INV_DEST_COLOR	= 9,

				SRC_ALPHA_SAT	= 10,

				// COLOR
				COLOR			= 11,
				COLOR_ALPHA		= 12
			};
			enum BlendOp
			{
				OP_ADD		= 0,
				OP_SUB		= 1,
				OP_REV_SUB	= 2,
				OP_MIN		= 3,
				OP_MAX		= 4
			};
		private:
			Blend _Src;
			Blend _Dest;
			BlendOp _BlendOp;
			Blend _SrcAlpha;
			Blend _DestAlpha;
			BlendOp _BlendAlphaOp;

		public:
			BlendState(_In_ const Blend& Src, _In_ const Blend& Dest, _In_ const BlendOp& BlendOpCol, _In_ const Blend& SrcAlpha, _In_ const Blend& DestAlpha, _In_ const BlendOp& BlendAlphaOp);
			virtual ~BlendState() {}
			virtual void Apply(Context* DrawContext) = 0;
		};
	}
}

#endif

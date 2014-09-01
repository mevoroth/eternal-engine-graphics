#ifndef _BLEND_STATE_HPP_
#define _BLEND_STATE_HPP_

namespace Eternal
{
	namespace Graphics
	{
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
			Blend _src;
			Blend _dest;
			BlendOp _blendOp;
			Blend _srcAlpha;
			Blend _destAlpha;
			Blend _blendAlphaOp;
		public:
			BlendState(_In_ const Blend& src, _In_ const Blend& dest, _In_ const BlendOp& blendOp, _In_ const Blend& srcAlpha, _In_ const Blend& destAlpha, _In_ const Blend& blendAlphaOp);
			virtual void Apply() = 0;
		};
	}
}

#endif

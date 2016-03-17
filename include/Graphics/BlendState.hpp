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

		public:
			BlendState() {}
			BlendState(_In_ const Blend& Src, _In_ const Blend& Dest, _In_ const BlendOp& BlendOpCol, _In_ const Blend& SrcAlpha, _In_ const Blend& DestAlpha, _In_ const BlendOp& BlendAlphaOp);
			virtual ~BlendState() {}
			//virtual void Apply(Context* DrawContext) = 0;

			inline bool IsEnabled() const { return _Enabled; }
			inline const Blend& GetSrc() const { return _Src; }
			inline const Blend& GetDest() const { return _Dest; }
			inline const BlendOp& GetBlendOp() const { return _BlendOp; }
			inline const Blend& GetSrcAlpha() const { return _SrcAlpha; }
			inline const Blend& GetDestAlpha() const { return _DestAlpha; }
			inline const BlendOp& GetBlendAlphaOp() const { return _BlendAlphaOp; }

		private:
			bool _Enabled			= false;
			Blend _Src				= SRC_ALPHA;
			Blend _Dest				= INV_SRC_ALPHA;
			BlendOp _BlendOp		= OP_ADD;
			Blend _SrcAlpha			= SRC_ALPHA;
			Blend _DestAlpha		= INV_SRC_ALPHA;
			BlendOp _BlendAlphaOp	= OP_ADD;
		};
	}
}

#endif

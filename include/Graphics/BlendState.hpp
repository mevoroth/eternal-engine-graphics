#ifndef _BLEND_STATE_HPP_
#define _BLEND_STATE_HPP_

namespace Eternal
{
	namespace Graphics
	{
		enum class Blend
		{
			BLEND_ZERO						= 0,
			BLEND_ONE						= 1,

			// SRC
			BLEND_SRC_COLOR					= 2,
			BLEND_INV_SRC_COLOR				= 3,
			BLEND_SRC_ALPHA					= 4,
			BLEND_INV_SRC_ALPHA				= 5,

			// DEST
			BLEND_DEST_ALPHA				= 6,
			BLEND_INV_DEST_ALPHA			= 7,
			BLEND_DEST_COLOR				= 8,
			BLEND_INV_DEST_COLOR			= 9,

			BLEND_SRC_ALPHA_SAT				= 10,

			// BLEND FACTOR
			BLEND_BLEND_COLOR_FACTOR		= 11,
			BLEND_INV_BLEND_COLOR_FACTOR	= 12,
			BLEND_BLEND_ALPHA_FACTOR		= 13,
			BLEND_INV_BLEND_ALPHA_FACTOR	= 14,
			
			BLEND_SRC1_COLOR				= 15,
			BLEND_INV_SRC1_COLOR			= 16,
			BLEND_SRC1_ALPHA				= 17,
			BLEND_SRC1_INV_ALPHA			= 18,
			BLEND_COUNT
		};

		enum class BlendOp
		{
			BLEND_OP_ADD		= 0,
			BLEND_OP_SUB		= 1,
			BLEND_OP_REV_SUB	= 2,
			BLEND_OP_MIN		= 3,
			BLEND_OP_MAX		= 4,
			BLEND_OP_COUNT
		};

		enum class LogicOp
		{
			LOGIC_OP_CLEAR			= 0,
			LOGIC_OP_SET			= 1,
			LOGIC_OP_COPY			= 2,
			LOGIC_OP_COPY_INVERTED	= 3,
			LOGIC_OP_NOOP			= 4,
			LOGIC_OP_INVERT			= 5,
			LOGIC_OP_AND			= 6,
			LOGIC_OP_NAND			= 7,
			LOGIC_OP_OR				= 8,
			LOGIC_OP_NOR			= 9,
			LOGIC_OP_XOR			= 10,
			LOGIC_OP_EQUIV			= 11,
			LOGIC_OP_AND_REVERSE	= 12,
			LOGIC_OP_AND_INVERTED	= 13,
			LOGIC_OP_OR_REVERSE		= 14,
			LOGIC_OP_OR_INVERTED	= 15,
			LOGIC_OP_COUNT
		};

		enum class BlendChannel
		{
			BLEND_CHANNEL_RED	= 0x1,
			BLEND_CHANNEL_GREEN	= 0x2,
			BLEND_CHANNEL_BLUE	= 0x4,
			BLEND_CHANNEL_ALPHA	= 0x8,
			BLEND_CHANNEL_ALL	= (BLEND_CHANNEL_RED | BLEND_CHANNEL_GREEN | BLEND_CHANNEL_BLUE | BLEND_CHANNEL_ALPHA)
		};

		class BlendState
		{
		public:
			BlendState() {}
			BlendState(_In_ const Blend& Src, _In_ const Blend& Dest, _In_ const BlendOp& BlendColorOp, _In_ const Blend& SrcAlpha, _In_ const Blend& DestAlpha, _In_ const BlendOp& BlendAlphaOp, _In_ const BlendChannel& BlendChannelObj);
			virtual ~BlendState() {}

			inline bool IsEnabled() const						{ return _Enabled; }
			inline const Blend& GetSrc() const					{ return _Src; }
			inline const Blend& GetDest() const					{ return _Dest; }
			inline const BlendOp& GetBlendOp() const			{ return _BlendOp; }
			inline const Blend& GetSrcAlpha() const				{ return _SrcAlpha; }
			inline const Blend& GetDestAlpha() const			{ return _DestAlpha; }
			inline const BlendOp& GetBlendAlphaOp() const		{ return _BlendAlphaOp; }
			inline const BlendChannel& GetBlendChannel() const	{ return _BlendChannel; }

		private:
			bool			_Enabled		= false;
			Blend			_Src			= Blend::BLEND_ONE;
			Blend			_Dest			= Blend::BLEND_ZERO;
			BlendOp			_BlendOp		= BlendOp::BLEND_OP_ADD;
			Blend			_SrcAlpha		= Blend::BLEND_ONE;
			Blend			_DestAlpha		= Blend::BLEND_ZERO;
			BlendOp			_BlendAlphaOp	= BlendOp::BLEND_OP_ADD;
			BlendChannel	_BlendChannel	= BlendChannel::BLEND_CHANNEL_ALL;
		};

		class LogicBlend
		{
		public:
			LogicBlend() {}
			LogicBlend(_In_ const LogicOp& LogicOpObj);

			bool IsEnabled() const { return _Enabled; }
			const LogicOp& GetLogicOp() const { return _LogicOp; }

		private:
			bool	_Enabled	= false;
			LogicOp	_LogicOp	= LogicOp::LOGIC_OP_NOOP;
		};

		extern const BlendState BlendStateNone;
		extern const BlendState BlendStateAlphaBlend;
		extern const LogicBlend LogicBlendNone;
	}
}

#endif

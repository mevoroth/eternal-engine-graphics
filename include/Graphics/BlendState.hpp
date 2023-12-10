#pragma once

namespace Eternal
{
	namespace Graphics
	{
		enum class Blend
		{
			BLEND_ZERO							= 0,
			BLEND_ONE							= 1,

			// SOURCE
			BLEND_SOURCE_COLOR					= 2,
			BLEND_INVERSE_SOURCE_COLOR			= 3,
			BLEND_SOURCE_ALPHA					= 4,
			BLEND_INVERSE_SOURCE_ALPHA			= 5,

			// DESTINATION
			BLEND_DESTINATION_ALPHA				= 6,
			BLEND_INVERSE_DESTINATION_ALPHA		= 7,
			BLEND_DESTINATION_COLOR				= 8,
			BLEND_INVERSE_DESTINATION_COLOR		= 9,

			BLEND_SOURCE_ALPHA_SATURATE			= 10,

			// BLEND FACTOR
			BLEND_BLEND_COLOR_FACTOR			= 11,
			BLEND_INVERSE_BLEND_COLOR_FACTOR	= 12,
			BLEND_BLEND_ALPHA_FACTOR			= 13,
			BLEND_INVERSE_BLEND_ALPHA_FACTOR	= 14,
			
			//BLEND_SOURCE1_COLOR					= 15,
			//BLEND_INVERSE_SOURCE1_COLOR			= 16,
			//BLEND_SOURCE1_ALPHA					= 17,
			//BLEND_SOURCE1_INVERSE_ALPHA			= 18,
			BLEND_COUNT
		};

		enum class BlendOperator
		{
			BLEND_OPERATOR_ADD				= 0,
			BLEND_OPERATOR_SUBTRACT			= 1,
			BLEND_OPERATOR_REVERSE_SUBTRACT	= 2,
			BLEND_OPERATOR_MIN				= 3,
			BLEND_OPERATOR_MAX				= 4,
			BLEND_OPERATOR_COUNT
		};

		enum class LogicOperator
		{
			LOGIC_OPERATOR_CLEAR			= 0,
			LOGIC_OPERATOR_SET				= 1,
			LOGIC_OPERATOR_COPY				= 2,
			LOGIC_OPERATOR_COPY_INVERTED	= 3,
			LOGIC_OPERATOR_NOOP				= 4,
			LOGIC_OPERATOR_INVERT			= 5,
			LOGIC_OPERATOR_AND				= 6,
			LOGIC_OPERATOR_NAND				= 7,
			LOGIC_OPERATOR_OR				= 8,
			LOGIC_OPERATOR_NOR				= 9,
			LOGIC_OPERATOR_XOR				= 10,
			LOGIC_OPERATOR_EQUIV			= 11,
			LOGIC_OPERATOR_AND_REVERSE		= 12,
			LOGIC_OPERATOR_AND_INVERTED		= 13,
			LOGIC_OPERATOR_OR_REVERSE		= 14,
			LOGIC_OPERATOR_OR_INVERTED		= 15,
			LOGIC_OPERATOR_COUNT
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
			BlendState(
				_In_ const Blend& InSource,
				_In_ const Blend& InDestination,
				_In_ const BlendOperator& InBlendColorOperator,
				_In_ const Blend& InSourceAlpha,
				_In_ const Blend& InDestinationAlpha,
				_In_ const BlendOperator& InBlendAlphaOperator,
				_In_ const BlendChannel& InBlendChannel = BlendChannel::BLEND_CHANNEL_ALL
			);

			inline bool IsEnabled() const								{ return _Enabled; }
			inline const Blend& GetSource() const						{ return _Source; }
			inline const Blend& GetDestination() const					{ return _Destination; }
			inline const BlendOperator& GetBlendOperator() const		{ return _BlendOperator; }
			inline const Blend& GetSourceAlpha() const					{ return _SourceAlpha; }
			inline const Blend& GetDestinationAlpha() const				{ return _DestinationAlpha; }
			inline const BlendOperator& GetBlendAlphaOperator() const	{ return _BlendAlphaOperator; }
			inline const BlendChannel& GetBlendChannel() const			{ return _BlendChannel; }

		private:
			bool			_Enabled			= false;
			Blend			_Source				= Blend::BLEND_ONE;
			Blend			_Destination		= Blend::BLEND_ZERO;
			BlendOperator	_BlendOperator		= BlendOperator::BLEND_OPERATOR_ADD;
			Blend			_SourceAlpha		= Blend::BLEND_ONE;
			Blend			_DestinationAlpha	= Blend::BLEND_ZERO;
			BlendOperator	_BlendAlphaOperator	= BlendOperator::BLEND_OPERATOR_ADD;
			BlendChannel	_BlendChannel		= BlendChannel::BLEND_CHANNEL_ALL;
		};

		class LogicBlend
		{
		public:
			LogicBlend() {}
			LogicBlend(_In_ const LogicOperator& InLogicOperator);

			bool IsEnabled() const { return _Enabled; }
			const LogicOperator& GetLogicOperator() const { return _LogicOp; }

		private:
			bool			_Enabled	= false;
			LogicOperator	_LogicOp	= LogicOperator::LOGIC_OPERATOR_NOOP;
		};

		extern const BlendState BlendStateNone;
		extern const BlendState BlendStateAlphaBlend;
		extern const BlendState BlendStateAdditive;
		extern const LogicBlend LogicBlendNone;
	}
}

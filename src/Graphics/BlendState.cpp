#include "Graphics/BlendState.hpp"

using namespace Eternal::Graphics;

namespace Eternal
{
	namespace Graphics
	{
		const LogicBlend LogicBlendNone;
		const BlendState BlendStateNone;
		const BlendState BlendStateAlphaBlend(
			Blend::BLEND_SOURCE_ALPHA,
			Blend::BLEND_INVERSE_SOURCE_ALPHA,
			BlendOperator::BLEND_OPERATOR_ADD,
			Blend::BLEND_SOURCE_ALPHA,
			Blend::BLEND_INVERSE_SOURCE_ALPHA,
			BlendOperator::BLEND_OPERATOR_ADD,
			BlendChannel::BLEND_CHANNEL_ALL
		);
	}
}

BlendState::BlendState(
	_In_ const Blend& InSource,
	_In_ const Blend& InDestination,
	_In_ const BlendOperator& InBlendColorOperator,
	_In_ const Blend& InSourceAlpha,
	_In_ const Blend& InDestinationAlpha,
	_In_ const BlendOperator& InBlendAlphaOperator,
	_In_ const BlendChannel& InBlendChannel
)
	: _Enabled(true)
	, _Source(InSource)
	, _Destination(InDestination)
	, _BlendOperator(InBlendColorOperator)
	, _SourceAlpha(InSourceAlpha)
	, _DestinationAlpha(InDestinationAlpha)
	, _BlendAlphaOperator(InBlendAlphaOperator)
	, _BlendChannel(InBlendChannel)
{
}

LogicBlend::LogicBlend(_In_ const LogicOperator& InLogicOperator)
	: _Enabled(true)
	, _LogicOp(InLogicOperator)
{
}

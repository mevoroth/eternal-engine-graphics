#pragma once

namespace Eternal
{
	namespace Graphics
	{
		enum class ComparisonFunction
		{
			COMPARISON_FUNCTION_NEVER			= 0,
			COMPARISON_FUNCTION_LESS,
			COMPARISON_FUNCTION_EQUAL,
			COMPARISON_FUNCTION_LESS_EQUAL,
			COMPARISON_FUNCTION_GREATER,
			COMPARISON_FUNCTION_NOT_EQUAL,
			COMPARISON_FUNCTION_GREATER_EQUAL,
			COMPARISON_FUNCTION_ALWAYS
		};
	}
}
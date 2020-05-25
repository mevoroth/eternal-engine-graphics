#ifndef _COMPARISON_HPP_
#define _COMPARISON_HPP_

namespace Eternal
{
	namespace Graphics
	{
		enum class Comparison
		{
			COMPARISON_NEVER			= 0,
			COMPARISON_LESS,
			COMPARISON_EQUAL,
			COMPARISON_LESS_EQUAL,
			COMPARISON_GREATER,
			COMPARISON_NOT_EQUAL,
			COMPARISON_GREATER_EQUAL,
			COMPARISON_ALWAYS
		};
	}
}

#endif

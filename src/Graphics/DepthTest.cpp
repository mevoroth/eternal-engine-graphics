#include "Graphics/DepthTest.hpp"

namespace Eternal
{
	namespace Graphics
	{
		const DepthTest DepthTestNone;
		const DepthTest DepthTestTestWriteLess(
			DepthTest::Mask::MASK_ALL, ComparisonFunction::COMPARISON_FUNCTION_LESS
		);
	}
}

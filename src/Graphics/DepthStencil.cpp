#include "Graphics/DepthStencil.hpp"

#include "Graphics/DepthTest.hpp"
#include "Graphics/StencilTest.hpp"

namespace Eternal
{
	namespace Graphics
	{
		const DepthStencil DepthStencilNoneNone;
		const DepthStencil DepthStencilTestWriteLessNone(
			DepthTest(DepthTest::Mask::MASK_ALL, ComparisonFunction::COMPARISON_FUNCTION_LESS)
		);
		const DepthStencil DepthStencilTestWriteGreaterNone(
			DepthTest(DepthTest::Mask::MASK_ALL, ComparisonFunction::COMPARISON_FUNCTION_GREATER)
		);
#if ETERNAL_USE_REVERSED_Z
		const DepthStencil DepthStencilTestWriteNone = DepthStencilTestWriteGreaterNone;
#else
		const DepthStencil DepthStencilTestWriteNone = DepthStencilTestWriteLessNone;
#endif
	}
}

#include "Graphics/DepthStencil.hpp"

#include "Graphics/DepthTest.hpp"
#include "Graphics/StencilTest.hpp"

namespace Eternal
{
	namespace Graphics
	{
		const DepthStencil DepthStencilNoneNone(DepthTestNone, StencilTestNone);
	}
}

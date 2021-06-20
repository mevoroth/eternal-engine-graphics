#ifndef _DEPTH_STENCIL_HPP_
#define _DEPTH_STENCIL_HPP_

#include "Graphics/DepthTest.hpp"
#include "Graphics/StencilTest.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class DepthStencil
		{
		public:
			DepthStencil(_In_ const DepthTest& DepthTestState, _In_ const StencilTest& StencilTestState)
				: _DepthTest(DepthTestState)
				, _StencilTest(StencilTestState)
			{
			}

			const DepthTest& GetDepthTest() const
			{
				return _DepthTest;
			}

			const StencilTest& GetStencilTest() const
			{
				return _StencilTest;
			}

		private:
			DepthTest _DepthTest;
			StencilTest _StencilTest;
		};

		extern const DepthStencil DepthStencilNoneNone;
	}
}

#endif

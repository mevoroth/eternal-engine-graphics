#pragma once

#include "Graphics/DepthTest.hpp"
#include "Graphics/StencilTest.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class DepthStencil
		{
		public:
			DepthStencil(_In_ const DepthTest& DepthTestState = DepthTest(), _In_ const StencilTest& StencilTestState = StencilTest())
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
		extern const DepthStencil DepthStencilTestWriteLessNone;
		extern const DepthStencil DepthStencilTestWriteGreaterNone;
	}
}

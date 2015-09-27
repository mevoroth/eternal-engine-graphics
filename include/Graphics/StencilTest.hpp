#ifndef _STENCIL_HPP_
#define _STENCIL_HPP_

#include <cstdint>

#include "Graphics/Comparison.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class StencilTest
		{
		public:
			enum StencilOperator
			{
				KEEP,
				ZERO,
				REPLACE,
				INC_SAT,
				DEC_SAT,
				INV,
				INC,
				DEC
			};
			struct FaceOperator
			{
				FaceOperator() {}
				FaceOperator(const StencilOperator& Fail, const StencilOperator& FailDepth, const StencilOperator& Pass, const Comparison& ComparisonOp)
					: Fail(Fail)
					, FailDepth(FailDepth)
					, Pass(Pass)
					, ComparisonOp(ComparisonOp)
				{
				}

				StencilOperator Fail = KEEP;
				StencilOperator FailDepth = KEEP;
				StencilOperator Pass = KEEP;
				Comparison ComparisonOp = ALWAYS;
			};

			StencilTest() {}
			StencilTest(const FaceOperator& Front, const FaceOperator& Back)
				: _Enabled(true)
				, _Front(Front)
				, _Back(Back)
			{
			}
			bool IsEnabled() const
			{
				return _Enabled;
			}
			const FaceOperator& GetFront() const
			{
				return _Front;
			}
			const FaceOperator& GetBack() const
			{
				return _Back;
			}

		private:
			bool _Enabled = false;
			FaceOperator _Front;
			FaceOperator _Back;
		};
	}
}

#endif

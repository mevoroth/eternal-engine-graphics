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
				STENCIL_OPERATOR_KEEP	= 0,
				STENCIL_OPERATOR_ZERO,
				STENCIL_OPERATOR_REPLACE,
				STENCIL_OPERATOR_INC_SAT,
				STENCIL_OPERATOR_DEC_SAT,
				STENCIL_OPERATOR_INV,
				STENCIL_OPERATOR_INC,
				STENCIL_OPERATOR_DEC
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

				StencilOperator Fail		= STENCIL_OPERATOR_KEEP;
				StencilOperator FailDepth	= STENCIL_OPERATOR_KEEP;
				StencilOperator Pass		= STENCIL_OPERATOR_KEEP;
				Comparison ComparisonOp		= COMPARISON_ALWAYS;
			};

			StencilTest() {}
			StencilTest(_In_ const FaceOperator& Front, _In_ const FaceOperator& Back, _In_ uint8_t ReadMask, _In_ uint8_t WriteMask)
				: _Enabled(true)
				, _Front(Front)
				, _Back(Back)
				, _ReadMask(ReadMask)
				, _WriteMask(WriteMask)
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

			uint8_t GetReadMask() const
			{
				return _ReadMask;
			}

			uint8_t GetWriteMask() const
			{
				return _WriteMask;
			}

		private:
			FaceOperator _Front;
			FaceOperator _Back;
			bool _Enabled = false;
			uint8_t _ReadMask = 0xFF;
			uint8_t _WriteMask = 0xFF;
		};
	}
}

#endif

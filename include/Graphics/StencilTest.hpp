#pragma once

#include "Graphics/Comparison.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class StencilTest
		{
		public:
			enum class StencilOperator
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

				StencilOperator Fail		= StencilOperator::STENCIL_OPERATOR_KEEP;
				StencilOperator FailDepth	= StencilOperator::STENCIL_OPERATOR_KEEP;
				StencilOperator Pass		= StencilOperator::STENCIL_OPERATOR_KEEP;
				Comparison ComparisonOp		= Comparison::COMPARISON_ALWAYS;
			};

			StencilTest() {}
			StencilTest(_In_ const FaceOperator& Front, _In_ const FaceOperator& Back, _In_ uint8_t ReadMask, _In_ uint8_t WriteMask, _In_ uint8_t Reference)
				: _Enabled(true)
				, _Front(Front)
				, _Back(Back)
				, _ReadMask(ReadMask)
				, _WriteMask(WriteMask)
				, _Reference(Reference)
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
			uint8_t GetReference() const
			{
				return _Reference;
			}

		private:
			FaceOperator _Front;
			FaceOperator _Back;
			bool _Enabled = false;
			uint8_t _ReadMask = 0x0;
			uint8_t _WriteMask = 0x0;
			uint8_t _Reference = 0x0;
		};

		extern const StencilTest StencilTestNone;
	}
}

#ifndef _DEPTH_TEST_HPP_
#define _DEPTH_TEST_HPP_

#include "Graphics/Comparison.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class DepthTest
		{
		public:
			enum class Mask
			{
				ZERO	= 0,
				ALL		= 1
			};
			DepthTest() {};
			DepthTest(const Mask& MaskValue, const Comparison& ComparisonOp)
				: _Enabled(true)
				, _Mask(MaskValue)
				, _ComparisonOp(ComparisonOp)
			{
			}

			inline bool IsEnabled() const
			{
				return _Enabled;
			}
			inline Mask GetMask() const
			{
				return _Mask;
			}
			inline bool IsWriteEnabled() const
			{
				return _Mask == Mask::ALL;
			}
			inline Comparison GetComparisonOperator() const
			{
				return _ComparisonOp;
			}

		private:
			bool _Enabled				= false;
			Mask _Mask					= Mask::ZERO;
			Comparison _ComparisonOp	= Comparison::COMPARISON_LESS;
		};

		extern const DepthTest DepthTestNone;
	}
}

#endif

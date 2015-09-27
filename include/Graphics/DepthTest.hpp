#ifndef _DEPTH_TEST_HPP_
#define _DEPTH_TEST_HPP_

#include <cstdint>

#include "Graphics/Comparison.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class DepthTest
		{
		public:
			enum Mask
			{
				ZERO	= 0,
				ALL		= 1
			};
			DepthTest() {};
			DepthTest(Mask MaskValue, const Comparison& ComparisonOp)
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
			inline Comparison GetComparisonOperator() const
			{
				return _ComparisonOp;
			}

		private:
			bool _Enabled = false;
			Mask _Mask = ZERO;
			Comparison _ComparisonOp = LESS;
		};
	}
}

#endif

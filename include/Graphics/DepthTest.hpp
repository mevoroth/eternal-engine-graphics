#pragma once

#include "Graphics/ComparisonFunction.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class DepthTest
		{
		public:
			enum class Mask
			{
				MASK_ZERO	= 0,
				MASK_ALL	= 1
			};
			DepthTest() {};
			DepthTest(const Mask& MaskValue, const ComparisonFunction& InComparison)
				: _Enabled(true)
				, _Mask(MaskValue)
				, _Comparison(InComparison)
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
				return _Mask == Mask::MASK_ALL;
			}
			inline ComparisonFunction GetComparisonFunction() const
			{
				return _Comparison;
			}

		private:
			bool _Enabled				= false;
			Mask _Mask					= Mask::MASK_ZERO;
			ComparisonFunction _Comparison	= ComparisonFunction::COMPARISON_FUNCTION_LESS;
		};

		extern const DepthTest DepthTestNone;
		extern const DepthTest DepthTestTestWriteLess;
	}
}

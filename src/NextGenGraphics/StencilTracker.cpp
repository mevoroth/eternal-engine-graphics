#include "NextGenGraphics/StencilTracker.hpp"

namespace Eternal
{
	namespace Graphics
	{
		constexpr uint32_t StencilTracker::BitCount;

		StencilTracker::StencilTracker()
		{
		}

		StencilTracker::~StencilTracker()
		{
			Validate();
		}

		void StencilTracker::AcquireStencil(_In_ uint32_t InBit, _In_ const string& InOwner)
		{
			ETERNAL_ASSERT(InBit < BitCount);
			ETERNAL_ASSERT(!(_StencilFlags & (1 << InBit)));
			_StencilFlags |= (1 << InBit);
			_StencilFlagOwners[InBit] = InOwner;
		}

		void StencilTracker::ReleaseStencil(_In_ uint32_t InBit)
		{
			ETERNAL_ASSERT(InBit < BitCount);
			ETERNAL_ASSERT(_StencilFlags & (1 << InBit));
			_StencilFlags &= ~(1 << InBit);
			_StencilFlagOwners[InBit] = string();
		}

		void StencilTracker::Validate() const
		{
			ETERNAL_ASSERT(!_StencilFlags);
		}
	}
}

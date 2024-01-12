#pragma once

#include <string>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class StencilTracker
		{
		public:

			static constexpr uint32_t BitCount = 8;

			StencilTracker();
			~StencilTracker();

			void AcquireStencil(_In_ uint32_t InBit, _In_ const string& InOwner);
			void ReleaseStencil(_In_ uint32_t InBit);
			void Validate() const;

		private:

			string _StencilFlagOwners[BitCount]	= {};
			uint8_t _StencilFlags				= 0x0;

		};
	}
}

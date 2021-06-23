#pragma once

#include "Graphics/View.hpp"

enum D3D12_RTV_DIMENSION;

namespace Eternal
{
	namespace Graphics
	{
		extern const D3D12_RTV_DIMENSION D3D12_RTV_DIMENSIONS[];

		class D3D12View : public View
		{
		public:
			D3D12View(_In_ const RenderTargetViewCreateInformation& InViewCreateInformation);
			~D3D12View();
		};
	}
}

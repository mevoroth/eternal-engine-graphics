#include "Graphics_deprecated/ConstantFactory.hpp"

#include "d3d11/D3D11Constant.hpp"

namespace Eternal
{
	namespace Graphics
	{
		Constant* CreateConstant(_In_ size_t BufferSize, _In_ const Resource::Usage& UsageObj, _In_ const Resource::CPUAccess& CPUMode)
		{
			return new D3D11Constant(BufferSize, UsageObj, CPUMode);
		}
		Constant* CreateConstant(_In_ size_t BufferSize, _In_ const Resource::Usage& UsageObj, _In_ const Resource::CPUAccess& CPUMode, _In_ void* Data)
		{
			return new D3D11Constant(BufferSize, UsageObj, CPUMode, Data);
		}
	}
}

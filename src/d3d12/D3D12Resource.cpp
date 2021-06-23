#include "d3d12/D3D12Resource.hpp"
#include <d3d12.h>
#include <string>

namespace Eternal
{
	namespace Graphics
	{
		D3D12Resource::D3D12Resource(_In_ const D3D12ResourceBackBufferCreateInformation& InResourceCreateInformation)
			: Resource(InResourceCreateInformation, ResourceSource::BACK_BUFFER)
			, _Resource(InResourceCreateInformation.BackBufferResource)
		{
			ETERNAL_ASSERT(_Resource);

			std::wstring UTF8String(InResourceCreateInformation.Name.begin(), InResourceCreateInformation.Name.end());
			_Resource->SetName(UTF8String.c_str());
		}

		D3D12Resource::~D3D12Resource()
		{
			switch (GetResourceSource())
			{
			case ResourceSource::BACK_BUFFER:
			case ResourceSource::UNKNOWN:
				_Resource->Release();
				_Resource = nullptr;
				break;
			}
		}
	}
}

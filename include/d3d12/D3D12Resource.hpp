#pragma once

#include "Graphics/Resource.hpp"

struct ID3D12Resource;

namespace Eternal
{
	namespace Graphics
	{
		struct D3D12ResourceBackBufferCreateInformation : public ResourceCreateInformation
		{
			D3D12ResourceBackBufferCreateInformation(_In_ Device& InDevice, _In_ const std::string& InName, _In_ ID3D12Resource* InBackBufferResource)
				: ResourceCreateInformation(InDevice, InName)
				, BackBufferResource(InBackBufferResource)
			{
			}
			ID3D12Resource* BackBufferResource = nullptr;
		};

		class D3D12Resource : public Resource
		{
		public:
			D3D12Resource(_In_ const D3D12ResourceBackBufferCreateInformation& InResourceCreateInformation); // Back buffer
			virtual ~D3D12Resource();

		private:
			ID3D12Resource* _Resource = nullptr;
		};
	}
}

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
				: ResourceCreateInformation(InDevice, InName, ResourceMemoryType::RESOURCE_MEMORY_TYPE_GPU_MEMORY, TransitionState::TRANSITION_PRESENT)
				, BackBufferResource(InBackBufferResource)
			{
			}
			ID3D12Resource* BackBufferResource = nullptr;
		};

		class D3D12Resource : public Resource
		{
		public:
			/**
			 * Back buffer
			 */
			D3D12Resource(_In_ const D3D12ResourceBackBufferCreateInformation& InResourceCreateInformation);
			/**
			 * Texture
			 */
			D3D12Resource(_In_ const TextureResourceCreateInformation& InResourceCreateInformation);
			/**
			 * Buffer
			 */
			D3D12Resource(_In_ const BufferResourceCreateInformation& InResourceCreateInformation);
			virtual ~D3D12Resource();

			inline ID3D12Resource* GetD3D12Resource() { return _Resource; }

		private:
			void _SetName();

			ID3D12Resource*	_Resource		= nullptr;
		};
	}
}

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

		class D3D12Resource final : public Resource
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
			virtual ~D3D12Resource() override final;

			virtual void Unmap(_In_ const MapRange& InMapRange) override final;
			virtual TextureToBufferMemoryFootprint GetTextureToBufferMemoryFootprint(_In_ Device& InDevice) const override final;

			inline ID3D12Resource* GetD3D12Resource() const { return _Resource; }

		protected:
			virtual void* Map(_In_ const MapRange& InMapRange) override final;

		private:
			void _SetDebugName();

			ID3D12Resource*	_Resource		= nullptr;
		};
	}
}

#pragma once

#include "Graphics/CommandList.hpp"
#include "d3d12/D3D12CommandAllocator.hpp"
#include <d3d12.h>

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class CommandAllocator;

		class D3D12CommandList : public CommandList
		{
		public:
			D3D12CommandList(_In_ Device& InDevice, _In_ CommandAllocator& InCommandAllocator);
			~D3D12CommandList();

			virtual void Begin() override final;
			virtual void End() override final;
			
			virtual void BeginRenderPass(RenderPass& InRenderPass) override final;
			virtual void EndRenderPass() override final;

			virtual void Transition(_In_ const ResourceTransition InResourceTransitions[], _In_ uint32_t InResourceTransitionsCount) override final;

			inline ID3D12CommandList* GetD3D12CommandList() { return _GraphicCommandList5; }
			inline D3D12CommandAllocator& GetD3D12CommandAllocator() { return static_cast<D3D12CommandAllocator&>(GetCommandAllocator()); }

		private:
			ID3D12GraphicsCommandList5* _GraphicCommandList5 = nullptr;
		};
	}
}

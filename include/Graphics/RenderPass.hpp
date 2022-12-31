#pragma once

#include "Graphics/BlendState.hpp"
#include "Graphics/Viewport.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class View;
		class Viewport;

		enum class LoadOperator
		{
			NO_LOAD,
			LOAD,
			CLEAR,
			COUNT
		};

		enum class StoreOperator
		{
			NO_STORE,
			STORE,
			RESOLVE,
			COUNT
		};

		struct RenderTargetOperator
		{
			static const RenderTargetOperator NoLoad_NoStore;
			static const RenderTargetOperator Load_NoStore;
			static const RenderTargetOperator Load_Store;
			static const RenderTargetOperator Load_Resolve;
			static const RenderTargetOperator Clear_NoStore;
			static const RenderTargetOperator Clear_Store;
			static const RenderTargetOperator Clear_Resolve;

			RenderTargetOperator(_In_ const LoadOperator& InLoadOperator, _In_ const StoreOperator& InStoreOperator)
				: Load(InLoadOperator)
				, Store(InStoreOperator)
			{
			}

			LoadOperator Load	= LoadOperator::NO_LOAD;
			StoreOperator Store	= StoreOperator::NO_STORE;
		};

		struct RenderTargetInformation
		{
			RenderTargetInformation(_In_ const BlendState& InRenderTargetBlendState, _In_ const RenderTargetOperator& InOperator, _In_ View* InRenderTarget)
				: RenderTargetBlendState(InRenderTargetBlendState)
				, Operator(InOperator)
				, RenderTarget(InRenderTarget)
			{
			}

			BlendState				RenderTargetBlendState;
			RenderTargetOperator	Operator;
			View*					RenderTarget = nullptr;
		};

		struct RenderPassCreateInformation
		{
			RenderPassCreateInformation(
				_In_ const Viewport& InViewport,
				_In_ const vector<RenderTargetInformation>& InRenderTargets,
				_In_ View* InDepthStencilRenderTarget = nullptr,
				_In_ const RenderTargetOperator& InDepthStencilOperator = RenderTargetOperator::NoLoad_NoStore,
				_In_ const LogicBlend& InLogicBlend = LogicBlendNone
			)
				: RenderTargets(InRenderTargets)
				, LogicBlend(InLogicBlend)
				, DepthStencilOperator(InDepthStencilOperator)
				, Viewport(InViewport)
				, DepthStencilRenderTarget(InDepthStencilRenderTarget)
			{
			}

			RenderPassCreateInformation(
				_In_ const Viewport& InViewport,
				_In_ View* InDepthStencilRenderTarget,
				_In_ const RenderTargetOperator& InDepthStencilOperator = RenderTargetOperator::NoLoad_NoStore,
				_In_ const LogicBlend& InLogicBlend = LogicBlendNone
			)
				: LogicBlend(InLogicBlend)
				, DepthStencilOperator(InDepthStencilOperator)
				, Viewport(InViewport)
				, DepthStencilRenderTarget(InDepthStencilRenderTarget)
			{
			}

			vector<RenderTargetInformation> RenderTargets;
			LogicBlend LogicBlend;
			RenderTargetOperator DepthStencilOperator;
			const Viewport& Viewport;
			View* DepthStencilRenderTarget = nullptr;
		};

		class RenderPass
		{
		public:
			virtual ~RenderPass() {}

			const vector<RenderTargetInformation>&	GetRenderTargets() const			{ return _RenderPassCreateInformation.RenderTargets; }
			const LogicBlend&						GetLogicBlend() const				{ return _RenderPassCreateInformation.LogicBlend; }
			const Viewport&							GetViewport() const					{ return _RenderPassCreateInformation.Viewport; }
			const View*								GetDepthStencilRenderTarget() const	{ return _RenderPassCreateInformation.DepthStencilRenderTarget; }
			const RenderTargetOperator&				GetDepthStencilOperator() const		{ return _RenderPassCreateInformation.DepthStencilOperator; }

		protected:
			RenderPass(_In_ const RenderPassCreateInformation& InRenderPassCreateInformation);

		private:
			RenderPassCreateInformation _RenderPassCreateInformation;
		};
	}
}

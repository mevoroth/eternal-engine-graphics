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
			static const RenderTargetOperator NoLoad_Store;
			static const RenderTargetOperator NoLoad_Resolve;
			static const RenderTargetOperator Load_NoStore;
			static const RenderTargetOperator Load_Store;
			static const RenderTargetOperator Load_Resolve;
			static const RenderTargetOperator Clear_NoStore;
			static const RenderTargetOperator Clear_Store;
			static const RenderTargetOperator Clear_Resolve;

			RenderTargetOperator(const LoadOperator& InLoadOperator, const StoreOperator& InStoreOperator)
				: LoadOp(InLoadOperator)
				, StoreOp(InStoreOperator)
			{
			}

			LoadOperator LoadOp		= LoadOperator::NO_LOAD;
			StoreOperator StoreOp	= StoreOperator::NO_STORE;
		};

		struct RenderTargetInformation
		{
			RenderTargetInformation(const BlendState& InRenderTargetBlendState, RenderTargetOperator InOperator, View* InRenderTarget)
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

			vector<RenderTargetInformation> RenderTargets;
			LogicBlend LogicBlend;
			RenderTargetOperator DepthStencilOperator;
			const Viewport& Viewport;
			View* DepthStencilRenderTarget;
		};

		class RenderPass
		{
		public:
			RenderPass(_In_ const RenderPassCreateInformation& CreateInformation);
			virtual ~RenderPass() {}

			const vector<RenderTargetInformation>&	GetRenderTargets() const			{ return _CreateInformation.RenderTargets; }
			const LogicBlend&						GetLogicBlend() const				{ return _CreateInformation.LogicBlend; }
			const Viewport&							GetViewport() const					{ return _CreateInformation.Viewport; }
			const View*								GetDepthStencilRenderTarget() const	{ return _CreateInformation.DepthStencilRenderTarget; }
			const RenderTargetOperator&				GetDepthStencilOperator() const		{ return _CreateInformation.DepthStencilOperator; }

		private:
			RenderPassCreateInformation _CreateInformation;
		};
	}
}

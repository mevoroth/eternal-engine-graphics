#pragma once

#include "Graphics/CommandUtils.hpp"
#include <vector>
#include <unordered_set>
#include <unordered_map>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		struct FrameGraphPassOutputs;
		class View;
		class Resource;

		struct FrameGraphTransition
		{
			View* TransitionView	= nullptr;
			TransitionState State	= TransitionState::TRANSITION_UNDEFINED;
		};

		struct FrameGraphPassInputs
		{
			bool DependsOn(_In_ const FrameGraphPassOutputs& InOutputs) const;

			vector<View*> InputViews;
		};

		struct FrameGraphPassOutputs
		{
			bool DependsOn(_In_ const FrameGraphPassOutputs& InOutputs) const;

			unordered_map<Resource*, FrameGraphTransition> OutputViews;
		};

		class FrameGraphPass
		{
		public:

			template<typename RunFunctionType>
			void RunPass(_In_ const RunFunctionType& InRunFunctor)
			{
				InRunFunctor(this);
			}

			virtual void GetInputs(_Out_ FrameGraphPassInputs& OutInputs) const = 0;
			virtual void GetOutputs(_Out_ FrameGraphPassOutputs& OutOutputs) const = 0;

		};

		class FrameGraphPassGroup
		{
		public:
			
			void RegisterGraphPass(_In_ FrameGraphPass* InPass);
			void GetOutputs(_Out_ FrameGraphPassOutputs& OutOutputs) const;
			template<typename RunFunctionType>
			void RunGroup(_In_ const RunFunctionType& InRunFunctor)
			{
				for (uint32_t PassIndex = 0; PassIndex < _GraphPasses.size(); ++PassIndex)
					_GraphPasses[PassIndex]->RunPass(InRunFunctor);
			}

		private:

			vector<FrameGraphPass*> _GraphPasses;

			friend struct FrameGraphPassInputs;
		};

		class FrameGraph
		{
		public:

			FrameGraph();
			~FrameGraph();

			void RegisterGraphPass(_In_ FrameGraphPass* InPass);
			template<typename RunFunctionType>
			void RunGraph(_In_ const RunFunctionType& InRunFunctor)
			{
				for (uint32_t PassGroupIndex = 0; PassGroupIndex < _GraphPassGroups.size(); ++PassGroupIndex)
					_GraphPassGroups[PassGroupIndex].RunGroup(InRunFunctor);
			}

		private:

			vector<FrameGraphPassGroup> _GraphPassGroups;

		};

		//using namespace std;

		//class Resource;
		//class RenderPass;

		//class FrameGraph
		//{
		//	//struct Pass
		//	//{
		//	//	RenderPass*			Pass = nullptr;
		//	//	vector<Resource*>	Inputs;
		//	//	vector<Resource*>	Outputs;
		//	//};
		//public:
		//	//void RegisterResource(_In_ Resource* ResourceObj);
		//	void RegisterRenderPass(_In_ RenderPass* RenderPassObj, _In_ const vector<Resource*>& Inputs, _In_ const vector<Resource*>& Outputs);
		//	void Batch();

		//private:
		//	//vector<Resource*> _Resources;
		//	//map< Resource*, set<Resource*> > _Dependencies;
		//	//map< RenderPass*, set<Resource*> > _RenderPasses;
		//	//vector< vector<RenderPass*> > _BatchedRenderPasses;

		//	vector< set<Resource*> > _Consumed;
		//	vector< set<Resource*> > _Produced;
		//	vector< set<RenderPass*> > _RenderPasses;

		//	//vector<Pass> _Passes;
		//};
	}
}

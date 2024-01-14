#include "NextGenGraphics/FrameGraph.hpp"
#include "Graphics/View.hpp"

namespace Eternal
{
	namespace Graphics
	{
		bool FrameGraphPassInputs::DependsOn(_In_ const FrameGraphPassOutputs& InOutputs) const
		{
			for (uint32_t InputIndex = 0; InputIndex < InputViews.size(); ++InputIndex)
			{
				if (InOutputs.OutputViews.find(&InputViews[InputIndex]->GetResource()) != InOutputs.OutputViews.end())
					return true;
			}

			return false;
		}

		//////////////////////////////////////////////////////////////////////////

		bool FrameGraphPassOutputs::DependsOn(_In_ const FrameGraphPassOutputs& InOutputs) const
		{
			for (auto OutputViewIterator = OutputViews.begin(); OutputViewIterator != OutputViews.end(); ++OutputViewIterator)
			{
				if ((OutputViewIterator->second.State & TransitionState::TRANSITION_SHADER_WRITE) == TransitionState::TRANSITION_SHADER_WRITE &&
					InOutputs.OutputViews.find(OutputViewIterator->first) != InOutputs.OutputViews.end())
				{
					return true;
				}
			}
			return false;
		}

		//////////////////////////////////////////////////////////////////////////

		void FrameGraphPassGroup::RegisterGraphPass(_In_ FrameGraphPass* InPass)
		{
			_GraphPasses.push_back(InPass);
		}

		void FrameGraphPassGroup::GetOutputs(_Out_ FrameGraphPassOutputs& OutOutputs) const
		{
			for (uint32_t PassIndex = 0; PassIndex < _GraphPasses.size(); ++PassIndex)
			{
				FrameGraphPassOutputs CurrentPassOuputs;
				_GraphPasses[PassIndex]->GetOutputs(CurrentPassOuputs);
				for (auto OutputIterator = CurrentPassOuputs.OutputViews.begin(); OutputIterator != CurrentPassOuputs.OutputViews.end(); ++OutputIterator)
				{
					OutOutputs.OutputViews.emplace(*OutputIterator);
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////

		FrameGraph::FrameGraph()
		{
		}

		FrameGraph::~FrameGraph()
		{
		}

		void FrameGraph::RegisterGraphPass(_In_ FrameGraphPass* InPass)
		{
			FrameGraphPassGroup* PassGroup = nullptr;
			
			FrameGraphPassInputs Inputs;
			FrameGraphPassOutputs Outputs;
			InPass->GetInputs(Inputs);
			InPass->GetOutputs(Outputs);
			
			for (int PassGroupIndex = static_cast<int>(_GraphPassGroups.size()) - 1; PassGroupIndex >= 0; --PassGroupIndex)
			{
				FrameGraphPassOutputs PassOutputs;
				_GraphPassGroups[PassGroupIndex].GetOutputs(PassOutputs);

				if (!Inputs.DependsOn(PassOutputs) &&
					!Outputs.DependsOn(PassOutputs))
				{
					PassGroup = &_GraphPassGroups[PassGroupIndex];
					break;
				}
			}
			if (!PassGroup)
			{
				_GraphPassGroups.push_back({});
				PassGroup = &_GraphPassGroups.back();
			}
			
			ETERNAL_ASSERT(PassGroup);
			PassGroup->RegisterGraphPass(InPass);
		}

		void FrameGraph::CompileGraph()
		{

		}
	}
}

//static uint32_t FindMaxDepth(_In_ const vector<Resource*>& Resources)
//{
//	for (const Resource* const& CurrentResource : Resources)
//	{
//
//	}
//}
//
//void FrameGraph::RegisterRenderPass(_In_ RenderPass* RenderPassObj, _In_ const vector<Resource*>& Inputs, _In_ const vector<Resource*>& Outputs)
//{
//	uint32_t MinDepth = UINT32_MAX;
//	uint32_t TemporaryMinDepth = 0;
//
//	//auto FindMaxDepth = [TemporaryMinDepth](const Resource* ResourceObj)
//	//{
//	//	for ()
//	//};
//	//for_each(Inputs.cbegin(), Inputs.cend(), FindMaxDepth);
//
//	for (uint32_t InputIndex = 0; InputIndex < Inputs.size(); ++InputIndex)
//	{
//		Resource* Input = Inputs[InputIndex];
//		int Depth;
//		for (Depth = _Produced.size() - 1; Depth >= 0; --Depth)
//		{
//			if (_Produced[Depth].find(Input) != _Produced[Depth].cend())
//			{
//				Depth++;
//				break;
//			}
//		}
//		TemporaryMinDepth = std::max(TemporaryMinDepth, (uint32_t)std::max(0, Depth));
//	}
//	MinDepth = std::min(MinDepth, TemporaryMinDepth);
//
//	TemporaryMinDepth = 0;
//	for (uint32_t OutputIndex = 0; OutputIndex < Outputs.size(); ++OutputIndex)
//	{
//		Resource* Output = Outputs[OutputIndex];
//		int Depth;
//		for (Depth = _Consumed.size() - 1; Depth >= 0; --Depth)
//		{
//			ETERNAL_ASSERT(_Produced[Depth].find(Output) == _Produced[Depth].cend());
//			if (_Consumed[Depth].find(Output) != _Consumed[Depth].cend())
//			{
//				Depth++;
//				break;
//			}
//		}
//		TemporaryMinDepth = std::max(TemporaryMinDepth, (uint32_t)std::max(0, Depth));
//	}
//	MinDepth = std::max(MinDepth, TemporaryMinDepth);
//
//	if (MinDepth >= _Produced.size())
//	{
//		_Produced.push_back(set<Resource*>());
//		_Consumed.push_back(set<Resource*>());
//		_RenderPasses.push_back(set<RenderPass*>());
//	}
//
//	_Consumed[MinDepth].insert(Inputs.begin(), Inputs.end());
//	_Produced[MinDepth].insert(Outputs.begin(), Outputs.end());
//	_RenderPasses[MinDepth].insert(RenderPassObj);
//	
//	//for (int Depth = _Produced.size() - 1; Depth > 0; --Depth)
//	//{
//	//	for (int InputIndex = 0; InputIndex < Inputs.size(); ++InputIndex)
//	//	{
//
//	//	}
//	//	//for (int InputIndex = 0; InputIndex)
//	//	//_Produced[Depth].find()
//	//}
//
//	//Pass PassObj;
//	//PassObj.Pass = RenderPassObj;
//	//PassObj.Inputs = Inputs;
//	//PassObj.Outputs = Outputs;
//	//_Passes.push_back(PassObj);
//
//	//_RenderPasses[RenderPassObj].insert(Inputs.begin(), Inputs.end());
//	//_RenderPasses[RenderPassObj].insert(Outputs.begin(), Outputs.end());
//	//for (uint32_t OutputIndex = 0; OutputIndex < Outputs.size(); ++OutputIndex)
//	//{
//	//	set<Resource*>& Dependency = _Dependencies[Outputs[OutputIndex]];
//	//	Dependency.insert(Inputs.begin(), Inputs.end());
//	//}
//}
//
//void FrameGraph::Batch()
//{
//	//for (vector<Pass>::reverse_iterator CurrentPass = _Passes.rbegin(); CurrentPass != _Passes.rend(); ++CurrentPass)
//	//{
//	//	(*CurrentPass).Pass
//	//}
//
//	//vkCmdPipelineBarrier()
//}

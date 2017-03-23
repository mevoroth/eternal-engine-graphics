#include "NextGenGraphics/FrameGraph.hpp"

#include "Macros/Macros.hpp"
#include <set>
#include <algorithm>
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <Windows.h>
using namespace std;
using namespace Eternal::Graphics;

//void FrameGraph::RegisterResource(_In_ Resource* ResourceObj)
//{
//	ETERNAL_ASSERT(find(_Resources.cbegin(), _Resources.cend(), ResourceObj) == _Resources.cend());
//	_Resources.push_back(ResourceObj);
//}
#undef max
#undef min
void FrameGraph::RegisterRenderPass(_In_ RenderPass* RenderPassObj, _In_ const vector<Resource*>& Inputs, _In_ const vector<Resource*>& Outputs)
{
	uint32_t MinDepth = UINT32_MAX;
	uint32_t TemporaryMinDepth = 0;
	for (uint32_t InputIndex = 0; InputIndex < Inputs.size(); ++InputIndex)
	{
		Resource* Input = Inputs[InputIndex];
		int Depth;
		for (Depth = _Produced.size() - 1; Depth >= 0; --Depth)
		{
			if (_Produced[Depth].find(Input) != _Produced[Depth].cend())
			{
				Depth++;
				break;
			}
		}
		TemporaryMinDepth = std::max(TemporaryMinDepth, (uint32_t)std::max(0, Depth));
	}
	MinDepth = std::min(MinDepth, TemporaryMinDepth);

	TemporaryMinDepth = 0;
	for (uint32_t OutputIndex = 0; OutputIndex < Outputs.size(); ++OutputIndex)
	{
		Resource* Output = Outputs[OutputIndex];
		int Depth;
		for (Depth = _Consumed.size() - 1; Depth >= 0; --Depth)
		{
			ETERNAL_ASSERT(_Produced[Depth].find(Output) == _Produced[Depth].cend());
			if (_Consumed[Depth].find(Output) != _Consumed[Depth].cend())
			{
				Depth++;
				break;
			}
		}
		TemporaryMinDepth = std::max(TemporaryMinDepth, (uint32_t)std::max(0, Depth));
	}
	MinDepth = std::max(MinDepth, TemporaryMinDepth);

	if (MinDepth >= _Produced.size())
	{
		_Produced.push_back(set<Resource*>());
		_Consumed.push_back(set<Resource*>());
		_RenderPasses.push_back(set<RenderPass*>());
	}

	_Consumed[MinDepth].insert(Inputs.begin(), Inputs.end());
	_Produced[MinDepth].insert(Outputs.begin(), Outputs.end());
	_RenderPasses[MinDepth].insert(RenderPassObj);

	//for (int Depth = _Produced.size() - 1; Depth > 0; --Depth)
	//{
	//	for (int InputIndex = 0; InputIndex < Inputs.size(); ++InputIndex)
	//	{

	//	}
	//	//for (int InputIndex = 0; InputIndex)
	//	//_Produced[Depth].find()
	//}

	//Pass PassObj;
	//PassObj.Pass = RenderPassObj;
	//PassObj.Inputs = Inputs;
	//PassObj.Outputs = Outputs;
	//_Passes.push_back(PassObj);

	//_RenderPasses[RenderPassObj].insert(Inputs.begin(), Inputs.end());
	//_RenderPasses[RenderPassObj].insert(Outputs.begin(), Outputs.end());
	//for (uint32_t OutputIndex = 0; OutputIndex < Outputs.size(); ++OutputIndex)
	//{
	//	set<Resource*>& Dependency = _Dependencies[Outputs[OutputIndex]];
	//	Dependency.insert(Inputs.begin(), Inputs.end());
	//}
}

void FrameGraph::Batch()
{
	//for (vector<Pass>::reverse_iterator CurrentPass = _Passes.rbegin(); CurrentPass != _Passes.rend(); ++CurrentPass)
	//{
	//	(*CurrentPass).Pass
	//}
}

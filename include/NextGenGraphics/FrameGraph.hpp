#ifndef _FRAME_GRAPH_HPP_
#define _FRAME_GRAPH_HPP_

#include <map>
#include <set>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		class Resource;
		class RenderPass;

		class FrameGraph
		{
			//struct Pass
			//{
			//	RenderPass*			Pass = nullptr;
			//	vector<Resource*>	Inputs;
			//	vector<Resource*>	Outputs;
			//};
		public:
			//void RegisterResource(_In_ Resource* ResourceObj);
			void RegisterRenderPass(_In_ RenderPass* RenderPassObj, _In_ const vector<Resource*>& Inputs, _In_ const vector<Resource*>& Outputs);
			void Batch();

		private:
			//vector<Resource*> _Resources;
			//map< Resource*, set<Resource*> > _Dependencies;
			//map< RenderPass*, set<Resource*> > _RenderPasses;
			//vector< vector<RenderPass*> > _BatchedRenderPasses;

			vector< set<Resource*> > _Consumed;
			vector< set<Resource*> > _Produced;
			vector< set<RenderPass*> > _RenderPasses;

			//vector<Pass> _Passes;
		};
	}
}

#endif

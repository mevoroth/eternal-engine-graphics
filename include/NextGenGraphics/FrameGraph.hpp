#ifndef _FRAME_GRAPH_HPP_
#define _FRAME_GRAPH_HPP_

#include <cstdint>

namespace Eternal
{
	namespace Graphics
	{
		class Resource;
		class RenderPass;

		class FrameGraph
		{
		public:
			void RegisterResource(_In_ Resource* ResourceObj);
			void RegisterRenderPass(_In_ RenderPass* RenderPassObj, _In_ Resource* Inputs[], _In_ uint32_t InputsCount, _In_ Resource* Outputs[], _In_ uint32_t OutputsCount);
		};
	}
}

#endif

#ifndef _CONTEXT_HPP_
#define _CONTEXT_HPP_

#include <cstdint>
#include "GraphicsSettings.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Device;
		class Fence;
		class RenderPass;
		class Resource;

		class Context
		{
		public:
			Context(_In_ Device& DeviceObj);
			virtual ~Context();

			void		SetFrameIndex(_In_ uint32_t FrameIndex)					{ _FrameIndex = FrameIndex; }
			uint32_t	GetFrameIndex() const									{ return _FrameIndex; }
			Fence*		GetFrameFence()											{ return _FrameFence; }
			void		SetFrameRenderPass(_In_ RenderPass* FrameRenderPass)	{ _FrameRenderPass = FrameRenderPass; }
			RenderPass*	GetFrameRenderPass()									{ return _FrameRenderPass; }
			void		SetFrameBackBuffer(_In_ Resource* FrameBackBuffer)		{ _FrameBackBuffer = FrameBackBuffer; }
			Resource*	GetFrameBackBuffer()									{ return _FrameBackBuffer; }

		private:
			Fence*			_FrameFence			=	nullptr;
			uint32_t		_FrameIndex			=	0u;
			RenderPass*		_FrameRenderPass	=	nullptr;
			Resource*		_FrameBackBuffer	=	nullptr;
		};
	}
}

#endif

#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include <cstdint>

#include "GraphicsSettings.hpp"
#include "BlendState.hpp"
#include "Vertex.hpp"
#include "Viewport.hpp"
#include "Types/Types.hpp"
#include "Container/Stack.hpp"

#define RENDER_MAX_CONTEXTS 64

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Types;
		using namespace Eternal::Container;

		class RenderTarget;

		class Renderer
		{
		public:
			enum RenderMode
			{
				HARDWARE,
				SOFTWARE
			};
			enum AntiAliasing
			{
				NO_AA = 0x10000 | 0x0,
				MSAA_2X = 0x20000 | 0x0,
				MSAA_4X = 0x40000 | 0x0,
				MSAA_8X = 0x80000 | 0x0,
				CSAA_8X = 0x40000 | 0x8,
				CSAA_16X = 0x40000 | 0x10,
				CSAA_32X = 0x40000 | 0x20,
				CSAA_8XQ = 0x80000 | 0x8,
				CSAA_16XQ = 0x80000 | 0x10,
				CSAA_32XQ = 0x80000 | 0x20
			};
		private:
			static Renderer* _Inst;
			RenderMode _Mode;
			AntiAliasing _AA;
			RenderTarget* _BackBuffer;

		protected:
			virtual void _SetBackBuffer(_In_ RenderTarget* backBuffer);

		public:
			Renderer(_In_ const RenderMode& mode = HARDWARE, _In_ const AntiAliasing& aa = MSAA_4X);
			static Renderer* Get();
			/**
			 * Get Render Mode (HW or SW)
			 */
			inline RenderMode GetRenderMode() const
			{
				return _Mode;
			}
			/**
			 * Get AntiAliasing
			 */
			inline AntiAliasing GetAntiAliasing() const
			{
				return _AA;
			}
			virtual RenderTarget* GetBackBuffer() const;
		};
	}
}

#endif

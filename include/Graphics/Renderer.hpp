#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include <cstdint>

#include "Material.hpp"
#include "VertexBuffer.hpp"
#include "Camera.hpp"
#include "RenderTarget.hpp"
#include "BlendState.hpp"
#include "Vertex.hpp"

namespace Eternal
{
	namespace Graphics
	{
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
			RenderMode _mode;
			AntiAliasing _aa;
			RenderTarget* _backBuffer;
		protected:
			virtual void _SetBackBuffer(_In_ RenderTarget* backBuffer);
		public:
			Renderer(_In_ const RenderMode& mode = HARDWARE, _In_ const AntiAliasing& aa = MSAA_4X);
			/**
			 * Attach Camera
			 */
			virtual void AttachCamera(_In_ Camera* camera) = 0;
			/**
			 * Set VBO
			 */
			virtual void SetVBO(_In_ VertexBuffer* buffer) = 0;
			/**
			 * Attach Material for Rendering
			 */
			virtual void AttachMaterial(_In_ Material* material) = 0;
			virtual void AttachRenderTargets(_In_ RenderTarget** renderTargets, _In_ int count) = 0;
			virtual void ClearRenderTargets(_In_ RenderTarget** renderTargets, _In_ int count) = 0;
			virtual void SetBlendMode(_In_ BlendState* blendMode) = 0;
			/**
			 * Draw
			 */
			//virtual void Draw() = 0;
			virtual void DrawIndexed(_In_ const Vertex vertices[], _In_ int verticesCount, _In_ size_t vertexSize,
				_In_ const uint16_t indices[], _In_ int indicesCount) = 0;
			/**
			 * Get Render Mode (HW or SW)
			 */
			inline RenderMode GetRenderMode() const
			{
				return _mode;
			}
			/**
			 * Get AntiAliasing
			 */
			inline AntiAliasing GetAntiAliasing() const
			{
				return _aa;
			}
			virtual RenderTarget* GetBackBuffer() const;
		};
	}
}

#endif

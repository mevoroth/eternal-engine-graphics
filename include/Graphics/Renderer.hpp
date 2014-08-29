#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "Material.hpp"
#include "VertexBuffer.hpp"
#include "Camera.hpp"
#include "RenderTarget.hpp"

namespace Eternal
{
	namespace Graphics
	{
		template <typename Vector4, typename Matrix4x4> class Renderer
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
			virtual void AttachCamera(_In_ Camera<Vector4, Matrix4x4>* camera) = 0;
			/**
			 * Set VBO
			 */
			virtual void SetVBO(_In_ const VertexBuffer& buffer) = 0;
			/**
			 * Attach Material for Rendering
			 */
			virtual void AttachMaterial(_In_ const Material& material) = 0;
			virtual void AttachRenderTargets(_In_ RenderTarget** renderTargets, _In_ int count) = 0;
			/**
			 * Draw
			 */
			virtual void Draw() = 0;
			/**
			 * Get Render Mode (HW or SW)
			 */
			inline RenderMode GetRenderMode() const;
			/**
			 * Get AntiAliasing
			 */
			inline AntiAliasing GetAntiAliasing() const;
			virtual RenderTarget* GetBackBuffer() const;
		};

#pragma region Renderer Implementation
		template <typename Vector4, typename Matrix4x4>
		Renderer<Vector4, Matrix4x4>::Renderer(_In_ const RenderMode& mode, _In_ const AntiAliasing& aa)
			: _mode(mode)
			, _aa(aa)
		{
		}

		template <typename Vector4, typename Matrix4x4>
		inline typename Renderer<Vector4, Matrix4x4>::RenderMode Renderer<Vector4, Matrix4x4>::GetRenderMode() const
		{
			return _mode;
		}

		template <typename Vector4, typename Matrix4x4>
		inline typename Renderer<Vector4, Matrix4x4>::AntiAliasing Renderer<Vector4, Matrix4x4>::GetAntiAliasing() const
		{
			return _aa;
		}
		template <typename Vector4, typename Matrix4x4>
		void Renderer<Vector4, Matrix4x4>::_SetBackBuffer(RenderTarget* backBuffer)
		{
			_backBuffer = backBuffer;
		}
		template <typename Vector4, typename Matrix4x4>
		RenderTarget* Renderer<Vector4, Matrix4x4>::GetBackBuffer() const
		{
			return _backBuffer;
		}
#pragma endregion
	}
}

#endif

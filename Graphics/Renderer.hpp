#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "Material.hpp"
#include "VertexBuffer.hpp"
#include "Camera.hpp"

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
		private:
			RenderMode _mode;
		public:
			Renderer(_In_ const RenderMode& mode = HARDWARE);
			/**
			 * Attach Camera
			 */
			virtual void AttachCamera(_In_ const Camera<Vector4, Matrix4x4>& camera) = 0;
			/**
			 * Set VBO
			 */
			virtual void SetVBO(_In_ const VertexBuffer& buffer) = 0;
			/**
			 * Attach Material for Rendering
			 */
			virtual void AttachMaterial(_In_ const Material& material) = 0;
			/**
			 * Draw
			 */
			virtual void Draw() = 0;
			/**
			 * Get Render Mode (HW or SW)
			 */
			inline RenderMode GetRenderMode() const;
		};

#pragma region Renderer Implementation
		template <typename Vector4, typename Matrix4x4>
		Renderer<Vector4, Matrix4x4>::Renderer(_In_ const RenderMode& mode)
			: _mode(mode)
		{
		}

		template <typename Vector4, typename Matrix4x4>
		inline Renderer<Vector4, Matrix4x4>::RenderMode Renderer<Vector4, Matrix4x4>::GetRenderMode() const
		{
			return _mode;
		}
#pragma endregion
	}
}

#endif

#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "Material.hpp"
#include "VertexBuffer.hpp"
#include "Camera.hpp"

namespace Eternal
{
	namespace Graphics
	{
		template <typename Matrix> class Renderer
		{
		public:
			/**
			 * Attach Camera
			 */
			virtual void AttachCamera(_In_ const Camera<Matrix>& camera) = 0;
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
		};
	}
}

#endif

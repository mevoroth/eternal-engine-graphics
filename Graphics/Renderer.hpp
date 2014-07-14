#ifndef _RENDERER_HPP_
#define _RENDERER_HPP_

#include "Material.hpp"
#include "VertexBuffer.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class Renderer
		{
		public:
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

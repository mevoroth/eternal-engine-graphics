#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_

#include <vector>
#include <string>
#include <d3d11.h>

#include "MaterialProperty.hpp"
#include "Shader.hpp"

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		class Material
		{
		public:
			/**
			 * Set Material Params
			 */
			virtual void SetMaterialDesc(_In_ const MaterialProperty& matProperty) = 0;
			/**
			 * Apply Material Params
			 */
			virtual void Apply() = 0;
			/**
			 * Set vertex shader
			 */
			virtual void AttachVertexShader(_Inout_ Shader* shader) = 0;
			/**
			 * Set geometry shader
			 */
			virtual void AttachGeometryShader(_Inout_ Shader* shader) = 0;
			/**
			 * Set pixel shader
			 */
			virtual void AttachPixelShader(_Inout_ Shader* shader) = 0;
		};
	}
}

#endif

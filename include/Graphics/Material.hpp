#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_

#include "GraphicsSettings.hpp"
#include "Types/Types.hpp"
#include "MaterialProperty.hpp"
#include "Shader.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Types;

		class Texture;

		class Material
		{
		public:
			/**
			 * Set Material Params
			 */
			virtual void SetMaterialDesc(_In_ const MaterialProperty& MatProperty) = 0;
			/**
			 * Apply Material Params
			 */
			virtual void Apply() = 0;
			/**
			 * Set vertex shader
			 */
			virtual void AttachVertexShader(_Inout_ Shader* Shader) = 0;
			/**
			 * Set geometry shader
			 */
			virtual void AttachGeometryShader(_Inout_ Shader* Shader) = 0;
			/**
			 * Set pixel shader
			 */
			virtual void AttachPixelShader(_Inout_ Shader* Shader) = 0;
			virtual void SetFloat(_In_ const string& Name, _In_ float Value) = 0;
			virtual void SetVector2(_In_ const string& Name, _In_ const Vector2& Value) = 0;
			virtual void SetVector3(_In_ const string& Name, _In_ const Vector3& Value) = 0;
			virtual void SetVector4(_In_ const string& Name, _In_ const Vector4& Value) = 0;
			virtual void SetTexture(_In_ const string& Name, _In_ const Texture& Value) = 0;
			virtual void SetColor(_In_ const string& Name, _In_ const Vector4& Value) = 0;
		};
	}
}

#endif

#ifndef _MATERIAL_INSTANCE_HPP_
#define _MATERIAL_INSTANCE_HPP_

#include <string>

#include "GraphicsSettings.hpp"
#include "Types/Types.hpp"
#include "Texture.hpp"

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;
		using namespace Eternal::Types;

		class MaterialInstance
		{
		public:
			virtual void SetFloat(_In_ const string& Name, _In_ float Value) = 0;
			virtual void SetVector2(_In_ const string& Name, _In_ const Vector2& Value) = 0;
			virtual void SetVector3(_In_ const string& Name, _In_ const Vector3& Value) = 0;
			virtual void SetVector4(_In_ const string& Name, _In_ const Vector4& Value) = 0;
			virtual void SetTexture(_In_ const string& Name, _In_ const Texture& Value) = 0;
		};
	}
}

#endif

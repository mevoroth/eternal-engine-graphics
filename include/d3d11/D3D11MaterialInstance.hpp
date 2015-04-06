#ifndef _D3D11_MATERIAL_INSTANCE_HPP_
#define _D3D11_MATERIAL_INSTANCE_HPP_

#include "Graphics/MaterialInstance.hpp"

namespace Eternal
{
	namespace Graphics
	{
		class D3D11MaterialInstance : public MaterialInstance
		{
		public:
			virtual void SetFloat(_In_ const string& Name, _In_ float Value) override;
			virtual void SetVector2(_In_ const string& Name, _In_ const Vector2& Value) override;
			virtual void SetVector3(_In_ const string& Name, _In_ const Vector3& Value) override;
			virtual void SetVector4(_In_ const string& Name, _In_ const Vector4& Value) override;
			virtual void SetTexture(_In_ const string& Name, _In_ const Texture& Value) override;
		};
	}
}

#endif

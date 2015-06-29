#ifndef _D3D11_RESOURCE_HPP_
#define _D3D11_RESOURCE_HPP_

#include "Graphics/Resource.hpp"

struct ID3D11ShaderResourceView;
struct ID3D11Buffer;
struct D3D11_SUBRESOURCE_DATA;

namespace Eternal
{
	namespace Graphics
	{
		class D3D11Resource : public Resource
		{
		public:
			enum Usage
			{
				DEFAULT,
				IMMUTABLE,
				DYNAMIC,
				STAGING
			};
			enum Bind
			{
				VERTEX_BUFFER = 0x1,
				INDEX_BUFFER = 0x2,
				CONSTANT_BUFFER = 0x4,
				SHADER_RESOURCE = 0x8,
				STREAM_OUTPUT = 0x10,
				RENDER_TARGET = 0x20,
				DEPTH_STENCIL = 0x40,
				UNORDERED_ACCESS = 0x80
			};
			D3D11Resource(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode, const Bind& BindMode);
			D3D11Resource(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode, const Bind& BindMode, void* Data);
			virtual ID3D11ShaderResourceView* GetD3D11ShaderResourceView() = 0;
			inline ID3D11Buffer* GetD3D11Buffer()
			{
				return _D3D11Buffer;
			}
#pragma region Resource
			virtual void* Lock(const CPUAccess& LockingMode) override;
			virtual void Unlock() override;
#pragma endregion Resource

		protected:
			void _CreateBuffer(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode, const Bind& BindMode, const D3D11_SUBRESOURCE_DATA* SubResourceData);

		private:
			CPUAccess _CPUAccess;
			ID3D11Buffer* _D3D11Buffer = nullptr;
		};
	}
}

#endif

#ifndef _D3D11_RESOURCE_HPP_
#define _D3D11_RESOURCE_HPP_

#include "Graphics/Resource.hpp"

struct ID3D11ShaderResourceView;
struct ID3D11Resource;
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
			virtual ~D3D11Resource();
			virtual ID3D11ShaderResourceView* GetD3D11ShaderResourceView() = 0;
#pragma region Resource
			virtual void* Lock(const CPUAccess& LockingMode) override;
			virtual void Unlock() override;
#pragma endregion Resource
			ID3D11Resource* GetD3D11Resource();

		protected:
			D3D11Resource();
			D3D11Resource(ID3D11Resource* ResourceObj);
			void _CreateBuffer(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode, const Bind& BindMode, const D3D11_SUBRESOURCE_DATA* SubResourceData);
			void _SetD3D11Resource(ID3D11Resource* D3D11ResourceObj);

		private:
			CPUAccess _CPUAccess;
			ID3D11Resource* _D3D11Resource = nullptr;
		};
	}
}

#endif

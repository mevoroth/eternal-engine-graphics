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
			enum Bind
			{
				VERTEX_BUFFER		= 0x1,
				INDEX_BUFFER		= 0x2,
				CONSTANT_BUFFER		= 0x4,
				SHADER_RESOURCE		= 0x8,
				STREAM_OUTPUT		= 0x10,
				RENDER_TARGET		= 0x20,
				DEPTH_STENCIL		= 0x40,
				UNORDERED_ACCESS	= 0x80
			};
			D3D11Resource(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode, const Bind& BindMode);
			D3D11Resource(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode, const Bind& BindMode, void* Data);
			virtual ~D3D11Resource();
#pragma region Resource
			virtual LockedResource Lock(Context& ContextObj, const LockMode& LockingMode) override;
			virtual void Unlock(Context& ContextObj) override;
#pragma endregion Resource
			ID3D11Resource* GetD3D11Resource();

		protected:
			static uint32_t CPUAccessToD3D11(_In_ const CPUAccess& CPUMode);

			D3D11Resource();
			D3D11Resource(ID3D11Resource* ResourceObj);
			void _CreateBuffer(size_t BufferSize, const Usage& UsageObj, const CPUAccess& CPUMode, const Bind& BindMode, const D3D11_SUBRESOURCE_DATA* SubResourceData);
			void _SetD3D11Resource(ID3D11Resource* D3D11ResourceObj);
			void _SetCPUAccess(const CPUAccess& CPUMode);

		private:
			CPUAccess _CPUAccess;
			ID3D11Resource* _D3D11Resource = nullptr;
		};
	}
}

#endif

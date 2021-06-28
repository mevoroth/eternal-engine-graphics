#pragma once

#include "Graphics/RootSignature.hpp"

struct ID3D12RootSignature;

//////////////////////////////////////////////////////////////////////////
//
//	https://docs.microsoft.com/en-us/windows/win32/direct3d12/root-signature-limits
//
//	Fast memory can only contains 16 DWORDs
//
//	Input Assembler cost 1 DWORD
//
//	Descriptor tables cost 1 DWORD each.
//	Root constants cost 1 DWORD each, since they are 32 - bit values.
//	Root descriptors(64-bit GPU virtual addresses) cost 2 DWORDs each.
//
//	Use a small a root signature as necessary, though balance this with the flexibility of a larger root signature.
//	Arrange parameters in a large root signature so that the parameters most likely to change often, or if low access latency for a given parameter is important, occur first.
//	If convenient, use root constants or root constant buffer views over putting constant buffer views in a descriptor heap.
//
//////////////////////////////////////////////////////////////////////////

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		enum class RootSignatureAccess;

		class Device;
		class Sampler;
		class DescriptorHeap;

		class D3D12RootSignature : public RootSignature
		{
		public:
			D3D12RootSignature(_In_ Device& InDevice);
			D3D12RootSignature(_In_ Device& InDevice, _In_ const RootSignatureCreateInformation& InRootSignatureCreateInformation);
			
			~D3D12RootSignature();

			ID3D12RootSignature* GetD3D12RootSignature() const { return _RootSignature; }

		private:
			ID3D12RootSignature* _RootSignature = nullptr;
		};
	}
}

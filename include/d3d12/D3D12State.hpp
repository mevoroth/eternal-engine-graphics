#ifndef _D3D12_STATE_HPP_
#define _D3D12_STATE_HPP_

class D3D12Device;
class InputLayout;
struct ID3D12PipelineState;
struct ID3D12RootSignature;

namespace Eternal
{
	namespace Graphics
	{
		class Shader;
		class DepthTest;
		class StencilTest;
		class BlendState;

		class D3D12State
		{
		public:
			D3D12State(_In_ D3D12Device& Device, _In_ InputLayout& InputLayoutObj, _In_ Shader* VS, _In_ Shader* PS, _In_ const DepthTest& DepthTestObj, _In_ const StencilTest& StencilTestObj, _In_ const BlendState& BlendStateObj);

		private:
			ID3D12RootSignature* _RootSignature = nullptr;
			ID3D12PipelineState* _PipelineState = nullptr;
		};
	}
}

#endif

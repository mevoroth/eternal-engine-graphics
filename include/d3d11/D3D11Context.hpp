#ifndef _D3D11_CONTEXT_HPP_
#define _D3D11_CONTEXT_HPP_

#include <vector>
#include "Graphics/Context.hpp"
#include "Time/Time.hpp"

#define D3D11_MAX_RENDERTARGETS 8

struct ID3D11DeviceContext;
struct ID3D11CommandList;

namespace Eternal
{
	namespace Graphics
	{
		using namespace Eternal::Time;
		using namespace std;
		class D3D11Context : public Context
		{
		public:
			D3D11Context(_In_ ID3D11DeviceContext* D3D11ContextObj, _In_ bool IsDeferred = false);
			virtual ~D3D11Context() override;
			
			virtual void PrepareFlush(Context& ContextObj) override;
			virtual void Flush() override;
			virtual void DrawIndexed(_In_ VertexBuffer* VerticesBuffer, _In_ IndexBuffer* IndicesBuffer) override;
			virtual void DrawDirect(_In_ VertexBuffer* VerticesBuffer) override;
			virtual void DrawPrimitive(_In_ uint32_t PrimitiveCount) override;
			virtual void SetRenderTargets(_In_ RenderTarget** RenderTargets, _In_ int RenderTargetsCount) override;
			virtual void SetDepthBuffer(_In_ RenderTarget* DepthBuffer) override;
			virtual void SetViewport(_In_ Viewport* ViewportObj) override;
			virtual void SetBlendMode(_In_ BlendState* BlendStateObj) override;
			virtual void SetTopology(_In_ const Topology& TopologyObj) override;
			virtual void BindDepthStencilState(_In_ DepthStencil* DepthStencilState) override;
			virtual void UnbindDepthStencilState() override;

			inline ID3D11DeviceContext* GetD3D11Context()
			{
				return _DeviceContext;
			}

		protected:
			virtual void _BindVertexShader(_In_ Shader* ShaderObj) override;
			virtual void _BindGeometryShader(_In_ Shader* ShaderObj) override;
			virtual void _BindPixelShader(_In_ Shader* ShaderObj) override;
			virtual void _UnbindVertexShader() override;
			virtual void _UnbindGeometryShader() override;
			virtual void _UnbindPixelShader() override;
			virtual void _BindVSConstant(_In_ uint32_t Slot, _In_ Constant* ConstantObj) override;
			virtual void _BindGSConstant(_In_ uint32_t Slot, _In_ Constant* ConstantObj) override;
			virtual void _BindPSConstant(_In_ uint32_t Slot, _In_ Constant* ConstantObj) override;
			virtual void _UnbindVSConstant(_In_ uint32_t Slot) override;
			virtual void _UnbindGSConstant(_In_ uint32_t Slot) override;
			virtual void _UnbindPSConstant(_In_ uint32_t Slot) override;
			virtual void _BindVSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj) override;
			virtual void _BindGSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj) override;
			virtual void _BindPSBuffer(_In_ uint32_t Slot, _In_ Resource* BufferObj) override;
			virtual void _UnbindVSBuffer(_In_ uint32_t Slot) override;
			virtual void _UnbindGSBuffer(_In_ uint32_t Slot) override;
			virtual void _UnbindPSBuffer(_In_ uint32_t Slot) override;
			virtual void _BindVSSampler(_In_ uint32_t Slot, _In_ Sampler* SamplerObj) override;
			virtual void _BindGSSampler(_In_ uint32_t Slot, _In_ Sampler* SamplerObj) override;
			virtual void _BindPSSampler(_In_ uint32_t Slot, _In_ Sampler* SamplerObj) override;
			virtual void _UnbindVSSampler(_In_ uint32_t Slot) override;
			virtual void _UnbindGSSampler(_In_ uint32_t Slot) override;
			virtual void _UnbindPSSampler(_In_ uint32_t Slot) override;
			virtual void BeginCommandList() override;
			virtual void EndCommandList() override;

		private:
			inline void _MarkRenderStateAsDirty()
			{
				_RenderStateDirty = true;
			}
			inline bool _IsRenderStateDirty() const
			{
				return _RenderStateDirty;
			}
			void _CommitRenderState();

			bool _RenderStateDirty = false;

			Viewport* _Viewport = nullptr;
			BlendState* _BlendState = nullptr;
			RenderTarget* _DepthBuffer = nullptr;
			RenderTarget* _RenderTargets[D3D11_MAX_RENDERTARGETS];

			ID3D11DeviceContext* _DeviceContext = nullptr;
			
			struct CommandListRecord
			{
				TimeMicroSecondsT Time = 0ull;
				ID3D11CommandList* CommandList = nullptr;
			};
			vector<CommandListRecord> _CommandLists;
		};
	}
}

#endif

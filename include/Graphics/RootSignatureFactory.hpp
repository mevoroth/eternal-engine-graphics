#pragma once

namespace Eternal
{
	namespace Graphics
	{
		struct RootSignatureCreateInformation;
		class GraphicsContext;
		class RootSignature;

		RootSignature* CreateRootSignature(_In_ GraphicsContext& InContext, _In_ const RootSignatureCreateInformation& InRootSignatureCreateInformation);
		RootSignature* CreateRootSignature(_In_ GraphicsContext& InContext);
		void DestroyRootSignature(_Inout_ RootSignature*& InOutRootSignature);
	}
}

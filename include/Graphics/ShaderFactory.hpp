#pragma once

#include <string>

namespace Eternal
{
	namespace Graphics
	{
		using namespace std;

		enum class ShaderType;

		class Shader;
		class GraphicsContext;
		struct ShaderCreateInformation;

		class ShaderFactory
		{
		public:
			ShaderFactory();
			virtual ~ShaderFactory();

			Shader* GetShader(_In_ GraphicsContext& Context, _In_ const ShaderCreateInformation& CreateInformation);
			void Create(_In_ GraphicsContext& Context, _In_ const ShaderCreateInformation& CreateInformation, _In_ Shader* InShader = nullptr);

		private:
			Shader* _Find(_In_ const string& Name);
			Shader* _Create(_In_ GraphicsContext& Context, _In_ const ShaderCreateInformation& CreateInformation, _In_ Shader* InShader = nullptr);

			vector<Shader*>	_Shaders;
		};
	}
}

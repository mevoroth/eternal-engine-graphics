#pragma once

namespace Eternal
{
	namespace Graphics
	{
		enum class FillMode
		{
			FILL_MODE_WIREFRAME = 0,
			FILL_MODE_SOLID,
			FILL_MODE_COUNT
		};

		enum class CullMode
		{
			CULL_MODE_NONE = 0,
			CULL_MODE_FRONT,
			CULL_MODE_BACK,
			CULL_MODE_COUNT
		};

		enum class FrontFace
		{
			FRONT_FACE_COUNTER_CLOCKWISE = 0,
			FRONT_FACE_CLOCKWISE,
			FRONT_FACE_COUNT
		};

		class Rasterizer
		{
		public:
			Rasterizer() {}
			Rasterizer(_In_ const FrontFace& InFrontFace)
				: _FrontFace(InFrontFace)
			{
			}
			Rasterizer(_In_ const FillMode& InFillMode)
				: _Fill(InFillMode)
				, _ConservativeRasterization(InFillMode == FillMode::FILL_MODE_SOLID)
			{
			}

			inline const FillMode& GetFillMode() const { return _Fill; }
			inline const CullMode& GetCullMode() const { return _Cull; }
			inline const FrontFace& GetFrontFace() const { return _FrontFace; }
			inline bool GetDepthClip() const { return _DepthClip; }
			inline int32_t GetDepthBias() const { return _DepthBias; }
			inline float GetDepthBiasClamp() const { return _DepthBiasClamp; }
			inline float GetDepthBiasSlopeScale() const { return _DepthBiasSlopeScale; }
			inline bool GetConservativeRasterization() const { return _ConservativeRasterization; }

		private:
			FillMode _Fill					= FillMode::FILL_MODE_SOLID;
			CullMode _Cull					= CullMode::CULL_MODE_BACK;
			FrontFace _FrontFace			= FrontFace::FRONT_FACE_COUNTER_CLOCKWISE;
			bool _DepthClip					= true;
			int32_t _DepthBias				= 0;
			float _DepthBiasClamp			= 0.0f;
			float _DepthBiasSlopeScale		= 0.0f;
			bool _ConservativeRasterization	= true;
		};

		extern const Rasterizer RasterizerDefault;
		extern const Rasterizer RasterizerClockWise;
		extern const Rasterizer RasterizerLine;
	}
}

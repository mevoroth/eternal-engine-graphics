#pragma once

#include "Graphics/ComparisonFunction.hpp"

namespace Eternal
{
	namespace Graphics
	{
		enum class AddressMode
		{
			ADDRESS_MODE_WRAP	= 0,
			ADDRESS_MODE_MIRROR,
			ADDRESS_MODE_CLAMP
		};

		enum class BorderColor
		{
			BORDER_COLOR_TRANSPARENT_BLACK,
			BORDER_COLOR_OPAQUE_BLACK,
			BORDER_COLOR_OPAQUE_WHITE,
			BORDER_COLOR_COUNT
		};

		struct SamplerCreateInformation
		{
			SamplerCreateInformation(
				_In_ bool InMINLinear = true,
				_In_ bool InMAGLinear = true,
				_In_ bool InMIPLinear = false,
				_In_ const AddressMode& InU = AddressMode::ADDRESS_MODE_WRAP,
				_In_ const AddressMode& InV = AddressMode::ADDRESS_MODE_WRAP,
				_In_ const AddressMode& InW = AddressMode::ADDRESS_MODE_WRAP
			)
				: MINLinear(InMINLinear)
				, MAGLinear(InMAGLinear)
				, MIPLinear(InMIPLinear)
				, U(InU)
				, V(InV)
				, W(InW)
			{
			}

			bool IsStatic					= false;
			AddressMode U					= AddressMode::ADDRESS_MODE_WRAP;
			AddressMode V					= AddressMode::ADDRESS_MODE_WRAP;
			AddressMode W					= AddressMode::ADDRESS_MODE_WRAP;
			bool MINLinear					= true;
			bool MAGLinear					= true;
			bool MIPLinear					= false;
			bool IsAnisotropic				= false;
			bool ComparisonEnabled			= false;
			ComparisonFunction Comparison	= ComparisonFunction::COMPARISON_FUNCTION_NEVER;
			float MIPLODBias				= 0.0f;
			uint32_t MaxAnisotropy			= 1;
			float MinLOD					= 0.0f;
			float MaxLOD					= FLT_MAX;
			BorderColor Border				= BorderColor::BORDER_COLOR_TRANSPARENT_BLACK;
		};

		struct StaticSamplerCreateInformation : public SamplerCreateInformation
		{
			StaticSamplerCreateInformation(
				_In_ bool InMINLinear = true,
				_In_ bool InMAGLinear = true,
				_In_ bool InMIPLinear = false,
				_In_ const AddressMode& InU = AddressMode::ADDRESS_MODE_WRAP,
				_In_ const AddressMode& InV = AddressMode::ADDRESS_MODE_WRAP,
				_In_ const AddressMode& InW = AddressMode::ADDRESS_MODE_WRAP
			)
				: SamplerCreateInformation(
					InMINLinear,
					InMAGLinear,
					InMIPLinear,
					InU,
					InV,
					InW
				)
			{
				IsStatic = true;
			}
		};

		class Sampler
		{
		public:
			virtual ~Sampler() {}

		protected:
			Sampler() {}
			Sampler(_In_ const SamplerCreateInformation& InSamplerCreateInformation)
				: _SamplerCreateInformation(InSamplerCreateInformation)
			{
			}

			bool IsStatic() const { return _SamplerCreateInformation.IsStatic; }

			bool IsComparisonEnabled() const { return _SamplerCreateInformation.ComparisonEnabled; }
			bool IsAnisotropic() const { return _SamplerCreateInformation.IsAnisotropic; }
			bool GetMINLinear() const { return _SamplerCreateInformation.MINLinear; }
			bool GetMAGLinear() const { return _SamplerCreateInformation.MAGLinear; }
			bool GetMIPLinear() const { return _SamplerCreateInformation.MIPLinear; }
			const AddressMode& GetU() const { return _SamplerCreateInformation.U; }
			const AddressMode& GetV() const { return _SamplerCreateInformation.V; }
			const AddressMode& GetW() const { return _SamplerCreateInformation.W; }
			float GetMipLODBias() const { return _SamplerCreateInformation.MIPLODBias; }
			uint32_t GetMaxAnisotropy() const { return _SamplerCreateInformation.MaxAnisotropy; }
			float GetMinLOD() const { return _SamplerCreateInformation.MinLOD; }
			float GetMaxLOD() const { return _SamplerCreateInformation.MaxLOD; }
			ComparisonFunction GetComparisonFunction() const { return _SamplerCreateInformation.Comparison; }
			BorderColor GetBorderColor() const { return _SamplerCreateInformation.Border; }

		private:
			SamplerCreateInformation _SamplerCreateInformation;
		};
	}
}

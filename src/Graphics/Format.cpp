#include "Graphics/Format.hpp"

namespace Eternal
{
	namespace Graphics
	{
		bool IsDepthStencilFormat(_In_ const Format& InFormat)
		{
			switch (InFormat)
			{
			case Format::FORMAT_D32:
			case Format::FORMAT_DS328:
				return true;

			default:
				break;
			}
			return false;
		}

		Format ConvertDepthStencilFormatToFormat(_In_ const Format& InFormat)
		{
			switch (InFormat)
			{
			case Format::FORMAT_D32:
				return Format::FORMAT_R32_FLOAT;
			case Format::FORMAT_DS328:
				return Format::FORMAT_RG328;

			default:
				break;
			}
			return InFormat;
		}
	}
}

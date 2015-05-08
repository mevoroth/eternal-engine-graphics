#ifndef _MATERIAL_PROPERTY_HPP_
#define _MATERIAL_PROPERTY_HPP_

#include <cstdint>
#include <string>

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		class MaterialProperty
		{
		public:
			enum ParamType
			{
				TEXTURE_T = 0,
				FLOAT_T = 1,
				VEC4_T = 2,
				PARAMTYPE_COUNT = 3
			};
			static uint32_t GetSize(_In_ const ParamType& Type);
		public:
			MaterialProperty(_In_ const string& Name, _In_ const ParamType& Type);
			string Name;
			ParamType Type;
		};
	}
}

#endif

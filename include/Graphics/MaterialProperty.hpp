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
				VEC2_T = 2,
				VEC3_T = 3,
				VEC4_T = 4,
				COLOR_T = 5,
				PARAMTYPE_COUNT = 6
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

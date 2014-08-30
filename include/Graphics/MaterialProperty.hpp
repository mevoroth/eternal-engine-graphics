#ifndef _MATERIAL_PROPERTY_HPP_
#define _MATERIAL_PROPERTY_HPP_

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
				TEXTURE_T,
				FLOAT_T,
				VEC2_T,
				VEC3_T,
				VEC4_T,
				COLOR_T
			};
		public:
			MaterialProperty(const string& name, const ParamType& type);
			string Name;
			ParamType Type;
		};
	}
}

#endif

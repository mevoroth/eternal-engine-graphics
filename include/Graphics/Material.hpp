#ifndef _MATERIAL_HPP_
#define _MATERIAL_HPP_

#include <vector>
#include <string>
#include <d3d11.h>

using namespace std;

namespace Eternal {
	namespace Graphics {
		class Material
		{
		public:
			enum ParamType
			{
				FLOAT_T,
				TEX_T,
				VEC2_T,
				VEC3_T,
				VEC4_T,
				COLOR_T
			};
			/**
			 * Set Material Params
			 */
			virtual void SetMaterialDesc(_In_ const string& paramName, _In_ const ParamType& paramType) = 0;
			/**
			 * Set vertex shader
			 */
			virtual void SetVertexShader(_In_ const string& shader) = 0;
			/**
			* Set geometry shader
			*/
			virtual void SetGeometryShader(_In_ const string& shader) = 0;
			/**
			 * Set pixel shader
			 */
			virtual void SetPixelShader(_In_ const string& shader) = 0;
		};
	}
}

#endif

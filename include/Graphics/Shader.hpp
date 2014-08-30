#ifndef _SHADER_HPP_
#define _SHADER_HPP_

#include <string>

using namespace std;

namespace Eternal
{
	namespace Graphics
	{
		class Shader
		{
		private:
			string _name;
		public:
			Shader(const string& name);
		};
	}
}

#endif

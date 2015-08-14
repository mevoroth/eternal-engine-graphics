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
			string _Name;
		public:
			Shader(const string& Name);
			const string& GetName() const;
		};
	}
}

#endif

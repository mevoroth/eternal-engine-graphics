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
		public:
			Shader(_In_ const string& Name);
			virtual ~Shader() {}
			const string& GetName() const;

		private:
			string _Name;
		};
	}
}

#endif

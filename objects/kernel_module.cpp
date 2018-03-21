#ifndef _kernel_module_
#define _kernel_module_

#include<string>
#include "object.cpp"

using namespace std;

class kernel_module : public object
{
	public:
		string file_path;

		string getinfo()
		{
			string json;

			return json;
		}
};

#endif

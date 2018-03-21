#ifndef _kernel_module_
#define _kernel_module_

#include <iostream>
#include <string>
#include <cstring>
#include "object.cpp"

using namespace std;

class kernel_module : public object
{
	public:
		string file_path;

		string get_info()
		{
			string json;
			json += "{ ";
			
			json += "\"name\" : ";
			json += this->name + ", ";
			
			json += "\"physical_offset\" : ";
			json += to_string(this->physical_offset) + ", ";

			json += "\"object_id\" : ";
			json += to_string(this->object_id) + ", ";

			json += "\"file_path\" : ";
			json += to_string(this->file_path) + ", ";

			json += "} ";
			return json;
		}
};

#endif

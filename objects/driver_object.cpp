#ifndef _kernel_module_
#define _kernel_module_

#include <vector>
#include <iomanip>
#include <fstream>
#include <iostream>
#include "object.cpp"

using namespace std;

class driver_object : public object
{
  public:
	string file_path;
	uint64_t driver_start;
	uint64_t driver_size;

	string get_info()
	{
		string json = "";
		json += "{ ";

		json += "\"name\" : ";
		json += "\"" + this->name + "\", ";

		json += "\"physical_offset\" : ";
		json += to_string(this->physical_offset) + ", ";

		json += "\"object_id\" : ";
		json += to_string(this->object_id) + ", ";

		json += "\"file_path\" : ";
		json += "\"" + this->file_path + "\", ";

		json += "\"marked\" : ";
		json += "\"enabled\", ";

		json += "\"comment\" : ";
		json += "\"\"";

		json += "} ";
		return json;
	}
};

#endif

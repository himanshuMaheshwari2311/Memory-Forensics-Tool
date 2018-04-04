#ifndef _dll_object_
#define _dll_object_

#include "object.cpp"

class dll_object : public object
{
  public:
    uint32_t pid;
    uint64_t dtb;
	uint64_t dll_base;
	uint32_t size_of_image;
	string full_dll_name;
    uint16_t load_count;
    string load_time;

	dll_object()
	{
	}

	string get_info()
	{
		string json = "";
		json += "{ ";

		json += "\"name\" : ";
		json += "\"" + this->name + "\", ";

		json += "\"full_dll_name\" : ";
		json += "\"" + this->full_dll_name + "\", ";

		json += "\"physical_offset\" : ";
		json += to_string(this->physical_offset) + ", ";

		json += "\"object_id\" : ";
		json += to_string(this->object_id) + ", ";

		json += "\"pid\" : ";
		json += to_string(this->pid) + ", ";

		json += "\"dll_base\" : ";
		json += to_string(this->dll_base) + ", ";

		json += "\"size_of_image\" : ";
		json += to_string(this->size_of_image) + ", ";

		json += "\"load_count\" : ";
		json += to_string(this->load_count) + ", ";
		
		json += "\"marked\" : ";
		json += "\"enabled\", ";

		json += "\"comment\" : ";
		json += "\"\"";

		json += "} ";
		return json;
	}
};

#endif
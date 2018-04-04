#ifndef _process_
#define _process_

#include "object.cpp"

class process : public object
{
  public:
	uint64_t dtb;
	uint64_t peb;
	uint32_t pid;
	uint32_t ppid;
	uint8_t active_threads;

	process()
	{
		pid = 0;
		ppid = 0;
		active_threads = 0;
	}

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

		json += "\"pid\" : ";
		json += to_string(this->pid) + ", ";

		json += "\"ppid\" : ";
		json += to_string(this->ppid) + ", ";

		json += "\"active_threads\" : ";
		json += to_string(this->active_threads) + ", ";
		
		json += "\"marked\" : ";
		json += "\"enabled\", ";

		json += "\"comment\" : ";
		json += "\"\"";

		json += "} ";
		return json;
	}
};

#endif
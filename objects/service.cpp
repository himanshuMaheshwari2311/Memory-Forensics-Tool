#ifndef _service_
#define _service_

#include "object.cpp"
#include <string>

class service : public object
{
  public:
	string display_name;
	string type;
	string state;
	string start;

	//Process thing left

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

		json += "\"display_name\" : ";
		json += "\"" + this->display_name + "\", ";

		json += "\"type\" : ";
		json += "\"" + this->type + "\", ";

		json += "\"state\" : ";
		json += "\"" + this->state + "\", ";

		json += "\"start\" : ";
		json += "\"" + this->start + "\"";

		json += "} ";
		return json;
	}

	string get_type(uint32_t index)
	{
		uint32_t temp = index;
		index = 0;
		while (temp)
		{
			index++;
			temp = temp >> 1;
		}
		if (index > 9)
			index = 9;
		string service_type[] =
			{
				"",
				"SERVICE_KERNEL_DRIVER",
				"SERVICE_FILE_SYSTEM_DRIVER",
				"",
				"",
				"SERVICE_WIN32_OWN_PROCESS",
				"SERVICE_WIN32_SHARE_PROCESS",
				"",
				"",
				"SERVICE_INTERACTIVE_PROCESS",
				""};
		return service_type[index];
	}

	string get_state(uint32_t index)
	{
		if (index > 8)
			index = 8;

		string service_state[] =
			{
				"",
				"SERVICE_STOPPED",
				"SERVICE_START_PENDING",
				"SERVICE_STOP_PENDING",
				"SERVICE_RUNNING",
				"SERVICE_CONTINUE_PENDING",
				"SERVICE_PAUSE_PENDING",
				"SERVICE_PAUSED",
				""};
		return service_state[index];
	}

	string get_start(uint32_t index)
	{
		if (index > 5)
			index = 5;
		string service_start[] =
			{
				"SERVICE_BOOT_START",
				"SERVICE_SYSTEM_START",
				"SERVICE_AUTO_START",
				"SERVICE_DEMAND_START",
				"SERVICE_DISABLED",
				""};
		return service_start[index];
	}
};

#endif
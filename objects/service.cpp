#ifndef _service_
#define _service_

#include "object.cpp"

class service : public object
{
  public:
	string display_name;
	string driver_name;
	string process_binary_path;
	uint8_t pid;
	//service type, service state and process thing left
	string get_info()
	{
		//json left
		return "This is a Service Object";
	}
};

#endif
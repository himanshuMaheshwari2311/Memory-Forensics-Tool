#include "object.cpp"

class process: public object
{
	private:
		string display_name;
		string driver_name;
		string process_binary_path;
		uint8_t pid;
	
	public:
		string getinfo()
		{
            return "This is a Service Object";
		}
};
#ifndef _registry_
#define _registry_

#include <string>
#include <vector>
#include "object.cpp"

using namespace std;

class registry : public object
{

  public:
    string file_path;
    string key_name;
    vector<string> subkey_list;

    string get_info()
    {
        	string json;
			json += "{ ";
			
			json += "\"name\" : ";
			json += "\"" + this->name + "\", ";
			
			json += "\"physical_offset\" : ";
			json += to_string(this->physical_offset) + ", ";

			json += "\"object_id\" : ";
			json += to_string(this->object_id) + ", ";

			json += "\"file_path\" : ";
			json += "\"" + this->file_path + "\"";

			json += "} ";
			return json;
    }
};

#endif
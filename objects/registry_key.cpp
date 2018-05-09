#ifndef _registry_key_
#define _registry_key_

#include <string>
#include <vector>
#include "object.cpp"

using namespace std;

class registry_key : public object
{

  public:
    string file_path;
    string key_name;
    string type;
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
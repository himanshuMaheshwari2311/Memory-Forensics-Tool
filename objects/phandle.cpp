#ifndef _p_handle_
#define _phandle_

#include "object.cpp"

class phandle : public object
{
  public:
    uint32_t pid;
    string type;
    uint64_t handle;
    uint64_t access;

    phandle()
    {
    }

    string get_info()
    {
        string json = "";
        json += "{ ";

        json += "\"name\" : ";
        json += "\"" + this->name + "\", ";

        json += "\"type\" : ";
        json += "\"" + this->type + "\", ";

        json += "\"physical_offset\" : ";
        json += to_string(this->physical_offset) + ", ";

        json += "\"object_id\" : ";
        json += to_string(this->object_id) + ", ";

        json += "\"pid\" : ";
        json += to_string(this->pid) + ", ";

        json += "\"marked\" : ";
        json += "\"enabled\", ";

        json += "\"comment\" : ";
        json += "\"\"";

        json += "} ";
        return json;
    }
};

#endif
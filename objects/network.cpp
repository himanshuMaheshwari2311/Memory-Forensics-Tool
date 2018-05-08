#ifndef _network_
#define _network_

#include <string>
#include <vector>
#include "object.cpp"

using namespace std;

class network : public object
{

  public:
    string protocol_version;
    string local_address;
    string foreign_address;
    uint32_t pid;
    uint16_t port;
    string owner_name;

    string get_info()
    {
        string json = "";
        json += "{ ";

        json += "\"object_id\" : ";
        json += to_string(this->object_id) + ", ";

        json += "\"protocol_version\" : ";
        json += "\"" + this->protocol_version + "\", ";

        json += "\"physical_offset\" : ";
        json += to_string(this->physical_offset) + ", ";

        json += "\"local_address\" : ";
        json += "\"" + this->local_address + "\", ";

        json += "\"foreign_address\" : ";
        json += "\"" + this->foreign_address + "\", ";

        json += "\"port\" : ";
        json += "\"" + to_string(this->port) + "\", ";

        json += "\"owner_process\" : ";
        json += "\"" + this->owner_name + "\", ";

        json += "\"pid\" : ";
        json += to_string(this->pid) + ", ";

        /*
        Removing for now
        json += "\"create_time\" : ";
        json += "\"" + this->create_time + "\", ";
        */ 

        json += "\"marked\" : ";
        json += "\"enabled\", ";

        json += "\"comment\" : ";
        json += "\"\"";

        json += "} ";


        return json;
    }
};

#endif
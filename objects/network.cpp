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
        return "network";
    }
};

#endif
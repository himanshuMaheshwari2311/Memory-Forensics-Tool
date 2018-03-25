#ifndef _registry_
#define _registry_

#include <string>
#include <vector>
#include "object.cpp"

using namespace std;

class registry : public object
{

  public:
    string protocol_version;
    string local_address;
    string foreign_address;
    uint32_t pid;
    string owner_name;

    string get_info()
    {

    }
};

#endif
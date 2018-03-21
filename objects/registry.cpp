#ifndef _registry_
#define _registry_

#include <string>
#include <vector>
#include "object.cpp"

using namespace std;

class registry : public object
{

  public:
    char *file_path;
    string key_name;
    vector<string> subkey_list;

    string getinfo()
    {
        return "this is a registy object";
    }
};

#endif
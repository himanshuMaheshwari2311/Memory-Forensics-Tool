#include <iostream>
#include <string>
#include <vector>
#include "object.cpp"


using namespace std;

class registry_module : public object
{
    private:
        string file_path;
        string key_name;
        vector<string> subkey_list;
    
    public:
        string getinfo()
        {
            return "this is a registy object";
        }

};

#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h>
#include "../objects/registry_module.cpp"
#include "../utils/utility_function.cpp"

using namespace std;

class registry_handler 
{
    public:
        vector<registry_module> modules;

    private:
        vector<unit64_t> pool_scan_tag(ifstream file, profile prf)
        {
            vector<uint64_t> some_name;
            char pattern[8];
            while(file.eof() == 0)
            {
                file.read(pattern, 8);
                if(utility_function.scan_tag(pattern, prf.hive_pool_tag, 8))
                {
                    
                }
            }

        }

        void generate_registry_modules(ifstream file, profile prf)
        {

        }
};

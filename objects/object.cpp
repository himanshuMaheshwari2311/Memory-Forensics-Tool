#include <string>
#include <stdint.h>

using namespace std;

enum object_type {process, service, kernal_module, hive, lsa, shin_cache, shell_bags};

class object
{
private:
    string name;
    uint64_t physical_offset;
    uint8_t object_id;
    enum object_type type; 

public:
    virtual string getinfo()=0;
};
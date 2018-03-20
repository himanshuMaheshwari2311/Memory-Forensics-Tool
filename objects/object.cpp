#include <string>
#include <stdint.h>

using namespace std;

enum object_type {process, service, kernal_module, hive, lsa, shin_cache, shell_bags};

typedef
struct datetime
{
    uint8_t day;
    uint8_t month;
    uint8_t year;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
}datetime;

class object
{
    private:
        string name;
        uint64_t physical_offset;
        uint8_t object_id;
        enum object_type type; 
        datetime timestamp;

    public:
        virtual string getinfo()=0;
};
#include "object.cpp"

class process: public object
{
    public:
        uint32_t pid;
        uint32_t ppid;
        uint8_t active_threads;

        string getinfo(){
            return "This is a Process Object";
        }
};
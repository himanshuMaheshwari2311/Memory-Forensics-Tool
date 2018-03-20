#include "object.cpp"

class process: public object
{
    public:
        uint8_t pid;
        uint8_t ppid;
        uint8_t active_threads;

        string getinfo(){
            return "This is a Process Object";
        }
};
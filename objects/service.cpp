#include "object.cpp"

class process: public object
{
    private:
        uint8_t pid;
        uint8_t ppid;
        uint8_t active_threads;

    public:
        string getinfo(){
            return "This is a Process Object";
        }
};
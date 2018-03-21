#define mainfunc
#include "handlers/process_handler.cpp"
#include "handlers/service_handler.cpp"
#include "handlers/registry_handler.cpp"
#include "handlers/kernel_module_handler.cpp"

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        return -1;
    }
    string file_name = argv[1];
    int type;
    if(strcmp(argv[2], "10"))
    {
        type = 10;
    } 
    else
    {
        type = 7;
    }
    process_handler ph;
    service_handler sh;
    registry_handler rh;
    kernel_module_handler kmh;

    vector<process> process_list;
    vector<service> service_list;
    vector<registry> registry_list;
    vector<kernel_module> kernel_list;
    
    return 0;
}
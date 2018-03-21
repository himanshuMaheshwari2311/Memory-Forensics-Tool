#define mainfunc

#include <fstream>
#include <cstring>
#include <iostream>

#include "handlers/process_handler.cpp"
#include "handlers/service_handler.cpp"
#include "handlers/registry_handler.cpp"
#include "handlers/kernel_module_handler.cpp"

using namespace std;

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        return -1;
    }
    char* fname = argv[1];
    profile prf;
    if(strcmp(argv[2], "10") == 0)
    {
        prf.init_as_win10();
    } 
    else
    {
        prf.init_as_win7();
    }
    
    ifstream ifile;
    ifile.open(fname, ios::in | ios::binary);
	if (!ifile)
	{
		cout << "Error in opening file..!!";
		return -1;
	}

    process_handler ph;
    service_handler sh;
    registry_handler rh;
    kernel_module_handler kmh;
    
    vector<process> process_list = ph.get_process_list(ifile, prf);
    vector<service> service_list;
    vector<registry> registry_list;
    vector<kernel_module> kernel_list;
        
    return 0;
}
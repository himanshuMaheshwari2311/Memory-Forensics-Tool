#define mainfunc

#include <fstream>
#include <cstring>
#include <vector>
#include <sstream>
#include <iterator>
#include <iostream>
#include <string.h>
#include <ctime>

#include "handlers/process_handler.cpp"
#include "handlers/dll_object_handler.cpp"
#include "handlers/phandle_handler.cpp"
#include "handlers/service_handler.cpp"
#include "handlers/registry_handler.cpp"
#include "handlers/kernel_module_handler.cpp"
#include "handlers/network_handler.cpp"

using namespace std;

vector<string> string_to_vector(string s)
{
	stringstream ss(s);
	istream_iterator<string> begin(ss);
	istream_iterator<string> end;
	vector<string> vstrings(begin, end);
	return vstrings;
}

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		return -1;
	}

	char *fname = argv[1];
	profile prf;
	if (strcmp(argv[2], "10") == 0)
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
    dll_object_handler dh;
    phandle_handler hh;
    service_handler sh;
    registry_handler rh;
    kernel_module_handler kh;
    network_handler nh;

	cout << "Generating Registry List" << endl;
    vector<registry> registry_list = rh.get_registry_list(ifile, prf);
    cout << "Generating Process List" << endl;
    vector<process> process_list = ph.get_process_list(ifile, prf);
    cout << "Generating Dll List" << endl;
    vector<dll_object> dll_object_list = dh.get_dll_object_list(ifile, prf, ph);
    cout << "Generating Phandle List" << endl;
    vector<phandle> phandle_list = hh.get_phandle_list(ifile, prf, ph);
    cout << "Generating Service List" << endl;
    vector<service> service_list = sh.get_service_list(ifile, prf);
    cout << "Generating Kernel List" << endl;
    vector<kernel_module> kernel_list = kh.get_kernel_list(ifile, prf);
    cout<< "Generating Network List" << endl;
    vector<network> network_list = nh.get_network_list(ifile, prf);

	string command = "start";

	do
	{
		vector<string> vcmd = string_to_vector(command);
		
		int pid;
		bool pidPresent = false;
		
		if(vcmd.size() == 0)
		{
			cout<<"Try again\n";
		}
		else if(vcmd[0] == "start")
		{
			//To print help at start
		}
		else
		{
			if(vcmd.size() == 3)
			{
				if(vcmd[1] == "-p")
				{
					pid = stoi(vcmd[2]);
					pidPresent = true;
				}
			}

			if (vcmd[0] == "dlllist")
			{
				if(pidPresent)
				{
					dh.print_dlls(pid);
				}
				else
				{
					dh.print_dlls();
				}
			}
			else if (vcmd[0] == "kernel")
			{
				cout<<"Kernel Modules in Linked List\n";
				kh.print_ll();
				cout<<"Unlinked Kernel Modules\n";
				kh.print_unlinked_modules();
			}
			else if(vcmd[0] == "network")
			{
				nh.print_network_connections();
			}
			else if (vcmd[0] == "phandle")
			{
				if(pidPresent)
				{
					hh.print_phandles(pid);
				}
				else
				{
					hh.print_phandles();
				}
			}
			else if (vcmd[0] == "process")
			{
				ph.print_processes();
			}
			else if (vcmd[0] == "service")
			{
				sh.print_services();
			}
			
			else
			{
				cout << "Try again\n";
			}
		}

		cout<<"\n>>";
		getline(cin, command);
	} while (command != "exit");

	return 0;
}
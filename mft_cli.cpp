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
	// if (argc < 2)
	// {
	// 	return -1;
	// }

	// char *fname = argv[1];
	// profile prf;
	// if (strcmp(argv[2], "10") == 0)
	// {
	// 	prf.init_as_win10();
	// }
	// else
	// {
	// 	prf.init_as_win7();
	// }

	// ifstream ifile;
	// ifile.open(fname, ios::in | ios::binary);
	// if (!ifile)
	// {
	// 	cout << "Error in opening file..!!";
	// 	return -1;
	// }
	// process_handler ph;
	// service_handler sh;
	// registry_handler rh;
	// kernel_module_handler kh;
	// network_handler nh;

	// cout << "Generating Registry List" << endl;
	// vector<registry> registry_list = rh.get_registry_list(ifile, prf);
	// cout << "Generating Process List" << endl;
	// vector<process> process_list = ph.get_process_list(ifile, prf);
	// cout << "Generating Service List" << endl;
	// vector<service> service_list = sh.get_service_list(ifile, prf);
	// cout << "Generating Kernel List" << endl;
	// vector<kernel_module> kernel_list = kh.get_kernel_list(ifile, prf);
	// cout << "Generating Network List" << endl;
	// vector<network> network_list = nh.get_network_list(ifile, prf);

	string command = "start";

	do
	{
		vector<string> vcmd = string_to_vector(command);
		
		int pid, port;
		string name;
		
		if(vcmd.size() == 0)
		{
			cout<<"Try again\n";
		}
		else if(vcmd[0] == "start")
		{
			cout<<"Command: exit\n\n";

			cout<<"Command: network\n";
			cout<<"\t-i: pid\n";
			cout<<"\t-p: port\n";
			cout<<"\t-n: name\n\n";
		}
		else
		{
			for(int i = 1; i < vcmd.size(); ++i)		//parameters
			{
				if(vcmd[i] == "-i")
				{
					pid = stoi(vcmd[i + 1]);
					++i;
				}
				else if(vcmd[i] == "-p")
				{
					port = stoi(vcmd[i + 1]);
					++i;
				}
				else if(vcmd[i] == "-n")
				{
					name = vcmd[i + 1];
					++i;
				}
			}

			if (vcmd[0] == "network")
			{
				cout << "In network\n";

				cout<<pid<<" "<<port<<" "<<name<<"\n";
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
#ifndef _process_handler_
#define _process_handler_

#include <vector>
#include <iomanip>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <string>

#include "../objects/process.cpp"
#include "../data/profiles.cpp"
#include "../utils/utility_functions.cpp"

using namespace std;

class process_handler
{
  private:
	vector<process> process_list;

  public:
	void generate_processes(ifstream &ifile, profile prf)
	{
		ifile.clear();
		ifile.seekg(0);
		uint64_t addr_val = 0;
		uint64_t base_addr = 0;
		char found_pattern[8];
		char p_name[16];
		cout << setw(16) << "Address" << setw(16) << "PID" << setw(16) << "PPID" << setw(16) << "Name" << endl;
		while (ifile.eof() == 0)
		{
			ifile.read(found_pattern, 8);
			addr_val += 8;
			if (utility_functions::compare_array(prf.process_signature, found_pattern, 8))
			{
				process proc;
				base_addr = addr_val - 8;
				cout << setw(16) << hex << addr_val - 8;

				proc.physical_offset = addr_val - 8;

				ifile.ignore(prf.process_offsets[0]);
				addr_val += prf.process_offsets[0];

				ifile.read(reinterpret_cast<char *>(&proc.pid), sizeof(proc.pid));
				addr_val += 4;
				cout << setw(16) << dec << proc.pid;

				ifile.ignore(prf.process_offsets[1]);
				addr_val += prf.process_offsets[1];

				ifile.read(reinterpret_cast<char *>(&proc.ppid), sizeof(proc.ppid));
				addr_val += 4;
				cout << setw(16) << proc.ppid;

				ifile.ignore(prf.process_offsets[2]);
				addr_val += prf.process_offsets[2];

				ifile.read(p_name, 16);
				addr_val += 16;
				proc.name = p_name;
				if (strcmp(p_name, "services.exe") == 0)
				{
					ifile.seekg(-(prf.process_name_offset + 16), std::ios::cur);
					ifile.seekg(prf.dtb_offset, std::ios::cur);
					uint64_t temp_dtb;
					ifile.read(reinterpret_cast<char *>(&temp_dtb), sizeof(temp_dtb));
					ifile.seekg((prf.process_name_offset + 16 - prf.dtb_offset + 8), std::ios::cur);
					prf.service_dtb = temp_dtb;
				}
				cout << setw(16) << proc.name << endl;

				ifile.ignore(prf.process_offsets[3]);
				addr_val += prf.process_offsets[3];
				if(proc.pid % 4 == 0 && proc.ppid % 4 == 0 && proc.pid < 32768) // pids are divisible by 4
				{
					//is a valid EPROCESS
					process_list.push_back(proc);					
				}
			}
			else
			{
				addr_val += 8;
				ifile.ignore(8);
			}
		}
	}
	vector<process> get_process_list(ifstream &ifile, profile prf)
	{
		if (process_list.empty())
		{
			generate_processes(ifile, prf);
		}
		return process_list;
	}
	void print_processes()
	{
		cout << setw(16) << "Address" << setw(16) << "PID" << setw(16) << "PPID" << setw(16) << "Name" << endl;
		process proc;
		for (int i = 0; i < process_list.size(); i++)
		{
			proc = process_list[i];
			cout << setw(16) << hex << proc.physical_offset << setw(16) << dec << proc.pid << setw(16) << proc.ppid << setw(16) << proc.name << " " << proc.object_id << endl;
		}
	}
	string get_info()
	{
		string json = "";
		json += "{ ";
		json += "\"process_list\" : ";
		json += "[ ";
		for (int i = 0; i < process_list.size(); ++i)
		{
			json += process_list[i].get_info();
			if (i != process_list.size() - 1)
				json += ",";
			json += "\n";
		}
		json += "] ";
		json += "} ";

		return json;
	}
};

#ifndef mainfunc
int main(void)
{
	process_handler ph;
	ifstream ifile;
	profile prf(10);
	char fname[] = "../data/samples/win1064.vmem";

	ifile.open(fname, ios::in | ios::binary);
	if (!ifile)
	{
		cout << "Error in opening file..!!";
		return -1;
	}
	cout << "File opened..";
	cout << "\n";
	cout << prf.get_global_dtb(ifile) << endl;
	cout << prf.get_service_dtb(ifile) << endl;
	//ph.generate_processes(ifile, prf);
	//ph.print_processes();
	//cout<<ph.get_info();
	//ph.generate_processes(ifile, prf);
}
#endif
#endif
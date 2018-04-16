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
		utility_functions ::reset_file(ifile);
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

				ifile.seekg(base_addr + prf.process_offsets[1]);

				ifile.read(reinterpret_cast<char *>(&proc.pid), sizeof(proc.pid));
				cout << setw(16) << dec << proc.pid;

				ifile.seekg(base_addr + prf.process_offsets[2]);

				ifile.read(reinterpret_cast<char *>(&proc.ppid), sizeof(proc.ppid));
				cout << setw(16) << proc.ppid;

				ifile.seekg(base_addr + prf.process_offsets[3]);

				ifile.read(p_name, 16);
				proc.name = p_name;
				if (strcmp(p_name, "services.exe") == 0)
				{
					ifile.seekg(-(prf.process_name_offset + 16), std::ios::cur);
					ifile.seekg(prf.dtb_offset, std::ios::cur);
					uint64_t temp_dtb;
					ifile.read(reinterpret_cast<char *>(&temp_dtb), sizeof(temp_dtb));
					ifile.seekg((prf.process_name_offset - prf.dtb_offset + 8), std::ios::cur);
					prf.service_dtb = temp_dtb;
				}
				cout << setw(16) << proc.name;

				ifile.seekg(base_addr + prf.process_offsets[5], ios::beg);
				ifile.read(reinterpret_cast<char *>(&proc.dtb), sizeof(proc.dtb));

				ifile.seekg(base_addr + prf.process_offsets[6]);
				ifile.read(reinterpret_cast<char *>(&proc.peb), sizeof(proc.peb));

				cout << setw(16) << hex << proc.dtb;
				cout << setw(16) << hex << proc.peb;

				ifile.seekg(base_addr + prf.process_offsets[7]);
				ifile.read(reinterpret_cast<char *>(&proc.flink), sizeof(proc.flink));
				proc.flink = utility_functions ::opt_get_phy_addr(ifile, proc.flink, prf.get_global_dtb(ifile));
				cout << setw(16) << hex << proc.flink << endl;

				if (proc.pid % 4 == 0 && proc.ppid % 4 == 0 && proc.pid < 32768) // pids are divisible by 4
				{
					//is a valid EPROCESS
					process_list.push_back(proc);
				}
				addr_val += prf.process_offsets[0] - 8;
				ifile.seekg(base_addr + prf.process_offsets[0]);
			}
			else
			{
				addr_val += 8;
				ifile.ignore(8);
			}
		}
		scan_unlinked(ifile, prf);
	}
	void scan_unlinked(ifstream &ifile, profile prf)
	{
		uint64_t temp_addr;
		for (int i = 0; i < process_list.size(); i++)
		{
			temp_addr = process_list[i].flink;
			if(process_list[i].name == "Idle" || process_list[i].name == "System")
			{
					process_list[i].unlinked = "No";				
			}
			for (int j = 0; j < process_list.size(); j++)
			{
				if (temp_addr == process_list[j].physical_offset + prf.process_offsets[7])
				{
					process_list[j].unlinked = "No";
					break;
				}
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
		cout << endl;
		cout << setw(16) << "Address" << setw(16) << "PID" << setw(16) << "PPID" << setw(16) << "Name" << setw(16) << "Unlinked?" << endl;
		process proc;
		for (int i = 0; i < process_list.size(); i++)
		{
			proc = process_list[i];
			cout << setw(16) << hex << proc.physical_offset << setw(16) << dec << proc.pid << setw(16) << proc.ppid << setw(16) << proc.name << setw(16) << proc.unlinked << endl;
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
	profile prf(7);
	char fname[] = "../data/samples/win764.vmem";

	ifile.open(fname, ios::in | ios::binary);
	if (!ifile)
	{
		cout << "Error in opening file..!!";
		return -1;
	}
	cout << "File opened..";
	cout << "\n";
	//cout << prf.get_global_dtb(ifile) << endl;
	//cout << prf.get_service_dtb(ifile) << endl;
	prf.get_global_dtb(ifile);
	ph.generate_processes(ifile, prf);
	ph.print_processes();
	//cout<<ph.get_info();
	//ph.generate_processes(ifile, prf);
}
#endif
#endif
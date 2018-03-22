#ifndef _kernel_module_handler_
#define _kernel_module_handler_

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>

#include "../objects/kernel_module.cpp"
#include "../data/profiles.cpp"
#include "../utils/utility_functions.cpp"

using namespace std;

class kernel_module_handler
{
  private:
	vector<kernel_module> kernel_list;

  public:
	vector<uint64_t> pool_tag_scan(ifstream &ifile, profile prf)
	{

		vector<uint64_t> phy_offsets;

		uint64_t addr_val = 0;
		char curr_pattern[8];

		uint64_t prev_size, pool_index, block_size, pool_type;

		while (ifile.eof() == 0)
		{
			ifile.read(curr_pattern, 8);
			addr_val += 8;
			if (utility_functions ::scan_tag(prf.kernel_pool_tag, curr_pattern, 8))
			{
				prev_size = curr_pattern[0] * 16;
				pool_index = curr_pattern[1];
				block_size = curr_pattern[2] * 16; //16 for 64bits and 8 for 32bits OS
				pool_type = curr_pattern[3];

				if (block_size <= 32)
				{
					ifile.ignore(8);
					addr_val += 8;
					continue;
				}

				phy_offsets.push_back(addr_val - 8);
				cout << hex << addr_val - 8 << "\n";

				ifile.ignore(8);
				addr_val += 8;
			}
			else
			{
				ifile.ignore(8);
				addr_val += 8;
			}
		}

		return phy_offsets;
	}

	kernel_module collect_info_module(ifstream &ifile, profile prf, uint64_t phy_offset)
	{
		kernel_module curr_module;
		curr_module.physical_offset = phy_offset;

		uint64_t name_addr, phy_name_addr, file_addr, phy_file_addr;
		uint16_t name_size;

		ifile.clear();
		ifile.seekg(phy_offset + prf.kernel_offsets[0], ios::beg);

		ifile.read(reinterpret_cast<char *>(&file_addr), sizeof(file_addr)); //0x60 for ptr to file path

		ifile.read(reinterpret_cast<char *>(&name_size), sizeof(name_size)); //0x68 size of name
		ifile.ignore(prf.kernel_offsets[1]);

		ifile.read(reinterpret_cast<char *>(&name_addr), sizeof(name_addr)); //0x70 ptr64 to name
		phy_name_addr = utility_functions ::opt_get_phy_addr(ifile, name_addr, 0x001ab000);

		ifile.clear();
		ifile.seekg(phy_name_addr, ios::beg);
		char name[name_size * 2];
		ifile.read(name, sizeof(name));
		curr_module.name = utility_functions ::get_utf_str(name, sizeof(name));
		curr_module.name.erase(remove_if(curr_module.name.begin(), curr_module.name.end(), utility_functions ::invalidChar), curr_module.name.end()); ///Need to copy this
		cout << curr_module.name << " ";

		phy_file_addr = utility_functions ::opt_get_phy_addr(ifile, file_addr, 0x001ab000);

		ifile.clear();
		ifile.seekg(phy_file_addr, ios::beg);
		char file_path[128];
		ifile.read(file_path, sizeof(file_path));
		curr_module.file_path = utility_functions ::get_utf_str(file_path, sizeof(file_path));
		replace(curr_module.file_path.begin(), curr_module.file_path.end(), '\\', '/');
		curr_module.file_path.erase(remove_if(curr_module.file_path.begin(), curr_module.file_path.end(), utility_functions ::invalidChar), curr_module.file_path.end());

		cout << curr_module.file_path << " ";

		cout << "\n";
		return curr_module;
	}

	void generate_kernel_modules(ifstream &ifile, profile prf)
	{
		ifile.clear();
		ifile.seekg(0, ios::beg);

		vector<uint64_t> phy_offsets = pool_tag_scan(ifile, prf);

		for (int i = 0; i < phy_offsets.size(); ++i)
		{
			kernel_list.push_back(collect_info_module(ifile, prf, phy_offsets[i]));
		}
	}

	vector<kernel_module> get_kernel_list(ifstream &ifile, profile prf)
	{
		if (kernel_list.empty())
		{
			generate_kernel_modules(ifile, prf);
		}
		return kernel_list;
	}

	string get_info()
	{
		string json = "";

		json += "{ ";
		json += "\"kernel_list\" : ";
		json += "[ ";
		for (int i = 0; i < kernel_list.size(); ++i)
		{
			json += kernel_list[i].get_info();
			if (i != kernel_list.size() - 1)
				json += ",";
			json += "\n";
		}
		json += "] ";
		json += "} ";

		return json;
	}
};

#ifndef mainfunc
int main()
{
	kernel_module_handler kh;
	ifstream ifile;
	profile prf(10);
	char fname[] = "../data/samples/win1064vir.vmem";

	ifile.open(fname, ios::in | ios::binary);
	if (!ifile)
	{
		cout << "Error in opening file..!!";
	}
	cout << "File opened..";
	cout << "\n";

	kh.get_kernel_list(ifile, prf);

	cout << kh.get_info() << endl;
}
#endif
#endif
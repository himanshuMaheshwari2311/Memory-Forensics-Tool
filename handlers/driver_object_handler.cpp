#ifndef _driver_object_handler_
#define _driver_object_handler_

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <unordered_map>

#include "../objects/driver_object.cpp"
#include "../data/profiles.cpp"
#include "../utils/utility_functions.cpp"

using namespace std;

class driver_object_handler
{
  private:
	vector<driver_object> driver_list;
	
  public:
	vector<uint64_t> pool_tag_scan(ifstream &ifile, profile &prf)
	{
		vector<uint64_t> phy_offsets;

		uint64_t addr_val = 0;
		char curr_pattern[8];

		uint64_t prev_size, pool_index, block_size, pool_type;

		while (ifile.eof() == 0)
		{
			ifile.read(curr_pattern, 8);
			addr_val += 8;
			if (utility_functions ::scan_tag(prf.driver_pool_tag, curr_pattern, 8))
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

				phy_offsets.push_back(addr_val - 8 + prf.driver_phy_offset);
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

	driver_object collect_info_object(ifstream &ifile, profile &prf, uint64_t phy_offset)
	{
		driver_object curr_object;
		curr_object.physical_offset = phy_offset;
		
		uint64_t name_addr, phy_name_addr, file_addr, phy_file_addr;
		uint16_t name_size;

		ifile.clear();

		ifile.seekg(phy_offset + prf.driver_offsets[0]);
		ifile.read(reinterpret_cast<char *>(&name_size), sizeof(name_size));

		ifile.seekg(phy_offset + prf.driver_offsets[1]);
		ifile.read(reinterpret_cast<char *>(&name_addr), sizeof(name_addr));

		phy_name_addr = utility_functions ::opt_get_phy_addr(ifile, name_addr, prf.get_global_dtb(ifile));

		ifile.clear();
		ifile.seekg(phy_name_addr);
		char name[name_size * 2];
		ifile.read(name, sizeof(name));
		curr_object.name = utility_functions ::get_utf_str(name, sizeof(name));
		curr_object.name.erase(remove_if(curr_object.name.begin(), curr_object.name.end(), utility_functions ::invalidChar), curr_object.name.end());
		cout << curr_object.name << " ";

		cout << "\n";
		return curr_object;
	}

	void generate_driver_object(ifstream &ifile, profile &prf)
	{
		ifile.clear();
		ifile.seekg(0, ios::beg);

		vector<uint64_t> phy_offsets = pool_tag_scan(ifile, prf);
		driver_object curr;

		for (int i = 0; i < phy_offsets.size(); ++i)
		{
			curr = collect_info_object(ifile, prf, phy_offsets[i]);
			driver_list.push_back(curr);
		}

	}

	vector<driver_object> get_driver_list(ifstream &ifile, profile &prf)
	{
		if (driver_list.empty())
		{
			generate_driver_object(ifile, prf);
		}
		return driver_list;
	}

	string get_info()
	{
		string json = "";

		json += "{ ";
		json += "\"driver_list\" : ";
		json += "[ ";
		for (int i = 0; i < driver_list.size(); ++i)
		{
			json += driver_list[i].get_info();
			if (i != driver_list.size() - 1)
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
	driver_object_handler dh;
	ifstream ifile;
	profile prf(7);
	char fname[] = "../data/samples/win764.vmem";

	ifile.open(fname, ios::in | ios::binary);
	if (!ifile)
	{
		cout << "Error in opening file..!!";
	}
	cout << "File opened..";
	cout << "\n";

	dh.get_driver_list(ifile, prf);

	cout << dh.get_info() << endl;
}
#endif
#endif
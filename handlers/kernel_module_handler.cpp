#ifndef _kernel_module_handler_
#define _kernel_module_handler_

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <unordered_map>

#include "../objects/kernel_module.cpp"
#include "../data/profiles.cpp"
#include "../utils/utility_functions.cpp"

using namespace std;

class kernel_module_handler
{
  private:
	vector<kernel_module> kernel_list;
	unordered_map<uint64_t, kernel_module> kernel_map;
	doubly_ll<kernel_module> *kernel_ll_head;

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

	kernel_module collect_info_module(ifstream &ifile, profile &prf, uint64_t phy_offset)
	{
		kernel_module curr_module;
		curr_module.physical_offset = phy_offset;

		uint64_t name_addr, phy_name_addr, file_addr, phy_file_addr;
		uint16_t name_size;

		ifile.clear();

		ifile.seekg(phy_offset + prf.kernel_offsets[0]);
		ifile.read(reinterpret_cast<char *>(&(curr_module.flink)), sizeof(curr_module.flink));

		ifile.seekg(phy_offset + prf.kernel_offsets[1]);
		ifile.read(reinterpret_cast<char *>(&(curr_module.blink)), sizeof(curr_module.blink));

		ifile.seekg(phy_offset + prf.kernel_offsets[2]);
		ifile.read(reinterpret_cast<char *>(&file_addr), sizeof(file_addr)); //0x60 for ptr to file path

		ifile.seekg(phy_offset + prf.kernel_offsets[3]);
		ifile.read(reinterpret_cast<char *>(&name_size), sizeof(name_size)); //0x68 size of name

		ifile.seekg(phy_offset + prf.kernel_offsets[4]);
		ifile.read(reinterpret_cast<char *>(&name_addr), sizeof(name_addr)); //0x70 ptr64 to name

		curr_module.flink = utility_functions ::opt_get_phy_addr(ifile, curr_module.flink, prf.get_global_dtb(ifile)) - 0x10;
		curr_module.blink = utility_functions ::opt_get_phy_addr(ifile, curr_module.blink, prf.get_global_dtb(ifile)) - 0x10;

		cout << curr_module.flink << " " << curr_module.physical_offset << " " << curr_module.blink << " ";

		phy_name_addr = utility_functions ::opt_get_phy_addr(ifile, name_addr, prf.get_global_dtb(ifile));

		ifile.clear();
		ifile.seekg(phy_name_addr);
		char name[name_size * 2];
		ifile.read(name, sizeof(name));
		curr_module.name = utility_functions ::get_utf_str(name, sizeof(name));
		curr_module.name.erase(remove_if(curr_module.name.begin(), curr_module.name.end(), utility_functions ::invalidChar), curr_module.name.end()); ///Need to copy this
		cout << curr_module.name << " ";

		phy_file_addr = utility_functions ::opt_get_phy_addr(ifile, file_addr, prf.get_global_dtb(ifile));

		ifile.clear();
		ifile.seekg(phy_file_addr);
		char file_path[128];
		ifile.read(file_path, sizeof(file_path));
		curr_module.file_path = utility_functions ::get_utf_str(file_path, sizeof(file_path));
		replace(curr_module.file_path.begin(), curr_module.file_path.end(), '\\', '/');
		curr_module.file_path.erase(remove_if(curr_module.file_path.begin(), curr_module.file_path.end(), utility_functions ::invalidChar), curr_module.file_path.end());

		//cout << curr_module.file_path << " ";

		cout << "\n";
		return curr_module;
	}

	void generate_kernel_modules(ifstream &ifile, profile &prf)
	{
		ifile.clear();
		ifile.seekg(0, ios::beg);

		vector<uint64_t> phy_offsets = pool_tag_scan(ifile, prf);
		kernel_module curr;

		for (int i = 0; i < phy_offsets.size(); ++i)
		{
			curr = collect_info_module(ifile, prf, phy_offsets[i]);
			kernel_list.push_back(curr);
			kernel_map[phy_offsets[i]] = curr;
		}

		generate_kernel_ll(ifile, prf);
	}

	void generate_kernel_ll(ifstream &ifile, profile &prf)
	{
		doubly_ll<kernel_module> *curr_node = new doubly_ll<kernel_module>;
		curr_node->module = kernel_list[0];
		kernel_map.erase(curr_node->module.physical_offset);

		kernel_ll_head = curr_node;

		doubly_ll<kernel_module> *next_node;

		while(kernel_map.find(curr_node->module.flink) != kernel_map.end())
		{
			next_node = new doubly_ll<kernel_module>;
			next_node->module = kernel_map[curr_node->module.flink];

			curr_node->next = next_node;
			next_node->prev = curr_node;
			next_node->next = NULL;

			kernel_map.erase(next_node->module.physical_offset);

			curr_node = next_node;
		}

		curr_node = kernel_ll_head;
		doubly_ll<kernel_module> *prev_node;

		while(kernel_map.find(curr_node->module.blink) != kernel_map.end())
		{
			prev_node = new doubly_ll<kernel_module>;
			prev_node->module = kernel_map[curr_node->module.blink];

			prev_node->next = curr_node;
			curr_node->prev = prev_node;
			prev_node->prev = NULL;
			
			kernel_map.erase(prev_node->module.physical_offset);

			curr_node = prev_node;
			kernel_ll_head = curr_node;
		}
	}

	void print_ll()
	{
		doubly_ll<kernel_module> *p;
		p = kernel_ll_head;

		while(p != NULL)
		{
			kernel_module curr = p->module;
			cout<<curr.physical_offset<<" "<<curr.name<<" ";
			cout<<"\n";
			p = p->next;
		}
	}

	void print_unloaded_modules()
	{
		for(auto it : kernel_map)
		{
			kernel_module curr = it.second;
			cout<<curr.physical_offset<<" "<<curr.name<<" ";
			cout<<"\n";
		}
	}

	vector<kernel_module> get_kernel_list(ifstream &ifile, profile &prf)
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
	char fname[] = "../data/samples/win1064.vmem";

	ifile.open(fname, ios::in | ios::binary);
	if (!ifile)
	{
		cout << "Error in opening file..!!";
	}
	cout << "File opened..";
	cout << "\n";

	kh.get_kernel_list(ifile, prf);

	cout << kh.get_info() << endl;

	cout<<"\n*** Kernel Modules in Memory ***\n";
	kh.print_ll();
	cout<<"\n*** Unloaded Kernel Modules ***\n";
	kh.print_unloaded_modules();
}
#endif
#endif
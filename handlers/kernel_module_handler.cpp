#include <iostream>
#include <string>
#include <vector>
#include <fstream>

#include "../objects/kernel_module.cpp"
#include "../data/profiles.cpp"
#include "../utils/utility_functions.cpp"

using namespace std;

class kernel_module_handler
{
	private:
		vector<kernel_module> modules;

		
	public:
		vector<uint64_t> pool_tag_scan(ifstream &ifile, profile prf)
		{

			vector<uint64_t> phy_offsets;

			uint64_t addr_val = 0;
			char curr_pattern[8];

			uint64_t prev_size, pool_index, block_size, pool_type;

			while(ifile.eof() == 0)
			{
				ifile.read(curr_pattern, 8);
				addr_val += 8;
				if(utility_functions::scan_tag(prf.kernel_pool_tag, curr_pattern, 8))
				{
					prev_size = curr_pattern[0] * 16;
					pool_index = curr_pattern[1];
					block_size = curr_pattern[2] * 16;     //16 for 64bits and 8 for 32bits OS
					pool_type = curr_pattern[3];

					if(block_size <= 32)
					{
						ifile.ignore(8);
						addr_val += 8;
						continue;
					}

					phy_offsets.push_back(addr_val - 8);
					cout<<hex<<addr_val - 8<<"\n";

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

			ifile.read(reinterpret_cast<char *>(&file_addr), sizeof(file_addr));	//0x60 for ptr to file path
			cout<<hex<<file_addr<<" ";

			ifile.read(reinterpret_cast<char *>(&name_size), sizeof(name_size));	//0x68 size of name
			ifile.ignore(6);
			cout<<name_size<<" ";
			
			ifile.read(reinterpret_cast<char *>(&name_addr), sizeof(name_addr));	//0x70 ptr64 to name
			phy_name_addr = (phy_offset & 0xfffffffffffff000) | (name_addr & 0x0fff);
			cout<<hex<<phy_name_addr<<"\n";

			return curr_module;
		}

		void generate_kernel_modules(ifstream &ifile, profile prf)
		{
			ifile.clear();
			ifile.seekg(0, ios::beg);

			vector<uint64_t> phy_offsets = pool_tag_scan(ifile, prf);
		}
};

int main()
{
	kernel_module_handler kh;
    ifstream ifile;
	profile prf(7);
    char fname[] = "../data/samples/win764.vmem";
    
    ifile.open(fname, ios::in | ios::binary);
    if(!ifile)
	{
		cout<<"Error in opening file..!!";
	}	
	cout<<"File opened..";
	cout<<"\n";
    
    kh.generate_kernel_modules(ifile, prf);
    
}
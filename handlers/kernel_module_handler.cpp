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

			uint64_t addr_val;
			char curr_pattern[8];
			char pool_tag[8] = prf.hive_pool_tag;

			uint64_t prev_size, pool_index, block_size, pool_type;


			while(ifile.eof() == 0)
			{
				ifile.read(curr_pattern, 8);
				addr_val += 8;
				if(utility_functions::scan_tag(pool_tag, curr_pattern, 8))
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
				}
				else
				{
					ifile.ignore(8);
					addr_val += 8;
				}
			}
		}

		kernel_module collect_info_module(ifstream &ifile, profile prf, uint64_t phy_offset)
		{

		}

		void generate_kernel_modules(ifstream &ifile, profile prf)
		{
			ifile.clear();
			ifile.seekg(0, ios::beg);

			pool_tag_scan(ifile, prf);
		}
};
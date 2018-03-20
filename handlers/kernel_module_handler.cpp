#include <string>
#include <vector>
#include <fstream>
#include "../objects/kernel_module.cpp"
#include "../data/profiles.cpp"

using namespace std;

class kernel_module_handler
{
	private:
		vector<kernel_module> modules;

		
	public:
		vector<uint64_t> pool_tag_scan(ifstream file, profile prf)
		{
			
		}

		kernel_module collect_info_module(ifstream file, profile prf, uint64_t phy_offset)
		{

		}

		void generate_kernel_modules(ifstream file, profile prf)
		{
			
		}
};
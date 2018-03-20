#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h>
#include "../objects/registry_module.cpp"
#include "../utils/utility_functions.cpp"
#include "../data/profiles.cpp"

using namespace std;

class registry_handler 
{
    public:
        vector<registry_module> modules;

    private:
        vector<unit64_t> pool_scan_tag(ifstream &ifile, profile prf)
        {
            vector<uint64_t> phy_offsets;
            uint64_t addr_val = 0;
            char current_pattern[8];
            while(ifile.eof() == 0)
            {
                ifile.read(current_pattern, 8);
                addr_val += 8;
                if(utility_functions::scan_tag(current_pattern, prf.hive_pool_tag, 8))
                {
                    ifile.ignore(8);
                    addr_val += 8;
                    phy_offsets.push_back(addr_val);
                }
            }
            return phy_offsets;
        }

        regisrty_module collect_info_module(ifstream &ifile, profile prf, uint64_t phy_offset)
        {
            
        }

        void generate_registry_modules(ifstream &ifile, profile prf)
        {
            
        }
};


int main(void)
{
    registry_handler rh;
    ifstream ifile;
	profile prf(7);
    char fname[] = "../data/samples/win764.vmem";
    vector<uint64_t> phy_offset;
    ifile.open(fname, ios::in | ios::binary);
    if(!ifile)
	{
		cout<<"Error in opening file..!!";
	}	
	cout<<"File opened..";
	cout<<"\n";
    
    phy_offset = rh.pool_scan_tag(ifile, prf);
    
}

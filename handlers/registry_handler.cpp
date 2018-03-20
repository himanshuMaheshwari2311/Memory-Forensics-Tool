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
    private:
        vector<registry_module> modules;

    public:
        vector<uint64_t> pool_scan_tag(ifstream &ifile, profile prf)
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

        registry_module collect_info_module(ifstream &ifile, profile prf, uint64_t phy_offset)
        {
            uint64_t addr_val = phy_offset, vir_file_addr, phy_file_addr;
            ifile.seekg(phy_offset, ios::beg);
            ifile.ignore(1776);
            addr_val += 1776;
            ifile.read(reinterpret_cast<char *>(&vir_file_addr), 8);
            addr_val += 8;
            if(vir_file_addr == 0){
                ifile.ignore(8);
                addr_val += 8;
                ifile.read(reinterpret_cast<char *>(&vir_file_addr), 8);
                addr_val += 8;
            }
            if(vir_file_addr == 0)
                cout<<"[no name]"<<endl;
            else
            {
                phy_file_addr = utility_functions::get_phy_addr(ifile, vir_file_addr, 0x00187000);
                //get_utf_string(ifile, file_path, phy_file_addr);
                //cout<<file_path<<endl;
            }
            ifile.clear();
        }

        void generate_registry_modules(ifstream &ifile, profile prf)
        {
            vector<uint64_t> phy_offsets = pool_scan_tag(ifile, prf);
            for(int i = 0; i < phy_offsets.size(); i++)
            {
                modules.push_back(collect_info_module(ifile, prf, phy_offsets[i]));
            }
        }

        vector<registry_module> get_modules()
        {
            return modules;
        }   
};


int main(void)
{
    registry_handler rh;
    ifstream ifile;
	profile prf(7);
    char fname[] = "../data/samples/win764.vmem";
    vector<uint64_t> phy_offsets;
    ifile.open(fname, ios::in | ios::binary);
    if(!ifile)
	{
		cout<<"Error in opening file..!!";
	}	
	cout<<"File opened..";
	cout<<"\n";
    
    phy_offsets = rh.pool_scan_tag(ifile, prf);
    for(int i = 0; i < phy_offsets.size(); i++)
        cout<<hex<<phy_offsets[i]<<endl;
}

#ifndef _registry_handler_
#define _registry_handler_

#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h>
#include <iomanip>

#include "../objects/registry.cpp"
#include "../data/profiles.cpp"

using namespace std;

class registry_handler
{
  private:
    vector<registry> registry_list;
    
  public:
    vector<uint64_t> pool_scan_tag(ifstream &ifile, profile prf)
    {
        vector<uint64_t> phy_offsets;
        uint64_t addr_val = 0;
        char current_pattern[8];
        while (ifile.eof() == 0)
        {
            ifile.read(current_pattern, 8);
            addr_val += 8;
            if (utility_functions::compare_array(current_pattern, prf.hive_signature, 8)) //utility_functions::scan_tag(current_pattern, prf.hive_pool_tag, 8)
            {
                phy_offsets.push_back(addr_val - 8);
                ifile.ignore(8);
                addr_val += 8;
            }
        }
        return phy_offsets;
    }

    registry collect_info_module(ifstream &ifile, profile prf, uint64_t phy_offset)
    {
        registry rm;
        char *file_path = new char[172];
        uint64_t addr_val = phy_offset, vir_file_addr, phy_file_addr;
        uint8_t offset_ct = 0;

        rm.physical_offset = addr_val;
        ifile.seekg(phy_offset, ios::beg);
        ifile.ignore(1776); //prf.hive_offsets[offset_ct]);
        ifile.read(reinterpret_cast<char *>(&vir_file_addr), 8);
        if (vir_file_addr == 0)
        {
            ifile.ignore(8);
            ifile.read(reinterpret_cast<char *>(&vir_file_addr), 8);
        }
        if (vir_file_addr == 0)
            cout << hex << phy_offset << setw(64) << "[no name]" << endl;
        else
        {
            phy_file_addr = utility_functions::opt_get_phy_addr(ifile, vir_file_addr, 0x00187000); //profiles::get_global_dtb(ifile) // 0x187000 -> windows 7
            ifile.clear();
            ifile.seekg(0, ios::beg);
            ifile.seekg(phy_file_addr, ios::beg);
            ifile.read(file_path, 172);
            rm.file_path = utility_functions::get_utf_str(file_path);
            cout << hex << phy_offset << setw(70) << rm.file_path << endl;
        }

        ifile.clear();
        return rm;
    }

    void generate_registry_modules(ifstream &ifile, profile prf)
    {
        vector<uint64_t> phy_offsets;
        phy_offsets = pool_scan_tag(ifile, prf);
        ifile.clear();
        ifile.seekg(0, ios::beg);
        for (int i = 0; i < phy_offsets.size(); i++)
        {
            registry_list.push_back(collect_info_module(ifile, prf, phy_offsets[i]));
        }
    }

    vector<registry> get_registry_list(ifstream &ifile, profile prf)
    {
        if(registry_list.empty())
        {
            generate_registry_modules(ifile, prf);
        }
        return registry_list;
    }
};

#ifndef mainfunc
int main(void)
{
    registry_handler rh;
    ifstream ifile;
    //changes for windows 10
    profile prf(7);
    char fname[] = "../data/samples/win764.vmem";
    vector<uint64_t> phy_offsets;
    ifile.open(fname, ios::in | ios::binary);
    if (!ifile)
    {
        cout << "Error in opening file..!!";
    }
    cout << "File opened..";
    cout << "\n";
    /*
    phy_offsets = rh.pool_scan_tag(ifile, prf);
    for(int i = 0; i < phy_offsets.size(); i++)
        cout<<hex<<phy_offsets[i]<<endl;
    */
    ifile.clear();
    ifile.seekg(0, ios::beg);
    rh.generate_registry_modules(ifile, prf);

    vector<registry> 
}
#endif
#endif
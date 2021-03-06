#ifndef _registry_handler_
#define _registry_handler_

#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h>
#include <iomanip>
#include <algorithm>

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
            if (utility_functions::compare_array(current_pattern, prf.hive_signature, 8))
            {
                cout << hex << addr_val - 8 << endl;
                phy_offsets.push_back(addr_val - 8);
                ifile.ignore(8);
                addr_val += 8;
            }
        }
        return phy_offsets;
    }

    registry collect_info_module(ifstream &ifile, profile &prf, uint64_t phy_offset)
    {
        registry rm;
        rm.name = "HKEY_LOCAL_MACHINE";
        char *file_path = new char[172];
        ifile.clear();
        ifile.seekg(0, ios::beg);
        uint64_t addr_val = phy_offset, vir_file_addr, phy_file_addr;

        rm.physical_offset = addr_val;
        ifile.seekg(phy_offset, ios::beg);
        ifile.seekg(phy_offset + prf.hive_offsets[0]);
        ifile.read(reinterpret_cast<char *>(&vir_file_addr), 8);
        if (vir_file_addr == 0)
        {
            ifile.ignore(8);
            ifile.read(reinterpret_cast<char *>(&vir_file_addr), 8);
        }
        if (vir_file_addr == 0)
        {
            if (phy_offset != 0)
            {
                rm.file_path = "[no name]";
                cout << hex << phy_offset << setw(64) << "[no name]" << endl;
            }
        }
        else
        {
            ifile.clear();
            ifile.seekg(0, ios::beg);
            phy_file_addr = utility_functions::opt_get_phy_addr(ifile, vir_file_addr, prf.get_global_dtb(ifile));
            ifile.clear();
            ifile.seekg(0, ios::beg);
            ifile.seekg(phy_file_addr, ios::beg);
            ifile.read(file_path, 172);
            rm.file_path = utility_functions::get_utf_str(file_path, 86);
            rm.file_path.erase(remove_if(rm.file_path.begin(), rm.file_path.end(), utility_functions ::invalidChar), rm.file_path.end());
            replace(rm.file_path.begin(), rm.file_path.end(), '\\', '/');
            cout << hex << phy_offset << setw(70) << rm.file_path << endl;
        }
        return rm;
    }

    void generate_registry_modules(ifstream &ifile, profile &prf)
    {
        vector<uint64_t> phy_offsets;
        ifile.clear();
        ifile.seekg(0, ios::beg);
        phy_offsets = pool_scan_tag(ifile, prf);
        cout << prf.hive_offsets[0] << endl;
        for (int i = 0; i < phy_offsets.size(); i++)
        {
            ifile.clear();
            ifile.seekg(0, ios::beg);
            registry_list.push_back(collect_info_module(ifile, prf, phy_offsets[i]));
        }
        cout << "List size:" << registry_list.size() << endl;
    }

    vector<registry> get_registry_list(ifstream &ifile, profile &prf)
    {
        if (registry_list.empty())
        {
            generate_registry_modules(ifile, prf);
        }
        return registry_list;
    }

    string get_info()
    {
        string json;
        json += "{ ";
        json += "\"registry_list\" : ";
        json += "[ ";
        for (int i = 0; i < registry_list.size(); ++i)
        {
            json += registry_list[i].get_info();
            if (i != registry_list.size() - 1)
                json += ",";
            json += "\n";
        }

        json += "] ";

        json += "} ";

        return json;
    }

    void print_registry_hive()
    {
        registry r;
        cout << setw(20) << "Address (P)" << setw(90) << "Root Key Path" << endl;
        cout << "---------------------------------------------------------------------------------------------------------------------------" << endl;
        for (int i = 0; i < registry_list.size(); i++)
        {
            r = registry_list[i];
            cout << setw(20) << r.physical_offset <<  setw(90) << r.file_path << endl;
        }
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
        exit(0);
    }
    cout << "File opened..";
    cout << "\n";

    vector<registry> rg = rh.get_registry_list(ifile, prf);
    rh.print_registry_hive();
}
#endif
#endif
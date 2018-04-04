#ifndef _dll_object_handler_
#define _dll_object_handler_

#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>

#include "../objects/dll_object.cpp"
#include "../data/profiles.cpp"
#include "../utils/utility_functions.cpp"
#include "process_handler.cpp"

using namespace std;

class dll_object_handler
{
  private:
    vector<dll_object> dll_object_list;

  public:
    vector<dll_object> dll_ll_traverse(ifstream &ifile, profile prf)
    {
        utility_functions::reset_file(ifile);
        vector<dll_object> dlls;
        vector<uint64_t> peb_ldr_list;
        vector<uint64_t> dtb_list;
        vector<uint32_t> pid_list;
        uint64_t temp_addr = 0;
        uint64_t addr_val = 0;
        process_handler ph;

        vector<process> plist = ph.get_process_list(ifile, prf);
        utility_functions::reset_file(ifile);

        for (int i = 0; i < plist.size(); i++)
        {
            if (plist[i].peb != 0)
            {
                temp_addr = utility_functions ::opt_get_phy_addr(ifile, plist[i].peb, plist[i].dtb);
                ifile.seekg(temp_addr + prf.ldr_in_peb, ios::beg);
                ifile.read(reinterpret_cast<char *>(&temp_addr), sizeof(temp_addr));
                temp_addr = utility_functions ::opt_get_phy_addr(ifile, temp_addr, plist[i].dtb);
                peb_ldr_list.push_back(temp_addr);
                dtb_list.push_back(plist[i].dtb);
                pid_list.push_back(plist[i].pid);
            }
        }

        utility_functions::reset_file(ifile);

        for (int i = 0; i < peb_ldr_list.size(); i++)
        {
            uint64_t dll_addr;
            map<uint64_t, int> hash_map;
            dll_object temp_dll;

            temp_dll.pid = pid_list[i];
            temp_dll.dtb = dtb_list[i];

            ifile.seekg(peb_ldr_list[i] + prf.peb_ldr_offsets[1]);
            ifile.read(reinterpret_cast<char *>(&dll_addr), sizeof(dll_addr));

            if (dll_addr != 0 && ifile.eof() == 0)
            {

                dll_addr = utility_functions::opt_get_phy_addr(ifile, dll_addr, dtb_list[i]);
                cout << endl;
                cout << " - " << hex << peb_ldr_list[i] << " " << dtb_list[i] << " - ";
                cout << endl;

                while (hash_map.find(dll_addr) == hash_map.end())
                {
                    temp_dll.physical_offset = dll_addr;
                    hash_map[dll_addr] = 1;
                    dlls.push_back(temp_dll);
                    cout << hex << temp_dll.physical_offset << " " << dec << temp_dll.pid << " | ";
                    ifile.seekg(dll_addr);
                    ifile.read(reinterpret_cast<char *>(&dll_addr), sizeof(dll_addr));
                    if (dll_addr == 0)
                    {
                        break;
                    }
                    dll_addr = utility_functions::opt_get_phy_addr(ifile, dll_addr, dtb_list[i]);
                }
                cout << endl;
            }
            utility_functions::reset_file(ifile);
        }
        return dlls;
    }

    dll_object collect_info(ifstream &ifile, profile prf, uint64_t phy_offset, uint64_t dtb)
    {
        dll_object temp_dll;
        char full_name[512] = {0}, base_name[512] = {0};
        uint64_t str_addr;
        temp_dll.physical_offset = phy_offset;
        temp_dll.dtb = dtb;
        cout << setw(10) << hex << phy_offset;

        ifile.clear();
        ifile.seekg(phy_offset + prf.dll_object_offsets[2], ios::beg);
        ifile.read(reinterpret_cast<char *>(&temp_dll.dll_base), sizeof(temp_dll.dll_base));
        temp_dll.dll_base = utility_functions::opt_get_phy_addr(ifile, temp_dll.dll_base, dtb);
        cout << setw(10) << hex << temp_dll.dll_base;

        ifile.clear();
        ifile.seekg(phy_offset + prf.dll_object_offsets[3], ios::beg);
        ifile.read(reinterpret_cast<char *>(&temp_dll.size_of_image), sizeof(temp_dll.size_of_image));
        cout << setw(10) << dec << temp_dll.size_of_image;

        ifile.clear();
        ifile.seekg(phy_offset + prf.dll_object_offsets[4], ios::beg);
        ifile.read(reinterpret_cast<char *>(&str_addr), sizeof(str_addr));
        str_addr = utility_functions::opt_get_phy_addr(ifile, str_addr, dtb);
        ifile.seekg(str_addr);
        ifile.read(full_name, sizeof(full_name));
        temp_dll.full_dll_name = utility_functions ::get_utf_str(full_name, sizeof(full_name));
        cout << setw(55) << hex << temp_dll.full_dll_name;

        ifile.clear();
        ifile.seekg(phy_offset + prf.dll_object_offsets[5], ios::beg);
        ifile.read(reinterpret_cast<char *>(&str_addr), sizeof(str_addr));
        str_addr = utility_functions::opt_get_phy_addr(ifile, str_addr, dtb);
        ifile.seekg(str_addr);
        ifile.read(base_name, sizeof(base_name));
        temp_dll.name = utility_functions ::get_utf_str(base_name, sizeof(base_name));
        cout << setw(40) << hex << temp_dll.name;

        ifile.clear();
        ifile.seekg(phy_offset + prf.dll_object_offsets[6], ios::beg);
        ifile.read(reinterpret_cast<char *>(&temp_dll.load_count), sizeof(temp_dll.load_count));
        cout << setw(10) << dec << temp_dll.load_count;

        cout << endl;

        return temp_dll;
    }

    void generate_dll_objects(ifstream &ifile, profile prf)
    {
        dll_object temp_dll;
        utility_functions::reset_file(ifile);

        vector<dll_object> dlls = dll_ll_traverse(ifile, prf);

        for (int i = 0; i < dlls.size(); ++i)
        {
            temp_dll = collect_info(ifile, prf, dlls[i].physical_offset - prf.dll_object_offsets[1], dlls[i].dtb);
            if (temp_dll.name.length() > 4)
            {
                temp_dll.pid = dlls[i].pid;
                dll_object_list.push_back(temp_dll);
            }
            else
            {
                cout << "Erased" << endl;
            }
        }
    }

    vector<dll_object> get_dll_object_list(ifstream &ifile, profile prf)
    {
        if (dll_object_list.empty())
        {
            generate_dll_objects(ifile, prf);
        }
        return dll_object_list;
    }
    void print_dlls()
    {
        cout << setw(16) << "Address" << setw(10) << "PID" << setw(16) << "DLL Base" << setw(26) << "Name" << setw(50) << "Path" << setw(10) << "Load Count" << endl;
        dll_object d;
        for (int i = 0; i < dll_object_list.size(); i++)
        {
            d = dll_object_list[i];
            cout << setw(16) << hex << d.physical_offset << setw(16) << dec << d.pid << setw(16) << d.dll_base << setw(26) << d.name << setw(50) << d.full_dll_name << setw(10) << d.load_count << endl;
        }
    }
};

#ifndef mainfunc
int main(void)
{
    dll_object_handler dh;
    ifstream ifile;
    profile prf(7);
    char fname[] = "../data/samples/win764.vmem";

    ifile.open(fname, ios::in | ios::binary);
    if (!ifile)
    {
        cout << "Error in opening file..!!";
        return -1;
    }
    cout << "File opened..";
    cout << "\n";

    dh.generate_dll_objects(ifile, prf);
    dh.print_dlls();
}
#endif
#endif
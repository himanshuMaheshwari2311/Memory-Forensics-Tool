#ifndef _phandle_handler_
#define _phandle_handler_

#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>
#include <map>

#include "../objects/phandle.cpp"
#include "../data/profiles.cpp"
#include "../utils/utility_functions.cpp"
#include "process_handler.cpp"

using namespace std;

class phandle_handler
{
  private:
    vector<phandle> phandle_list;

  public:
    vector<phandle> enumerate_phandles(ifstream &ifile, profile prf)
    {
        utility_functions::reset_file(ifile);

        vector<phandle> temp_phandle_list;
        process_handler ph;
        vector<process> plist = ph.get_process_list(ifile, prf);
        vector<uint64_t> table_offsets;
        vector<uint64_t> table_pid;
        vector<uint64_t> phandle_offsets;
        vector<uint64_t> phandle_pid;
        vector<uint64_t> phandle_access;
        uint64_t access;
        uint64_t temp_addr;
        uint64_t table_base;
        uint32_t level;
        utility_functions::reset_file(ifile);
        cout << endl;
        for (int i = 0; i < plist.size(); i++)
        {
            if (plist[i].obj_table != 0 && plist[i].name != "Idle" && plist[i].name != "System")
            {
                temp_addr = plist[i].obj_table;
                // Read Table Code
                ifile.seekg(temp_addr, ios::beg);
                ifile.read(reinterpret_cast<char *>(&table_base), sizeof(table_base));
                cout << plist[i].name << " " << hex << table_base << " -> ";
                // seperate out level and base address
                level = table_base & 7;
                table_base = table_base & ~7;
                table_base = utility_functions ::opt_get_phy_addr(ifile, table_base, prf.get_global_dtb(ifile));

                // print level and base
                cout << hex << table_base << ", Level: " << dec << level << endl;

                if (level == 0)
                {
                    table_offsets.push_back(table_base);
                    table_pid.push_back(plist[i].pid);
                }
                else if (level == 1)
                {
                    int counter = 1;
                    ifile.seekg(table_base, ios::beg);
                    ifile.read(reinterpret_cast<char *>(&temp_addr), sizeof(temp_addr));
                    while (temp_addr)
                    {
                        cout << hex << "--" << temp_addr << "--" << endl;
                        temp_addr = utility_functions ::opt_get_phy_addr(ifile, temp_addr, prf.get_global_dtb(ifile));
                        if (temp_addr != 0)
                        {
                            table_offsets.push_back(temp_addr);
                            table_pid.push_back(plist[i].pid);
                        }
                        ifile.seekg(table_base + counter * 8, ios::beg);
                        ifile.read(reinterpret_cast<char *>(&temp_addr), sizeof(temp_addr));
                        counter++;
                    }
                }
            }
        }
        cout << endl;
        for (int i = 0; i < table_offsets.size(); i++)
        {
            cout << hex << table_offsets[i] << endl;
            ifile.seekg(table_offsets[i] + 16, ios::beg);
            ifile.read(reinterpret_cast<char *>(&temp_addr), sizeof(temp_addr));
            ifile.read(reinterpret_cast<char *>(&access), sizeof(access));
            int count = 0;
            int flag = 0;
            for (int j = 0; j < 255; j++)
            {
                if (temp_addr)
                {
                    count++;
                    flag++;
                    phandle_offsets.push_back(temp_addr);
                    phandle_pid.push_back(table_pid[i]);
                    phandle_access.push_back(access);
                }
                ifile.read(reinterpret_cast<char *>(&temp_addr), sizeof(temp_addr));
                ifile.read(reinterpret_cast<char *>(&access), sizeof(access));
            }
            cout << "Count: " << dec << count << " PID: " << table_pid[i] << endl;
        }

        for (int i = 0; i < phandle_offsets.size(); i++)
        {
            phandle pha;
            phandle_offsets[i] = utility_functions ::opt_get_phy_addr(ifile, phandle_offsets[i], prf.get_global_dtb(ifile));
            pha.physical_offset = phandle_offsets[i] & ~7;
            pha.pid = phandle_pid[i];
            pha.access = phandle_access[i];
            temp_phandle_list.push_back(pha);
        }
        utility_functions::reset_file(ifile);
        return temp_phandle_list;
    }

    phandle collect_info(ifstream &ifile, profile prf, uint64_t physical_offset, uint64_t dtb)
    {
        uint8_t type;
        utility_functions::reset_file(ifile);
        phandle pha;
        // Collect Name, Type
        ifile.seekg(physical_offset + prf.object_header_offsets[0], ios::beg);
        ifile.read(reinterpret_cast<char *>(&type), sizeof(type));
        if (type != 0x0e)
        {
            pha.type = "Not Mutant";
            return pha;
        }
        pha.type = "Mutant";
        return pha;
    }

    void generate_phandles(ifstream &ifile, profile prf)
    {
        phandle temp_pha;
        prf.get_global_dtb(ifile);
        utility_functions::reset_file(ifile);
        vector<phandle> phandles = enumerate_phandles(ifile, prf);

        for (int i = 0; i < phandles.size(); ++i)
        {
            temp_pha = collect_info(ifile, prf, phandles[i].physical_offset, prf.get_global_dtb(ifile));
            if (temp_pha.type == "Mutant")
            {
                temp_pha.pid = phandles[i].pid;
                temp_pha.access = phandles[i].access;
                phandle_list.push_back(temp_pha);
                cout << "PID: " << temp_pha.pid << "Access: " << temp_pha.access << endl;
            }
            else
            {
                //cout << "Not Mutex" << endl;
            }
        }
    }

    vector<phandle> get_phandle_list(ifstream &ifile, profile prf, uint32_t pid = 0)
    {
        if (phandle_list.empty())
        {
            generate_phandles(ifile, prf);
        }
        if (pid == 0)
            return phandle_list;
        else
        {
            vector<phandle> via_pid;
            for (int i = 0; i < phandle_list.size(); i++)
            {
                if (phandle_list[i].pid == pid)
                {
                    via_pid.push_back(phandle_list[i]);
                }
            }
            return via_pid;
        }
    }

    void print_phandles(uint32_t pid = 0)
    {
        phandle pha;
        cout << setw(16) << "Address" << setw(10) << "PID" << setw(16) << "DLL Base" << setw(26) << "Name" << setw(50) << "Path" << setw(16) << "Load Count" << endl;
        for (int i = 0; i < phandle_list.size(); i++)
        {
            pha = phandle_list[i];
            if (pid == 0 || pid == pha.pid)
            {
            }
        }
    }
};

#ifndef mainfunc
int main(void)
{
    phandle_handler hh;
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

    hh.generate_phandles(ifile, prf);
}
#endif
#endif
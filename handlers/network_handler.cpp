#ifndef _network_handler_
#define _network_handler_

#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h>
#include <iomanip>
#include <algorithm>

#include "../objects/network.cpp"
#include "../data/profiles.cpp"

using namespace std;

class network_handler
{
  private:
    vector<network> network_list;

  public:
    vector<uint64_t> pool_scan_tag(ifstream &ifile, profile prf)
    {
        vector<uint64_t> phy_offsets;
        uint64_t addr_val = 0;
        char current_pattern[8];
        //scan udp sockets
        while (ifile.eof() == 0)
        {
            ifile.read(current_pattern, 8);
            addr_val += 8;
            if (utility_functions::scan_tag(current_pattern, prf.udp_pool_tag, 8)) 
            {
                cout << hex << addr_val - 8 << endl;
                phy_offsets.push_back(addr_val + 8);
                ifile.ignore(8);
                addr_val += 8;
            }
        }
        return phy_offsets;
    }

    network collect_info_module(ifstream &ifile, profile &prf, uint64_t phy_offset)
    {
    }

    void generate_network_modules(ifstream &ifile, profile &prf)
    {
    }

    vector<network> get_network_list(ifstream &ifile, profile &prf)
    {
        if (network_list.empty())
        {
            generate_network_modules(ifile, prf);
        }
        return network_list;
    }

    string get_info()
    {
        string json;
        json += "{ ";
        json += "\"registry_list\" : ";
        json += "[ ";
        for (int i = 0; i < network_list.size(); ++i)
        {
            json += network_list[i].get_info();
            if (i != network_list.size() - 1)
                json += ",";
            json += "\n";
        }

        json += "] ";

        json += "} ";

        return json;
    }
};

#ifndef mainfunc
int main(void)
{
}
#endif
#endif
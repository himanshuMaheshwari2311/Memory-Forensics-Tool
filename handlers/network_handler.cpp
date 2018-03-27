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

                cout << hex << addr_val + 8 << endl;
                phy_offsets.push_back(addr_val + 8);
                ifile.ignore(8);
                addr_val += 8;
            }
        }
        return phy_offsets;
    }

    network collect_info_module(ifstream &ifile, profile &prf, uint64_t phy_offset)
    {
        network n;
        uint16_t type;
        uint64_t vir_addr, phy_addr;

        //udp
        uint8_t ipv4 = 0;
        uint16_t ipv6;
        ifile.seekg(phy_offset, ios::beg);
        ifile.ignore(0x20);
        ifile.read(reinterpret_cast<char *>(&vir_addr), 8);
        phy_addr = utility_functions::opt_get_phy_addr(ifile, vir_addr, prf.get_global_dtb(ifile));
        ifile.clear();
        ifile.seekg(phy_addr, ios::beg);
        ifile.ignore(0x14);
        ifile.read(reinterpret_cast<char *>(&type), 2);
        if (type == 2)
            n.protocol_version = "UDPv4";
        else if (type == 0x17)
            n.protocol_version = "UDPv6";
        
        //port
        ifile.clear();
        ifile.seekg(phy_offset, ios::beg);
        ifile.ignore(0x80);
        ifile.read(reinterpret_cast<char *>(&n.port), 2);
        uint16_t r_port = n.port << 8;
        uint16_t l_port = n.port >> 8;
        n.port = r_port | l_port;

        //address
        ifile.clear();
        ifile.seekg(0, ios::beg);
        ifile.seekg(phy_offset, ios::beg);
        ifile.ignore(0x60);
        ifile.read(reinterpret_cast<char *>(&vir_addr), 8);
        phy_addr = utility_functions::opt_get_phy_addr(ifile, vir_addr, prf.get_global_dtb(ifile));
        ifile.clear();
        ifile.seekg(0, ios::beg);
        ifile.seekg(phy_addr, ios::beg);
        ifile.ignore(16);
        ifile.read(reinterpret_cast<char *>(&vir_addr), 8);
        phy_addr = utility_functions::opt_get_phy_addr(ifile, vir_addr, prf.get_global_dtb(ifile));
        ifile.clear();
        ifile.seekg(0, ios::beg);
        ifile.seekg(phy_addr, ios::beg);
        ifile.read(reinterpret_cast<char *>(&vir_addr), 8);
        phy_addr = utility_functions::opt_get_phy_addr(ifile, vir_addr, prf.get_global_dtb(ifile));
        ifile.clear();
        ifile.seekg(0, ios::beg);
        ifile.seekg(phy_addr, ios::beg);
        if (type == 0x2)
        {
            for (int i = 0; i < 4; i++)
            {
                ifile.read(reinterpret_cast<char *>(&ipv4), 1);
                if (i != 3)
                    n.local_address += to_string(ipv4) + ".";
                else
                    n.local_address += to_string(ipv4);
            }
        }
        else if (type == 0x17)
        {
            uint16_t r, l;
            ifile.read(reinterpret_cast<char *>(&ipv6), 2);
            r = ipv6 << 8;
            l = ipv6 >> 8;
            ipv6 = r | l;

            n.local_address += to_string(ipv6) + ":";
            ifile.read(reinterpret_cast<char *>(&ipv6), 2);
            r = ipv6 << 8;
            l = ipv6 >> 8;
            ipv6 = r | l;

            n.local_address += to_string(ipv6) + ":";

            ifile.ignore(4);
            for (int i = 0; i < 4; i++)
            {
                ifile.read(reinterpret_cast<char *>(&ipv6), 2);
                r = ipv6 << 8;
                l = ipv6 >> 8;
                ipv6 = r | l;
                if (i != 3)
                {
                    n.local_address += to_string(ipv6) + ":";
                }
                else
                    n.local_address += to_string(ipv6);
            }
        }
        else
        {
            n.local_address = "temp";
        }
        if (type != 0)
        {
            cout << hex << phy_offset << " " << n.protocol_version;
            cout << " " << dec << n.local_address << " :  " << n.port << endl;
        }
        return n;
    }

    void generate_network_modules(ifstream &ifile, profile &prf)
    {
        vector<uint64_t> phy_offsets = pool_scan_tag(ifile, prf);
        for (int i = 0; i < phy_offsets.size(); i++)
        {
            ifile.clear();
            ifile.seekg(0, ios::beg);
            network_list.push_back(collect_info_module(ifile, prf, phy_offsets[i]));
        }
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
        //left_to_do
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
    network_handler nh;
    ifstream ifile;
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

    nh.generate_network_modules(ifile, prf);
}
#endif
#endif

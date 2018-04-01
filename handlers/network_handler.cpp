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
        ifile.clear();
        ifile.seekg(0, ios::beg);
        vector<uint64_t> phy_offsets;
        uint64_t addr_val = 0;
        char current_pattern[8];
        //scan udp and tcp sockets
        while (ifile.eof() == 0)
        {
            ifile.read(current_pattern, 8);
            addr_val += 8;
            // udp
            if (utility_functions::scan_tag(current_pattern, prf.udp_pool_tag, 8))
            {
                cout<< hex << "UDP " << addr_val + 8 << endl;
                phy_offsets.push_back(addr_val + 8);
                ifile.ignore(8);
                addr_val += 8;
            }
            // tcp
            else if (utility_functions::scan_tag(current_pattern, prf.tcp_pool_tag, 8))
            {
                cout << hex << "TCP " << addr_val + 8 << endl;
                phy_offsets.push_back(addr_val + 8);
                ifile.ignore(8);
                addr_val += 8;
            }
        }
        return phy_offsets;
    }

    void get_local_address(ifstream &ifile, profile &prf, uint64_t phy_offset, uint64_t proto_offset, network &n, uint16_t type)
    {
        uint64_t phy_addr, vir_addr;
        uint8_t ipv4 = 0;
        uint16_t ipv6 = 0;
        ifile.clear();
        ifile.seekg(0, ios::beg);
        ifile.seekg(phy_offset + proto_offset);
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
    }

    network collect_info_module(ifstream &ifile, profile &prf, uint64_t phy_offset)
    {
        network n;
        char current_pattern[8];
        uint16_t type;
        uint64_t vir_addr, phy_addr;
        n.physical_offset = phy_offset;

        //udp
        ifile.clear();
        ifile.seekg(0, ios::beg);
        ifile.seekg(phy_offset - 16, ios::beg);
        ifile.read(current_pattern, 8);
        if (utility_functions::scan_tag(current_pattern, prf.udp_pool_tag, 8))
        {
            //inet_af
            ifile.seekg(phy_offset + prf.udp_offsets[0]);
            ifile.read(reinterpret_cast<char *>(&vir_addr), 8);
            phy_addr = utility_functions::opt_get_phy_addr(ifile, vir_addr, prf.get_global_dtb(ifile));
            ifile.clear();
            ifile.seekg(phy_addr, ios::beg);
            ifile.ignore(prf.udp_offsets[5]);
            if(prf.type == 7)
            {
                ifile.read(reinterpret_cast<char *>(&type), 2);
            }
            else
            {
                ifile.read(reinterpret_cast<char *>(&type), 1);
            }
            if (type == 2)
                n.protocol_version = "UDPv4";
            else if (type == 0x17)
                n.protocol_version = "UDPv6";

            //owner information
            char p_name[16];
            ifile.clear();
            ifile.seekg(phy_offset + prf.udp_offsets[3], ios::beg);
            ifile.read(reinterpret_cast<char *>(&vir_addr), 8);
            phy_addr = utility_functions::opt_get_phy_addr(ifile, vir_addr, prf.get_global_dtb(ifile));
            ifile.seekg(phy_addr + 8 + prf.process_offsets[0], ios::beg);
            ifile.read(reinterpret_cast<char *>(&n.pid), sizeof(n.pid));
            ifile.seekg(phy_addr + prf.udp_offsets[4], ios::beg);
            ifile.read(p_name, 16);
            n.owner_name = p_name;

            //port
            ifile.clear();
            ifile.seekg(phy_offset + prf.udp_offsets[1]);
            ifile.read(reinterpret_cast<char *>(&n.port), 2);
            uint16_t r_port = n.port << 8;
            uint16_t l_port = n.port >> 8;
            n.port = r_port | l_port;

            //address
            if(prf.type == 7)
            {
                get_local_address(ifile, prf, phy_offset, prf.udp_offsets[2], n, type);
            }
            else  {
                if(type == 0x2)
                    n.local_address = "0.0.0.0";
                else n.local_address = "0:0:0:0:0:0";
            }
        }
        else
        {
            // inet_af
            ifile.seekg(phy_offset + prf.tcp_offsets[0]);
            ifile.read(reinterpret_cast<char *>(&vir_addr), 8);
            phy_addr = utility_functions::opt_get_phy_addr(ifile, vir_addr, prf.get_global_dtb(ifile));
            ifile.clear();
            ifile.seekg(phy_addr, ios::beg);
            ifile.ignore(prf.udp_offsets[5]);
            if(prf.type == 7)
            {
                ifile.read(reinterpret_cast<char *>(&type), 2);
            }
            else
            {
                ifile.read(reinterpret_cast<char *>(&type), 1);
            }
            
            if (type == 2)
                n.protocol_version = "TCPv4";
            else if (type == 0x17)
                n.protocol_version = "TCPv6";

            // port
            ifile.clear();
            ifile.seekg(phy_offset + prf.tcp_offsets[1]);
            ifile.read(reinterpret_cast<char *>(&n.port), 2);
            uint16_t r_port = n.port << 8;
            uint16_t l_port = n.port >> 8;
            n.port = r_port | l_port;

            //owner info
            char p_name[16];
            ifile.clear();
            ifile.seekg(phy_offset + prf.tcp_offsets[3], ios::beg);
            ifile.read(reinterpret_cast<char *>(&vir_addr), 8);
            phy_addr = utility_functions::opt_get_phy_addr(ifile, vir_addr, prf.get_global_dtb(ifile));
            ifile.seekg(phy_addr + 8 + prf.process_offsets[0], ios::beg);
            ifile.read(reinterpret_cast<char *>(&n.pid), sizeof(n.pid));
            ifile.seekg(phy_addr + prf.tcp_offsets[4], ios::beg);
            ifile.read(p_name, 16);
            n.owner_name = p_name;

            //address
            if(prf.type == 7)
            {
                get_local_address(ifile, prf, phy_offset, prf.tcp_offsets[2], n, type);
            }
            else  {
                if(type == 0x2)
                    n.local_address = "0.0.0.0";
                else n.local_address = "0:0:0:0:0:0";
            }
        }
        if (type != 0)
        {
            cout << hex << phy_offset << " " << n.protocol_version;
            cout << " " << dec << n.local_address << " :  " << n.port << " " << n.owner_name << " " << n.pid << endl;
        }
        return n;
    }

    void generate_network_modules(ifstream &ifile, profile &prf)
    {
        string temp = "temp";
        vector<uint64_t> phy_offsets = pool_scan_tag(ifile, prf);
        for (int i = 0; i < phy_offsets.size(); i++)
        {
            ifile.clear();
            ifile.seekg(0, ios::beg);
            network_list.push_back(collect_info_module(ifile, prf, phy_offsets[i]));
            if (network_list[network_list.size() - 1].local_address.compare(temp) == 0)
                network_list.pop_back();
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
        cout << "Generating" << endl;
        string json;
        json += "{ ";
        json += "\"network_list\" : ";
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
    cout << nh.get_info() << endl;
}
#endif
#endif

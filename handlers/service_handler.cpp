#ifndef _service_handler_
#define _service_handler_

#include <vector>
#include <iomanip>
#include <iostream>
#include <fstream>

#include "../objects/service.cpp"
#include "../data/profiles.cpp"
#include "../utils/utility_functions.cpp"

using namespace std;

class service_handler
{
  private:
	vector<service> service_list;

  public:
	vector<uint64_t> service_tag_scan(ifstream &ifile, profile prf)
	{
		ifile.clear();
		ifile.seekg(0, ios::beg);

		vector<uint64_t> phy_offsets;
		uint64_t temp_addr = 0;
		uint64_t addr_val = 0;
		char found_pattern_s[8] = {0};

		while (ifile.eof() == 0)
		{
			ifile.read(found_pattern_s, 8);
			addr_val += 8;
			if (utility_functions ::compare_array(prf.service_pattern1, found_pattern_s, 8) || utility_functions ::compare_array(prf.service_pattern2, found_pattern_s, 8))
			{
				ifile.ignore(8);
				addr_val += 8;

				ifile.read(reinterpret_cast<char *>(&temp_addr), sizeof(temp_addr));
				addr_val += 8;

				if (temp_addr > 0)
				{
					phy_offsets.push_back(temp_addr);
					cout << hex << temp_addr << endl;
				}

				ifile.ignore(8);
				addr_val += 8;
			}
			else
			{
				ifile.ignore(8);
				addr_val += 8;
			}
		}
		return phy_offsets;
	}

	service collect_info(ifstream &ifile, profile prf, uint64_t phy_offset, uint64_t dtb)
	{
		service srv;
		uint64_t srv_record_addr[100], temp_addr, str_addr;
		char s_name[256] = {0}, d_name[256] = {0};
		uint32_t s_type;
		uint32_t s_state;
		uint32_t s_start;

		temp_addr = utility_functions ::opt_get_phy_addr(ifile, phy_offset, dtb);
		srv.physical_offset = temp_addr;
		cout << setw(10) << hex << temp_addr;

		ifile.clear();
		ifile.seekg(temp_addr + prf.service_record_offsets[0], ios::beg);
		ifile.read(reinterpret_cast<char *>(&str_addr), sizeof(str_addr));
		str_addr = utility_functions ::opt_get_phy_addr(ifile, str_addr, dtb);
		ifile.seekg(str_addr);
		ifile.read(s_name, sizeof(s_name));
		srv.name = utility_functions ::get_utf_str(s_name, sizeof(s_name));
		cout << setw(25) << hex << srv.name;
		srv.name.erase(remove_if(srv.name.begin(), srv.name.end(), utility_functions ::invalidChar), srv.name.end());

		ifile.clear();
		ifile.seekg(temp_addr + prf.service_record_offsets[1], ios::beg);
		ifile.read(reinterpret_cast<char *>(&str_addr), sizeof(str_addr));
		str_addr = utility_functions ::opt_get_phy_addr(ifile, str_addr, dtb);
		ifile.seekg(str_addr);
		ifile.read(d_name, sizeof(d_name));
		srv.display_name = utility_functions ::get_utf_str(d_name, sizeof(d_name));
		cout << setw(55) << hex << srv.display_name;
		srv.display_name.erase(remove_if(srv.display_name.begin(), srv.display_name.end(), utility_functions ::invalidChar), srv.display_name.end());

		ifile.clear();
		ifile.seekg(temp_addr + prf.service_record_offsets[2], ios::beg);
		ifile.read(reinterpret_cast<char *>(&s_type), sizeof(s_type));
		srv.type = srv.get_type(s_type);
		cout << setw(30) << hex << srv.type;

		ifile.clear();
		ifile.seekg(temp_addr + prf.service_record_offsets[3], ios::beg);
		ifile.read(reinterpret_cast<char *>(&s_state), sizeof(s_state));
		srv.state = srv.get_state(s_state);
		cout << setw(20) << hex << srv.state;

		ifile.clear();
		ifile.seekg(temp_addr + prf.service_record_offsets[4], ios::beg);
		ifile.read(reinterpret_cast<char *>(&s_start), sizeof(s_start));
		srv.start = srv.get_start(s_start);
		cout << setw(25) << hex << srv.start;

		cout << endl;

		return srv;
	}

	void generate_services(ifstream &ifile, profile prf)
	{
		ifile.clear();
		ifile.seekg(0, ios::beg);

		uint64_t service_dtb = prf.get_service_dtb(ifile);
		//uint64_t service_dtb = 0xbaae000;
		//uint64_t service_dtb = 0x38096000;

		vector<uint64_t> phy_offsets = service_tag_scan(ifile, prf);

		for (int i = 0; i < phy_offsets.size(); ++i)
		{
			service_list.push_back(collect_info(ifile, prf, phy_offsets[i], service_dtb));
		}
	}

	vector<service> get_service_list(ifstream &ifile, profile prf)
	{
		if (service_list.empty())
		{
			generate_services(ifile, prf);
		}
		return service_list;
	}

	void print_services()
	{
		cout << endl
			 << setw(16) << "Address" << setw(40) << "Service Name" << setw(50) << "Display Name" << endl;
		service srv;
		for (int i = 0; i < service_list.size(); i++)
		{
			srv = service_list[i];
			cout << setw(16) << hex << srv.physical_offset << setw(40) << srv.name << setw(50) << srv.display_name << endl;
		}
	}
	string get_info()
	{
		string json = "";
		json += "{ ";
		json += "\"service_list\" : ";
		json += "[ ";
		for (int i = 0; i < service_list.size(); ++i)
		{
			json += service_list[i].get_info();
			if (i != service_list.size() - 1)
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
	service_handler sh;
	ifstream ifile;
	profile prf(10);
	char fname[] = "../data/samples/win1064.vmem";

	ifile.open(fname, ios::in | ios::binary);
	if (!ifile)
	{
		cout << "Error in opening file..!!";
		return -1;
	}
	cout << "File opened..";
	cout << "\n";

	sh.generate_services(ifile, prf);
	cout << sh.get_info();
}
#endif
#endif
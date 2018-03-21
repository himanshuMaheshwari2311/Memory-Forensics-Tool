#ifndef _profiles_
#define _profiles_

#include <stdint.h>
#include <cstring>
#include <string>
#include <string.h>

#include "../utils/utility_functions.cpp"

class profile
{
  public:
	int type;

	uint64_t global_dtb = 0;
	uint64_t service_dtb = 0;

	char *dtb_eproc_name;
	int process_name_offset;

	char *process_signature;
	int *process_offsets;

	char *service_pattern1;
	char *service_pattern2;

	char *hive_signature;
	char *hive_pool_tag;
	int *hive_offsets;

	char *kernel_pool_tag;
	int *kernel_offsets;

	profile(int type)
	{
		this->type = type;
		if (type == 7)
			init_as_win7();
		if (type == 10)
			init_as_win10();
	}

	void init_as_win7()
	{
		process_signature = new char[8]{3, 0, 88, 0, 0, 0, 0, 0};
		process_offsets = new int[4]{376, 268, 76, 480};
		process_name_offset = 736;

		dtb_eproc_name = new char[5]{'I', 'd', 'l', 'e'};

		service_pattern1 = new char[8]{115, 101, 114, 72, 0, 0, 0, 0};
		service_pattern2 = new char[8]{115, 101, 114, 72, 4, 0, 0, 0};

		hive_signature = new char[8]{char(224), char(190), char(224), char(190), 0, 0, 0, 0};
		hive_pool_tag = new char[8]{'0', '0', '0', '0', 'C', 'M', '1', '0'};
		hive_offsets = new int[1]{1776};

		kernel_pool_tag = new char[8]{'0', '0', '0', '0', 'M', 'm', 'L', 'd'};
		kernel_offsets = new int[2]{96, 6};
	}

	void init_as_win10()
	{
		process_signature = new char[8]{3, 0, char(182), 0, 0, 0, 0, 0};
		process_offsets = new int[4]{736, 256, 112, 968};
		process_name_offset = 1104;

		dtb_eproc_name = new char[7]{'S', 'y', 's', 't', 'e', 'm'};

		service_pattern1 = new char[8]{115, 101, 114, 72, 0, 0, 0, 0};
		service_pattern2 = new char[8]{115, 101, 114, 72, 4, 0, 0, 0};

		hive_signature = new char[8]{char(224), char(190), char(224), char(190), 0, 0, 0, 0};
		hive_pool_tag = new char[8]{'0', '0', '0', '0', 'C', 'M', '1', '0'};
		hive_offsets = new int[1]{1776};
	}

	uint64_t get_global_dtb(ifstream &ifile)
	{
		// Mostly will not work for 10.. if else needs to be added for offset..
		// Or use an offset array which will be initialised during object creation
		if (global_dtb == 0)
		{
			char found_pattern_p[8];
			char p_name[16];
			while (ifile.eof() == 0)
			{
				ifile.read(found_pattern_p, 8);
				if (utility_functions::compare_array(process_signature, found_pattern_p, 8))
				{
					ifile.ignore(process_name_offset - 8);
					ifile.read(p_name, 16);
					if (strcmp(p_name, dtb_eproc_name) == 0)
					{
						cout << dtb_eproc_name << " found!" << endl;
						ifile.seekg(-(process_name_offset + 16), std::ios::cur);
						ifile.seekg(40, std::ios::cur);
						unsigned long temp;
						ifile.read(reinterpret_cast<char *>(&temp), sizeof(temp));
						cout << "DTB value for  " << dtb_eproc_name << " " << hex << temp << endl;
						global_dtb = temp;
						ifile.clear();
						ifile.seekg(0, std::ios::beg);
						return global_dtb;
					}
					ifile.ignore(480);
				}
				else
				{
					ifile.ignore(8);
				}
			}
		}
		else
		{
			return global_dtb;
		}
	}
	uint64_t get_service_dtb(ifstream &ifile)
	{
		if (service_dtb == 0)
		{
			uint64_t addr_val = 0;
			char found_pattern_p[8];
			char p_name[16];
			while (ifile.eof() == 0)
			{
				ifile.read(found_pattern_p, 8);
				addr_val += 8;
				if (utility_functions::compare_array(process_signature, found_pattern_p, 8))
				{
					ifile.ignore(736 - 8);
					ifile.read(p_name, 16);
					if (strcmp(p_name, "services.exe") == 0)
					{
						cout << "services.exe found at " << hex << addr_val - 8 << endl;
						ifile.seekg(-(736 + 16), std::ios::cur);
						ifile.seekg(40, std::ios::cur);
						unsigned long temp;
						ifile.read(reinterpret_cast<char *>(&temp), sizeof(temp));
						cout << "DTB value for  services.exe " << hex << temp << endl;
						global_dtb = temp;
						ifile.clear();
						ifile.seekg(0, std::ios::beg);
						return global_dtb;
					}
					ifile.ignore(480);
					addr_val += 1232 - 8;
				}
				else
				{
					ifile.ignore(8);
					addr_val += 8;
				}
			}
		}
		else
		{
			return service_dtb;
		}
	}
};

#endif
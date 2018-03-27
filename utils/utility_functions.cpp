#ifndef _utility_functions_
#define _utility_functions_

#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h>

using namespace std;

class utility_functions
{
  public:
	static int scan_tag(char arr1[], char arr2[], int n)
	{
		int i;
		for (i = 4; i < n; i++)
			if (arr1[i] != arr2[i])
				return 0;
		return 1;
	}

	static int compare_array(char arr1[], char arr2[], int n)
	{
		int i;
		for (i = 0; i < n; i++)
			if (arr1[i] != arr2[i])
				return 0;
		return 1;
	}

	static uint64_t get_phy_addr(ifstream &ifile, uint64_t vir_addr, uint32_t dtb)
	{
		uint8_t level;
		if ((vir_addr & 0b1000000000000000000000000000) == 0) //untested
			level = 4;
		else
			level = 3;

		ifile.clear();
		ifile.seekg(0, ios::beg);

		uint64_t phy_addr;
		uint32_t temp1;
		uint32_t temp2;
		uint16_t p[4];
		uint32_t offset;

		p[0] = vir_addr >> 39 & 0b111111111;
		p[1] = vir_addr >> 30 & 0b111111111;
		p[2] = vir_addr >> 21 & 0b111111111;
		p[3] = vir_addr >> 12 & 0b111111111;
		offset = vir_addr & 0b111111111111;
		if (level == 3)
		{
			offset = vir_addr & 0b0111111111111111111111;
		}

		temp1 = (dtb & 0xFFFFF000) + p[0] * 8;
		ifile.ignore(temp1);
		ifile.read(reinterpret_cast<char *>(&temp2), sizeof(temp2));
		ifile.clear();
		ifile.seekg(0, ios::beg);

		temp1 = (temp2 & 0xFFFFF000) + p[1] * 8;
		ifile.ignore(temp1);
		ifile.read(reinterpret_cast<char *>(&temp2), sizeof(temp2));
		ifile.clear();
		ifile.seekg(0, ios::beg);

		temp1 = (temp2 & 0xFFFFF000) + p[2] * 8;
		ifile.ignore(temp1);
		ifile.read(reinterpret_cast<char *>(&temp2), sizeof(temp2));
		ifile.clear();
		ifile.seekg(0, ios::beg);

		if (level == 3)
		{
			return ((temp2 & 0b11111111111000000000000000000000) + offset);
		}

		temp1 = (temp2 & 0xFFFFF000) + p[3] * 8;
		ifile.ignore(temp1);
		ifile.read(reinterpret_cast<char *>(&temp2), sizeof(temp2));
		ifile.clear();
		ifile.seekg(0, ios::beg);

		return ((temp2 & 0xFFFFF000) + offset);
	}

	static uint64_t opt_get_phy_addr(ifstream &ifile, uint64_t vir_addr, uint32_t dtb)
	{
		ifile.clear();
		ifile.seekg(0, ios::beg);

		uint16_t level;
		uint64_t phy_addr;
		uint32_t nextLvl;
		uint32_t lvl1;
		uint32_t lvl2;
		uint32_t lvl3;
		uint32_t lvl4;
		uint16_t p[4];
		uint32_t offset;

		p[0] = vir_addr >> 39 & 0b111111111;
		p[1] = vir_addr >> 30 & 0b111111111;
		p[2] = vir_addr >> 21 & 0b111111111;
		p[3] = vir_addr >> 12 & 0b111111111;
		offset = vir_addr & 0b111111111111;

		lvl1 = (dtb & 0xFFFFF000) + p[0] * 8;
		ifile.seekg(lvl1);
		ifile.read(reinterpret_cast<char *>(&nextLvl), sizeof(nextLvl));

		lvl2 = (nextLvl & 0xFFFFF000) + p[1] * 8;
		ifile.seekg(lvl2);
		ifile.read(reinterpret_cast<char *>(&nextLvl), sizeof(nextLvl));

		lvl3 = (nextLvl & 0xFFFFF000) + p[2] * 8;
		ifile.seekg(lvl3);
		ifile.read(reinterpret_cast<char *>(&nextLvl), sizeof(nextLvl));

		if ((nextLvl & 0b10000000) == 0) //untested
			level = 4;
		else
			level = 3;

		if (level == 3)
		{
			offset = vir_addr & 0b0111111111111111111111;
			return ((nextLvl & 0b11111111111000000000000000000000) + offset);
		}

		lvl4 = (nextLvl & 0xFFFFF000) + p[3] * 8;
		ifile.seekg(lvl4);
		ifile.read(reinterpret_cast<char *>(&nextLvl), sizeof(nextLvl));

		return ((nextLvl & 0xFFFFF000) + offset);
	}

	static char *get_utf_str(char uni_str[], int n = 64)
	{
		char *str = new char[n];
		int i = 0, j = 0;
		while (uni_str[j] != 0)
		{
			str[i] = uni_str[j];
			i += 1;
			j += 2;
		}
		str[i] = '\0';
		return str;
	}

	static bool invalidChar(char c)
	{
		return !(c >= 32 && c < 127 || c == 0 || c == '"');
	}
};

#endif
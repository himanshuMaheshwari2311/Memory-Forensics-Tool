#include <iostream>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <fstream>
#include <iomanip>
#include <stdint.h>

using namespace std;

uint64_t opt_get_phy_addr(ifstream &ifile, uint64_t vir_addr, uint32_t dtb)
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

uint64_t translateAddr(ifstream &ifile, uint64_t phy_offset, uint32_t dtb)
{
	uint64_t phy_addr;
	uint64_t vir_addr;

	ifile.seekg(phy_offset);
	ifile.read(reinterpret_cast<char *>(&vir_addr), sizeof(vir_addr));

	phy_addr = opt_get_phy_addr(ifile, vir_addr, dtb);
	return phy_addr;
}

#ifndef mainfunc
int main()
{
    char fname[] = "../data/samples/win764.vmem";
	//char fname[] = "../data/samples/win1064.vmem";
    //uint64_t dtb = 0x001ab000;
	uint64_t dtb = 0x00187000;
	//uint64_t dtb = 0x4a147000;
    
    ifstream ifile;

    ifile.open(fname, ios::in | ios::binary);
	if(!ifile)
	{
		cout<<"Error in opening file..!!";
		exit(0);
	}

	int choice = 999;
	do
	{
		if(choice == 1)
		{
			uint64_t addr;
			cin>>hex>>addr;
			cout<<"\nPhysical Address: "<<hex<<opt_get_phy_addr(ifile, addr, dtb)<<endl;
		}
		else if(choice == 2)
		{
			uint64_t addr;
			cin>>hex>>addr;
			cout<<"\nPhysical Address: "<<hex<<translateAddr(ifile, addr, dtb)<<endl;
		}

		cout<<"1. virtual\n";
		cout<<"2. phy offset\n";
		cout<<"0. exit (ctrl + c)\n";
		cin>>choice;
	} while(choice != 0);

	
	return 0;
}
#endif
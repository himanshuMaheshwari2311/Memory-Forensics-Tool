#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h>

using namespace std;

class utility_function
{
    public:
        static int scan_tag(char arr1[], char arr2[], int n)
        {
            int i;
            for(i = 4; i < n; i++)
                if(arr1[i] != arr2[i])
                    return 0;
            return 1;
        }

        static int compare_array(char arr1[], char arr2[], int n)
        {
            int i;
            for(i = 0; i < n; i++)
                if(arr1[i] != arr2[i])
                    return 0;
            return 1;
        }

        static uint64_t get_phy_addr(ifstream &ifile, uint64_t vir_addr, uint32_t dtb, uint8_t level)
        {
            ifile.clear();
            ifile.seekg(0, ios::beg);
            
            uint64_t phy_addr;
            uint32_t temp1;
            uint32_t temp2;
            uint16_t p[4];
            uint32_t offset;

            p[0] = vir_addr>>39 & 0b111111111;
            p[1] = vir_addr>>30 & 0b111111111;
            p[2] = vir_addr>>21 & 0b111111111;
            p[3] = vir_addr>>12 & 0b111111111;
            offset = vir_addr & 0b111111111111;
            if(level == 3)
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

            if(level == 3)
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

        
        static char* get_utf_str(char uniStr[], int n)
        {
            char *str = new char[n/2 + 1];
            for(int i = 0; i < n; i+=2)
            {
                str[i / 2] = uniStr[i];
            }
            str[n/2] = '0';
            return str;
        }
};

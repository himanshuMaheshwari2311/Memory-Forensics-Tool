#include <iostream>
#include <string>
#include <fstream>
#include <stdint.h>

using namespace std;

class utility_function
{
    static void get_utf_string(ifstream &ifile, char str[], uint64_t addr)
    {
        uint8_t c = 1;
        int size = 0;
        ifile.clear();
        ifile.seekg(addr, ios::beg);
        ifile.read(reinterpret_cast<char *>(&c), sizeof(c));
        while(c)
        {
            str[size] = c;
            size++;
            ifile.ignore(1);
            ifile.read(reinterpret_cast<char *>(&c), sizeof(c));
            if(c > 127)
                break;
        }
        str[size] = 0;
    }

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
};

#include <stdint.h>
#include "../utils/utility_functions.cpp"
#include <String.h>

class profile 
{
    public:
        int type;

        uint64_t global_dtb = 0;
        uint64_t service_dtb = 0;

        char *process_signature;
        int *process_offsets;

        char *service_pattern1;
        char *service_pattern2;

        char *hive_signature;
        char *hive_pool_tag;
        int *hive_offsets;

        char *kernel_pool_tag;
        int *kernel_offsets;

        profile (int type)
        {
            this->type = type;
            if(type == 7)
                init_as_win7();
            if(type == 10)
                init_as_win10();
        }

        void init_as_win7()
        {
            process_signature = new char[8] {3, 0, 88, 0, 0, 0, 0, 0};
            process_offsets = new int[4] {376, 268, 76, 480};

            service_pattern1 = new char[8] {115, 101, 114, 72, 0, 0, 0, 0};
            service_pattern2 = new char[8] {115, 101, 114, 72, 4, 0, 0, 0};

            hive_signature = new char[4] {char(224), char(190), char(224), char(190)};
            hive_pool_tag = new char[8] {'0', '0', '0', '0', 'C', 'M', '1', '0'};
            hive_offsets = new int[1] {1776};

            kernel_pool_tag = new char[8] {'0', '0', '0', '0', 'M', 'm', 'L', 'd'};
            kernel_offsets = new int[2] {96, 6};
        }

        void init_as_win10()
        {
            process_signature = new char[8] {3, 0, char(182), 0, 0, 0, 0, 0};
            process_offsets = new int[4] {376, 268, 76, 480};

            service_pattern1 = new char[8] {115, 101, 114, 72, 0, 0, 0, 0};
            service_pattern2 = new char[8] {115, 101, 114, 72, 4, 0, 0, 0};

            hive_signature = new char[4] {char(224), char(190), char(224), char(190)};
            hive_pool_tag = new char[8] {'0', '0', '0', '0', 'C', 'M', '1', '0'};
            hive_offsets = new int[1] {1776};
        }

        uint64_t get_global_dtb(ifstream &ifile)
        {
            if(global_dtb == 0)
            {
                uint64_t addr_val = 0;
                char found_pattern_p[8];
                char p_name[16];
                if(type == 7)
                {
                    while(ifile.eof()==0)
                    {
                        ifile.read(found_pattern_p, 8);
                        addr_val+=8;
                        if(utility_functions::compare_array(process_signature, found_pattern_p, 8))
                        {
                            ifile.ignore(736 - 8);
                            ifile.read(p_name, 16);
                            if(strcmp(p_name, "Idle")==0)
                            {
                                cout<<"Idle found at "<<hex<<addr_val-8<<endl;
                                ifile.seekg(-(736 + 16), std::ios::cur);
                                ifile.seekg(40, std::ios::cur);
                                unsigned long temp;
                                ifile.read(reinterpret_cast<char *>(&temp), sizeof(temp));
                                cout<<"DTB value for Idle "<<hex<<temp<<endl;
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
                else if(type == 10)
                {
                    
                }
            }
            else
            {
                return global_dtb;
            }
        }
        uint64_t get_service_dtb(ifstream &ifile)
        {
            if(service_dtb == 0)
            {

            }
            else
            {
                return service_dtb;
            }
        }
};

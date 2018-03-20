#include <stdint.h>

class profile 
{
    public:
        uint64_t global_dtb;
        uint64_t service_dtb;

        char *process_signature;
        int *process_offsets;

        char *service_pattern1;
        char *service_pattern2;

        char *hive_signature;
        char *hive_pool_tag;
        int *hive_offsets;

        profile (int type)
        {
            if(type == 7)
                init_as_win7();
            if(type == 10)
                init_as_win10();
        }

        void init_as_win7()
        {
            process_signature = new char[8] {3, 0, 88, 0, 0, 0, 0, 0};

            service_pattern1 = new char[8] {115, 101, 114, 72, 0, 0, 0, 0};
            service_pattern2 = new char[8] {115, 101, 114, 72, 4, 0, 0, 0};

            hive_signature = new char[4] {char(224), char(190), char(224), char(190)};
            hive_pool_tag = new char[8] {'0', '0', '0', '0', 'C', 'M', '1', '0'};
            hive_offsets = new int[1] {1776};

        }

        void init_as_win10()
        {
            process_signature = new char[8] {3, 0, 182, 0, 0, 0, 0, 0};

            service_pattern1 = new char[8] {115, 101, 114, 72, 0, 0, 0, 0};
            service_pattern2 = new char[8] {115, 101, 114, 72, 4, 0, 0, 0};

            hive_signature = new char[4] {char(224), char(190), char(224), char(190)};
            hive_pool_tag = new char[8] {'0', '0', '0', '0', 'C', 'M', '1', '0'};
            hive_offsets = new int[1] {1776};

        }

};

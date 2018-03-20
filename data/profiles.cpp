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

        }

        void init_as_win10()
        {
            process_signature = new char[8] {3, 0, 182, 0, 0, 0, 0, 0};

            service_pattern1 = new char[8] {115, 101, 114, 72, 0, 0, 0, 0};
            service_pattern2 = new char[8] {115, 101, 114, 72, 4, 0, 0, 0};

        }

}

class win7
{
    public:
        uint32_t global_dtb;
        uint32_t service_dtb;
        char process_signature[8] = {3, 0, 88, 0, 0, 0, 0, 0};
        char service_pattern1[8] = {115, 101, 114, 72, 0, 0, 0, 0};
        char service_pattern2[8] = {115, 101, 114, 72, 4, 0, 0, 0};
        char hive_signature[8] = {char(224), char(190), char(224), char(190)};
        char hive_pool_tag[8] = {'0', '0', '0', '0', 'C', 'M', '1', '0'};
        int process_offsets[4] = {376, 268, 76, 480};
        int hive_offsets[1] = {1776};
};


class win10
{
    public:
        uint32_t global_dtb;
        uint32_t service_dtb;
        char process_signature[8] = {3, 0, char(182), 0, 0, 0, 0, 0};
        char service_pattern1[8] = {115, 101, 114, 72, 0, 0, 0, 0};
        char service_pattern2[8] = {115, 101, 114, 72, 4, 0, 0, 0};
        char hive_signature[8] = {char(224), char(190), char(224), char(190)};
        char hive_pool_tag[8] = {'0', '0', '0', '0', 'C', 'M', '1', '0'};
        int process_offsets[4] = {376, 268, 76, 480};
        int hive_offsets[1] = {0};
};

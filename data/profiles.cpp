#include <stdint.h>

class profile
{
    public:
        uint32_t global_dtb;
        uint32_t service_dtb;
};

class win7: public profile
{
    public:
        char process_signature[8] = {3, 0, 88, 0, 0, 0, 0, 0};
        char service_pattern1[8] = {115, 101, 114, 72, 0, 0, 0, 0};
        char service_pattern2[8] = {115, 101, 114, 72, 4, 0, 0, 0};
        char hive_signature[8] = {char(224), char(190), char(224), char(190)};
        char hive_pool_tag[8] = "0000CM10";
        int process_offsets[4] = {376, 268, 76, 480};
        int hive_offsets[1] = {1776};
};


class win10: public profile
{
    public:
        char process_signature[8] = {3, 0, char(182), 0, 0, 0, 0, 0};
        char service_pattern1[8] = {115, 101, 114, 72, 0, 0, 0, 0};
        char service_pattern2[8] = {115, 101, 114, 72, 4, 0, 0, 0};
        char hive_signature[8] = {char(224), char(190), char(224), char(190)};
        char hive_pool_tag[8] = "0000CM10";
        int process_offsets[4] = {376, 268, 76, 480};
        int hive_offsets[1] = {0};
};

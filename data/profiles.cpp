
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
        int process_offsets[] = {376, 268, 76, 480};
};


class win10: public profile
{
    public:
        char process_signature[8] = {3, 0, 182, 0, 0, 0, 0, 0};
        char service_pattern1[8] = {115, 101, 114, 72, 0, 0, 0, 0};
        char service_pattern2[8] = {115, 101, 114, 72, 4, 0, 0, 0};
        int process_offsets[] = {376, 268, 76, 480};
};

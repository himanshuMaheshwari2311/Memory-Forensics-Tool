#ifndef _object_
#define _object_

#include <string>
#include <stdint.h>

using namespace std;

enum object_type
{
	tprocess,
	tservice,
	tkernal_module,
	thive,
	tlsa,
	tshin_cache,
	tshell_bags
};

typedef struct datetime
{
	uint8_t day;
	uint8_t month;
	uint8_t year;
	uint8_t hour;
	uint8_t minute;
	uint8_t second;
} datetime;

class object
{
  public:
	string name;
	uint64_t physical_offset;
	uint8_t object_id;
	datetime timestamp;
	void get_info(){};
};

#endif
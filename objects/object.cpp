#ifndef _object_
#define _object_

#include <string>
#include <stdint.h>

using namespace std;

template <typename T>
struct doubly_ll{
	T module;
	doubly_ll *next, *prev;
};

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
	static uint64_t counter;
	string name;
	uint64_t physical_offset;
	uint64_t object_id;
	datetime timestamp;
	object()
	{
		counter++;
		object_id = counter;
	}
	void get_info(){};
};

uint64_t object::counter = 0;

#endif
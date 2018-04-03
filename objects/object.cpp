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

class object
{
  public:
	static uint64_t counter;
	string name;
	uint64_t physical_offset;
	uint64_t object_id;
	string create_time;
	object()
	{
		counter++;
		object_id = counter;
	}
	void get_info(){};
};

uint64_t object::counter = 0;

#endif
#include "../interfaces.h"

key_values::~key_values() { interfaces::mem_alloc->free(this); }
#include "DefaultIncrement.h"

uint32_t DefaultIncrement::operator()(uint32_t size, uint32_t capacity) const
{
	uint32_t minSize = size + 1;
	if (minSize > capacity)
	{
		return size * 2 + 1;
	}
	return capacity;
}

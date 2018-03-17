#include "pch.h"
#include "HashMap.h"
#include <stdint.h>
#include <string>

using namespace std;

namespace FieaGameEngine
{
	uint32_t DefaultHash<std::string>::operator()(const std::string& key) const
	{
		uint32_t sum = 0;
		for (auto &value : key)
		{
			sum += 33 * static_cast<uint8_t>(value);
		}
		return sum;
	}

	uint32_t DefaultHash<char*>::operator()(const char* key) const
	{
		const uint8_t* data = reinterpret_cast<const uint8_t*>(key);
		return BernsteinHash(data, strlen(key));
	}

	uint32_t BernsteinHash(const uint8_t* dataPtr, size_t size)
	{
		if (dataPtr == nullptr)
		{
			throw std::exception("Null data");
		}
		uint32_t sum = 0;
		for (size_t i = 0; i < size; ++i)
		{
			sum += 33 * dataPtr[i];
		}
		return sum;
	}

	bool DefaultCompare<char*>::operator()(const char* lhs, const char* rhs) const
	{
		return !strcmp(lhs, rhs);
	}
}
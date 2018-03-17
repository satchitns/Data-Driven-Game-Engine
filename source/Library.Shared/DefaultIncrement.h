#pragma once
#include<cstdint>
/**
*@brief functor used as default type for the vector's incrementor type argument;
*contains a default reservation strategy for the vector
**/
class DefaultIncrement
{
public:
	/**
	*@brief returns new capacity amount for vector
	*@param size - current size of vector
	*@param capacity- current capacity of vector
	*@return new capacity of vector
	**/
	uint32_t operator()(uint32_t size, uint32_t capacity) const;
};


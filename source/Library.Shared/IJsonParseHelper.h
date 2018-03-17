#pragma once
#include "JsonParseMaster.h"

namespace FieaGameEngine
{
	/**
	*Abstract class to write helpers to parse Json data. The helpers will be added to a ParseMaster who will send
	*parse requests that the helper can decide whether to handle or not based on type.
	**/
	class IJsonParseHelper abstract
	{
	public:

		IJsonParseHelper() = default;
		IJsonParseHelper(const IJsonParseHelper&) = default;
		IJsonParseHelper& operator=(const IJsonParseHelper&) = default;
		virtual ~IJsonParseHelper() = default;

		/**
		*@brief Function to initialize the helper
		**/
		virtual void Initialize() {};

		/**
		*@brief Function that looks at the sharedData and determines whether
		*the request can be parsed by this helper, and does initialization for an element
		*@param name - Name of Json pair
		*@param element - Json::value to parse
		*@param data - SharedData to write into
		*@return true if value can be parsed
		**/
		virtual bool StartHandler(const std::string& name, const Json::Value& element, JsonParseMaster::SharedData& data) = 0;


		/**
		*@brief Function that looks at the sharedData and json element and determines whether
		*they can be parsed by this helper
		*@param name - Name of Json pair
		*@param element - Json::value to parse
		*@param data - SharedData to write into
		*@return true if value can be/was parsed
		**/
		virtual bool ValueHandler(const std::string& name, const Json::Value& element, JsonParseMaster::SharedData& data) = 0;

		/**
		*@brief Function that tears down the parsing of one element
		*@param name - Name of Json pair
		*@param element - Json::value to parse
		*@param data - SharedData to write into
		*@return true if value was parsed
		**/
		virtual bool EndHandler(const std::string& name, const Json::Value& element, JsonParseMaster::SharedData& data) = 0;

		/**
		*@brief Clone the helper and return a pointer to the clone
		**/
		virtual IJsonParseHelper* Clone() const = 0;
	};
}
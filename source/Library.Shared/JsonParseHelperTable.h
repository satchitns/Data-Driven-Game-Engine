#pragma once
#include "IJsonParseHelper.h"
#include "HashMap.h"
#include "Datum.h"

namespace FieaGameEngine
{
	class JsonParseHelperTable final : public IJsonParseHelper
	{
	public:
		using PairMap = HashMap<std::string, Json::Value>;
		JsonParseHelperTable() = default;
		JsonParseHelperTable(const JsonParseHelperTable&) = default;
		JsonParseHelperTable& operator=(const JsonParseHelperTable&) = default;
		JsonParseHelperTable(JsonParseHelperTable&&) = default;
		JsonParseHelperTable& operator=(JsonParseHelperTable&&) = default;
		~JsonParseHelperTable() = default;

		/**
		*@brief Function to initialize the helper
		**/
		virtual void Initialize() override;

		/**
		*@brief Clone the helper and return a pointer to the clone
		**/
		virtual IJsonParseHelper* Clone() const override;

		/**
		*@brief Function that looks at the sharedData and determines whether
		*the request can be parsed by this helper, and does initialization for an element
		*@param name - Name of Json pair
		*@param element  - json value
		*@param data - SharedData to write into
		*@return true if value can be parsed
		**/
		virtual bool StartHandler(const std::string& name, const Json::Value& element, JsonParseMaster::SharedData& data) override;


		/**
		*@brief Function that looks at the sharedData and json element and determines whether
		*they can be parsed by this helper
		*@param name - Name of Json pair
		*@param element - Json::value to parse
		*@param data - SharedData to write into
		*@return true if value can be/was parsed
		**/
		virtual bool ValueHandler(const std::string& name, const Json::Value& element, JsonParseMaster::SharedData& data) override;

		/**
		*@brief Function that tears down the parsing of one element
		*@param name - Name of Json pair
		*@param element - Json::value to parse
		*@param data - SharedData to write into
		*@return true if value was parsed
		**/
		virtual bool EndHandler(const std::string& name, const Json::Value& element, JsonParseMaster::SharedData& data) override;

	private:
		struct Attribute
		{
			Datum::DatumType type = Datum::DatumType::UNKNOWN;
			Json::Value data = Json::Value::null;
		};
		static HashMap<std::string, Datum::DatumType> sTypeMap;
		std::stack<Attribute> mAttributeStack;
	};
}
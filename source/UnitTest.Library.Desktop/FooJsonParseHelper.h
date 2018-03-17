#pragma once
#include "IJsonParseHelper.h"
#include "FooSharedData.h"

namespace FieaGameEngine
{
	class FooJsonParseHelper : public IJsonParseHelper
	{
	public:
		FooJsonParseHelper() = default;
		FooJsonParseHelper(const FooJsonParseHelper&) = default;
		FooJsonParseHelper& operator=(const FooJsonParseHelper&) = default;
		virtual ~FooJsonParseHelper() = default;
		virtual bool ValueHandler(const std::string& name, const Json::Value& element, JsonParseMaster::SharedData& data) override;
		virtual bool StartHandler(const std::string& name, const Json::Value& element, JsonParseMaster::SharedData& data) override;
		virtual bool EndHandler(const std::string& name, const Json::Value& element, JsonParseMaster::SharedData& data) override;
		virtual IJsonParseHelper* Clone() const override;
	};
}
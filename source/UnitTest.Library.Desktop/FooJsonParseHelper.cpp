#include "stdafx.h"
#include "FooJsonParseHelper.h"

namespace FieaGameEngine
{
	bool FooJsonParseHelper::ValueHandler(const std::string& name, const Json::Value& element, JsonParseMaster::SharedData & data)
	{
		UnitTestLibraryDesktop::FooSharedData* fooData = data.As<UnitTestLibraryDesktop::FooSharedData>();
		if (fooData != nullptr)
		{
			if (name == "Data" && element.isInt())
			{
				fooData->SetData(element.asInt());
				return true;
			}
			else if (name == "OtherData" && element.isInt())
			{
				fooData->SetOtherData(element.asInt());
				return true;
			}
		}
		return false;
	}

	bool FooJsonParseHelper::StartHandler(const std::string & name, const Json::Value& element, JsonParseMaster::SharedData & data)
	{
		name;
		element;
		UnitTestLibraryDesktop::FooSharedData* fooData = data.As<UnitTestLibraryDesktop::FooSharedData>();
		if (fooData != nullptr)
		{
			return true;
		}
		return false;
	}

	bool FooJsonParseHelper::EndHandler(const std::string & name, const Json::Value& element, JsonParseMaster::SharedData & data)
	{
		name;
		element;
		UnitTestLibraryDesktop::FooSharedData* fooData = data.As<UnitTestLibraryDesktop::FooSharedData>();
		if (fooData != nullptr)
		{
			return true;
		}
		return false;
	}

	IJsonParseHelper* FooJsonParseHelper::Clone() const
	{
		return new FooJsonParseHelper(*this);
	}
}
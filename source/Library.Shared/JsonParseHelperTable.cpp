#include "pch.h"
#include "JsonParseHelperTable.h"
#include "TableSharedData.h"

namespace FieaGameEngine
{
	HashMap<std::string, Datum::DatumType>  JsonParseHelperTable::sTypeMap =
	{
		{ "integer", Datum::DatumType::INTEGER },
		{ "float", Datum::DatumType::FLOAT },
		{ "vector", Datum::DatumType::VECTOR },
		{ "matrix", Datum::DatumType::MATRIX },
		{ "table", Datum::DatumType::TABLE },
		{ "string", Datum::DatumType::STRING }
	};

	bool JsonParseHelperTable::StartHandler(const std::string & name, const Json::Value& element, JsonParseMaster::SharedData & data)
	{
		TableSharedData* tableData = data.As<TableSharedData>();
		if (tableData != nullptr)
		{
			if (name.compare("type") && name.compare("data"))
			{
				Scope *scope = tableData->GetScope();
				if (element["data"].isObject())
				{
					Scope* childScope = &(scope->AppendScope(name));
					tableData->SetScope(*childScope);
				}
				else
				{
					scope->Append(name);
				}
				mAttributeStack.push(Attribute());
			}
			return true;
		}
		return false;
	}

	bool JsonParseHelperTable::ValueHandler(const std::string& name, const Json::Value& element, JsonParseMaster::SharedData& data)
	{
		TableSharedData* tableData = data.As<TableSharedData>();
		if (tableData != nullptr)
		{
			if (!name.compare("type"))
			{
				mAttributeStack.top().type = sTypeMap[element.asString()];
			}
			else if (!name.compare("data"))
			{
				mAttributeStack.top().data = element;
			}
			return true;
		}
		return false;
	}

	bool JsonParseHelperTable::EndHandler(const std::string & name, const Json::Value& element, JsonParseMaster::SharedData & data)
	{
		TableSharedData* tableData = data.As<TableSharedData>();
		if (tableData != nullptr)
		{
			if (!name.compare("data") && element.isObject())
			{
				tableData->SetScope(*(tableData->GetScope()->GetParent()));
			}
			else if (name.compare("type") && name.compare("data"))
			{
				Scope& scope = *(tableData->GetScope());
				Attribute& attribute = mAttributeStack.top();
				Datum::DatumType type = attribute.type;
				Json::Value& jsonData = attribute.data;
				if (type != Datum::DatumType::TABLE)
				{
					if (type == Datum::DatumType::UNKNOWN || jsonData.isNull())
					{
						throw std::exception("Not enough data to parse");
					}
					scope[name].SetType(type);
					if (jsonData.isArray())
					{
						uint32_t size = jsonData.size();
						scope[name].Resize(size);
						for (uint32_t i = 0; i < size; ++i)
						{
							scope[name].SetFromString(jsonData[i].asString(), i);
						}
					}
					else
					{
						scope[name].Resize(1);
						scope[name].SetFromString(jsonData.asString());
					}
				}
				mAttributeStack.pop();
			}
			return true;
		}
		return false;
	}

	IJsonParseHelper* JsonParseHelperTable::Clone() const
	{
		JsonParseHelperTable *clone = new JsonParseHelperTable(*this);
		while (!clone->mAttributeStack.empty())
		{
			clone->mAttributeStack.pop();
		}
		return clone;
	}

	void JsonParseHelperTable::Initialize()
	{
		IJsonParseHelper::Initialize();
		while (!mAttributeStack.empty())
		{
			mAttributeStack.pop();
		}
	}
}
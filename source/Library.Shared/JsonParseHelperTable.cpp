#include "pch.h"
#include "JsonParseHelperTable.h"
#include "TableSharedData.h"
#include "Sector.h"
#include "Entity.h"
#include "World.h"

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
			if (name.compare("type") && name.compare("data") && name.compare("class") && name.compare("className"))
			{
				Scope *scope = tableData->GetScope();
				if (element["data"].isObject())
				{
					Json::Value classValue = element["class"];
					Scope* childScope = nullptr;
					if (classValue.isNull())
					{
						childScope = &(scope->AppendScope(name));
					}
					else
					{
						std::string className = classValue.asString();
						if (!className.compare("entity"))
						{
							assert(scope->Is(Sector::TypeName()));
							Sector* sector = static_cast<Sector*>(scope);
							childScope = sector->CreateEntity(element["className"].asString(), name);
						}
						else if (!className.compare("sector"))
						{
							assert(scope->Is(World::TypeName()));
							World* sector = static_cast<World*>(scope);
							childScope = sector->CreateSector(name);
						}
					}
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
			else if (name.compare("type") && name.compare("data") && name.compare("class") && name.compare("className"))
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
						if (!scope[name].IsExternal())
						{
							scope[name].Resize(size);
						}
						for (uint32_t i = 0; i < size; ++i)
						{
							scope[name].SetFromString(jsonData[i].asString(), i);
						}
					}
					else
					{
						if (!scope[name].IsExternal())
						{
							scope[name].Resize(1);
						}
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
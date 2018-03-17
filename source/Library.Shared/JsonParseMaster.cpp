#include "pch.h"
#include "JsonParseMaster.h"
#include "IJsonParseHelper.h"

namespace FieaGameEngine
{
	JsonParseMaster::JsonParseMaster(SharedData& data)
	{
		SetSharedData(data);
		mData->SetJsonParseMaster(*this);
	}

	JsonParseMaster::JsonParseMaster(JsonParseMaster && other)
	{
		operator=(std::move(other));
	}

	JsonParseMaster & JsonParseMaster::operator=(JsonParseMaster && other)
	{
		if (this != &other)
		{
			if (!mIsClone)
			{
				mData = std::move(other.mData);
				mHelpers = std::move(other.mHelpers);
				mFileName = std::move(other.mFileName);
				mIsClone = false;
				other.mData = nullptr;
			}
		}
		return *this;
	}

	JsonParseMaster::~JsonParseMaster()
	{
		if (mIsClone)
		{
			for (auto &value : mHelpers)
			{
				delete value;
			}
			delete mData;
		}
	}

	JsonParseMaster * JsonParseMaster::Clone() const
	{
		SharedData * newData = mData->Clone();
		JsonParseMaster * clone = new JsonParseMaster(*newData);
		clone->mIsClone = true;
		for (uint32_t i = 0; i < mHelpers.Size(); ++i)
		{
			clone->mHelpers.PushBack(mHelpers[i]->Clone());
		}
		return clone;
	}

	void JsonParseMaster::AddHelper(IJsonParseHelper & helper)
	{
		if (mIsClone)
		{
			throw std::exception("Cannot add helper to clone");
		}
		mHelpers.PushBack(&helper);
	}

	void JsonParseMaster::RemoveHelper(IJsonParseHelper & helper)
	{
		if (mIsClone)
		{
			throw std::exception("Cannot add helper to clone");
		}
		mHelpers.Remove(&helper);
	}

	void JsonParseMaster::Parse(const std::string& data)
	{
		std::stringstream stream;
		stream << data;
		Parse(stream);
	}

	void JsonParseMaster::Parse(std::istream& stream)
	{
		Json::Value root;
		stream >> root;
		ParseValue(root);
	}

	void JsonParseMaster::ParseFromFile(const std::string& jsonString)
	{
		mFileName = jsonString;
		std::ifstream fileStream(jsonString.c_str());
		if (fileStream.fail())
		{
			throw std::exception("File failed to open");
		}
		Parse(fileStream);
	}

	const std::string & JsonParseMaster::GetFilePath() const
	{
		return mFileName;
	}

	JsonParseMaster::SharedData * JsonParseMaster::GetSharedData() const
	{
		return mData;
	}

	void JsonParseMaster::SetSharedData(SharedData & data)
	{
		mData = &data;
	}

	void JsonParseMaster::Reset()
	{
		for (auto &value : mHelpers)
		{
			value->Initialize();
		}
		mData->Initialize();
		mFileName.clear();
	}

	void JsonParseMaster::ParseValue(Json::Value& value)
	{
		for (auto & name : value.getMemberNames())
		{
			Json::Value& subValue = value[name];
			TriggerStartHelper(name, subValue);
			if (subValue.isObject())
			{
				ParseValue(subValue);
			}
			else
			{
				TriggerValueHelper(name, subValue);
			}
			TriggerEndHelper(name, subValue);
		}
	}

	void JsonParseMaster::TriggerStartHelper(const std::string& name, const Json::Value& value)
	{
		mData->IncrementDepth();
		for (auto &helper : mHelpers)
		{
			if (helper->StartHandler(name, value, *mData))
			{
				break;
			}
		}
	}

	void JsonParseMaster::TriggerValueHelper(const std::string& name, const Json::Value& value)
	{
		for (auto &helper : mHelpers)
		{
			if (helper->ValueHandler(name, value, *mData))
			{
				break;
			}
		}
	}

	void JsonParseMaster::TriggerEndHelper(const std::string& name, const Json::Value& value)
	{
		mData->DecrementDepth();
		for (auto &helper : mHelpers)
		{
			if (helper->EndHandler(name, value, *mData))
			{
				break;
			}
		}
	}

	RTTI_DEFINITIONS(JsonParseMaster::SharedData)
}
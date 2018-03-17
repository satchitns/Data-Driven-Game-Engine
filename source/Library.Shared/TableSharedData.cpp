#include "pch.h"
#include "TableSharedData.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(TableSharedData)

	TableSharedData::TableSharedData(Scope &scope)
		:mScope(&scope)
	{

	}

	TableSharedData::TableSharedData(TableSharedData&& other)
		: mScope(other.mScope)
	{
		other.mScope = nullptr;
	}

	TableSharedData & TableSharedData::operator=(TableSharedData && other)
	{
		if (this != &other)
		{
			mScope = other.mScope;
			other.mScope = nullptr;
		}
		return *this;
	}

	TableSharedData::~TableSharedData()
	{
		if (mIsClone)
		{
			delete mScope;
		}
	}

	Scope * TableSharedData::GetScope() const
	{
		return mScope;
	}

	void TableSharedData::SetScope(Scope& scope)
	{
		mScope = &scope;
	}

	FieaGameEngine::JsonParseMaster::SharedData* TableSharedData::Clone() const
	{
		Scope * scope = new Scope();
		TableSharedData *clone = new TableSharedData(*scope);
		clone->mIsClone = true;
		return clone;
	}

	void TableSharedData::Initialize()
	{
		SharedData::Initialize();
		if (!mIsClone)
		{
			mScope = nullptr;
		}
	}
}
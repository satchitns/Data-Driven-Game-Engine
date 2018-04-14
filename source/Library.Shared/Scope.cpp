#include "pch.h"
#include "RTTI.h"
#include "Scope.h"
#include "HashMap.h"

using namespace std;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Scope)

		Scope::Scope(uint32_t capacity)
		:mParent(nullptr), mTable(capacity), mVector(capacity)
	{
	}

	Scope::~Scope()
	{
		if (mParent != nullptr)
		{
			mParent->Relinquish(this);
		}
		Clear();
	}

	Scope::Scope(const Scope& other)
		:Scope(other.mVector.Capacity())
	{
		CopyScope(other);
	}

	Scope::Scope(Scope&& other)
		: mParent(other.mParent), mTable(move(other.mTable))
	{
		Reparent(move(other));
		mVector = move(other.mVector);
		other.mParent = nullptr;
	}

	Scope& Scope::operator=(const Scope& other)
	{
		if (this != &other)
		{
			Clear();
			mVector.Reserve(other.mVector.Capacity());
			mParent = nullptr;
			CopyScope(other);
		}
		return *this;
	}

	Scope& Scope::operator=(Scope&& other)
	{
		if (this != &other)
		{
			Clear();
			Reparent(move(other));
			mParent = other.mParent;
			mVector = std::move(other.mVector);
			mTable = std::move(other.mTable);
			other.mParent = nullptr;
		}
		return *this;
	}

	Datum* Scope::Find(const std::string& name) const
	{
		Datum *datum = nullptr;
		HashMap<std::string, Datum>::Iterator iterator;
		if (mTable.ContainsKey(name, iterator))
		{
			datum = &iterator->second;
		}
		return datum;
	}

	Datum* Scope::Search(const std::string& name, Scope*& foundScope) const
	{
		foundScope = const_cast<Scope*>(this);
		Datum* currentLevelResults = Find(name);
		if ((mParent == nullptr) || (currentLevelResults != nullptr))
		{
			return currentLevelResults;
		}
		return mParent->Search(name, foundScope);
	}

	Datum* Scope::Search(const std::string& name) const
	{
		Scope* dummyScope;
		return Search(name, dummyScope);
	}

	Datum& Scope::Append(const std::string& name)
	{
		if (name == string())
		{
			throw std::exception("Bad name");
		}
		bool valueInserted = false;
		auto mapIterator = mTable.Insert(make_pair(name, Datum()), valueInserted);
		if (valueInserted)
		{
			mVector.PushBack(&(*mapIterator));
		}
		return mapIterator->second;
	}

	Scope& Scope::AppendScope(const std::string& name)
	{
		if (name == string())
		{
			throw std::exception("Bad name");
		}
		Datum& foundDatum = Append(name);
		if ((foundDatum.Type() != Datum::DatumType::UNKNOWN) && (foundDatum.Type() != Datum::DatumType::TABLE))
		{
			throw exception("Name is already mapped to different type");
		}
		Scope* newScope = new Scope();
		newScope->mParent = this;
		foundDatum.PushBack(newScope);
		return *newScope;
	}

	void Scope::Adopt(Scope* child, const std::string& name)
	{
		if (child == nullptr)
		{
			throw exception("Cannot adopt nullptr");
		}
		if (child == this)
		{
			throw exception("Cannot adopt self");
		}
		Datum& foundDatum = Append(name);
		if ((foundDatum.Type() != Datum::DatumType::UNKNOWN) && (foundDatum.Type() != Datum::DatumType::TABLE))
		{
			throw exception("Name is already mapped to different type");
		}
		if (child->mParent != nullptr)
		{
			child->mParent->Relinquish(child);
		}
		child->mParent = this;
		foundDatum.PushBack(child);
	}

	Scope* Scope::GetParent() const
	{
		return mParent;
	}

	Datum& Scope::operator[](const std::string& name)
	{
		return Append(name);
	}

	Datum& Scope::operator[](uint32_t index)
	{
		return (*mVector[index]).second;
	}

	bool Scope::operator==(const Scope& other) const
	{
		if (mVector.Size() != other.mVector.Size())
		{
			return false;
		}
		bool equal = true, foundInOther;
		for (const auto& value : mVector)
		{
			foundInOther = false;
			for (const auto& otherValue : other.mVector)
			{
				if (*value == *otherValue)
				{
					foundInOther = true;
					break;
				}
			}
			if (!foundInOther)
			{
				equal = false;
				break;
			}
		}
		return equal;
	}

	std::string Scope::FindName(Scope const * scope) const
	{
		Datum *dummy = nullptr;
		uint32_t dummyIndex;
		return FindName(scope, dummy, dummyIndex);
	}

	std::string Scope::FindName(Scope const * scope, Datum*& outDatum, uint32_t& outIndex) const
	{
		for (const auto &value : mVector)
		{
			Datum &datum = value->second;
			if ((datum.Type() == Datum::DatumType::TABLE))
			{
				for (uint32_t i = 0; i < datum.Size(); ++i)
				{
					if (datum.Get<Scope*>(i) == scope)
					{
						outIndex = i;
						outDatum = &datum;
						return value->first;
					}
				}
			}
		}
		return string();
	}

	void Scope::Clear()
	{
		for (auto &value : mVector)
		{
			Datum &datum = value->second;
			if (datum.Type() == Datum::DatumType::TABLE)
			{
				for (uint32_t i = 0; i < datum.Size(); ++i)
				{
					Scope* tempScope = datum.Get<Scope*>(i);
					if (tempScope != nullptr)
					{
						tempScope->mParent = nullptr;
						delete tempScope;
					}
				}
			}
		}
		mTable.Clear();
		mVector.Clear();
	}

	void Scope::Relinquish(Scope* child)
	{
		if (child == nullptr)
		{
			throw exception("Can't relinquish null");
		}
		if (child->mParent != this)
		{
			throw exception("Passed in scope was not a child");
		}

		Datum *datum = nullptr;
		uint32_t index;
		FindName(child, datum, index);
		if (datum != nullptr)
		{
			datum->Remove(index);
			child->mParent = nullptr;
		}
	}

	bool Scope::Equals(const RTTI* rhs) const
	{
		if (rhs != nullptr && rhs->Is(TypeIdClass()))
		{
			const Scope* otherScope = static_cast<const Scope*>(rhs);
			if (otherScope == this)
			{
				return true;
			}
			return operator==(*otherScope);
		}
		return false;
	}

	void Scope::CopyScope(const Scope& other)
	{
		for (const auto &value : other.mVector)
		{
			Datum& datum = value->second;
			string& name = value->first;
			Datum& myDatum = Append(name);
			if (datum.Type() == Datum::DatumType::TABLE)
			{
				myDatum.SetType(Datum::DatumType::TABLE);
				for (uint32_t i = 0; i < datum.Size(); ++i)
				{
					Scope *otherScope = datum.Get<Scope*>(i);
					if (otherScope != nullptr)
					{

						Adopt(otherScope->Clone(), name);
					}
					else
					{
						myDatum.PushBack(otherScope);
					}
				}
			}
			else
			{
				myDatum = datum;
			}
		}
	}

	string Scope::ToString() const
	{
		string nameString;
		for (const auto & value : mVector)
		{
			nameString += value->first + ", ";
		}
		size_t length = nameString.length();
		if (length > 0)
		{
			nameString.resize(length - 2);
		}
		return nameString;
	}

	const Vector<Scope::TableElement>* Scope::GetVector() const
	{
		return &mVector;
	}

	Scope * Scope::Clone()
	{
		return new Scope(*this);
	}

	void Scope::Reparent(Scope&& other)
	{
		if (other.mParent != nullptr)
		{
			Datum *datum = nullptr;
			uint32_t index;
			other.mParent->FindName(&other, datum, index);
			if (datum != nullptr)
			{
				datum->Set(this, index);
				mParent = other.mParent;
				other.mParent = nullptr;
			}
		}
		for (const auto &value : other.mVector)
		{
			Datum& datum = value->second;
			if (datum.Type() == Datum::DatumType::TABLE)
			{
				for (uint32_t i = 0; i < datum.Size(); ++i)
				{
					Scope *otherScope = datum.Get<Scope*>(i);
					if (otherScope != nullptr)
					{

						otherScope->mParent = this;
					}
				}
			}
		}
	}
}
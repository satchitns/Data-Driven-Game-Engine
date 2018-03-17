#include "pch.h"
#include "Attributed.h"

using namespace std;
using namespace glm;

namespace FieaGameEngine
{
	HashMap <uint64_t, Attributed::StaticPair> Attributed::PrescribedMap;

	RTTI_DEFINITIONS(Attributed)

		Attributed::Attributed(uint64_t id)
	{
		string thisString = "this";
		auto it = PrescribedMap.Insert(make_pair(id, make_pair(this, Vector<string>())));
		if (it->second.first == this)
		{
			Vector<string>& vector = it->second.second;
			if (vector.Find(thisString) == vector.end())
			{
				vector.PushBack(thisString);
			}
		}
		Append(thisString).PushBack(static_cast<RTTI*>(this));
	}

	Attributed::Attributed(const Attributed& other)
		:Scope(other)
	{
		(*this)["this"] = static_cast<RTTI*>(this);
	}

	Attributed::Attributed(Attributed&& other)
		: Scope(std::move(other))
	{
		(*this)["this"] = static_cast<RTTI*>(this);
	}

	Attributed& Attributed::operator=(const Attributed &other)
	{
		if (this != &other)
		{
			Scope::operator=(other);
			(*this)["this"] = static_cast<RTTI*>(this);
		}
		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& other)
	{
		if (this != &other)
		{
			Scope::operator=(std::move(other));
			(*this)["this"] = static_cast<RTTI*>(this);
		}
		return *this;
	}

	bool Attributed::IsAttribute(const string & name) const
	{
		return (Find(name) != nullptr);
	}

	bool Attributed::IsPrescribedAttribute(const string & name) const
	{
		Vector<string>& vector = PrescribedMap[TypeIdInstance()].second;
		return (vector.Find(name) != vector.end());
	}

	Datum & Attributed::AppendAuxiliaryAttribute(const string& name)
	{
		Vector<string>& vector = PrescribedMap[TypeIdInstance()].second;
		if (vector.Find(name) != vector.end())
		{
			throw exception();
		}
		return Append(name);
	}

	const Vector<Scope::TableElement>& Attributed::Attributes() const
	{
		return *GetVector();
	}

	const Vector<string>& Attributed::PrescribedAttributes() const
	{
		return PrescribedMap[TypeIdInstance()].second;
	}

	Vector <Scope::TableElement> Attributed::AuxiliaryAttributes() const
	{
		Vector<TableElement> temp;
		const Vector<TableElement>& fullVector = *GetVector();
		for (auto i = PrescribedMap[TypeIdInstance()].second.Size(); i < fullVector.Size(); ++i)
		{
			temp.PushBack(fullVector[i]);
		}
		return temp;
	}

	void Attributed::AddInternalAttribute(const string& name, const int32_t& initialValue, uint32_t size)
	{
		HandlePrescribedAttribute(name);
		Datum& datum = Append(name);
		datum.SetType(Datum::DatumType::INTEGER);
		datum.Reserve(size);
		for (uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(initialValue);
		}
	}

	void Attributed::AddInternalAttribute(const string& name, const float& initialValue, uint32_t size)
	{
		HandlePrescribedAttribute(name);
		Datum& datum = Append(name);
		datum.SetType(Datum::DatumType::FLOAT);
		datum.Reserve(size);
		for (uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(initialValue);
		}
	}

	void Attributed::AddInternalAttribute(const string& name, const mat4& initialValue, uint32_t size)
	{
		HandlePrescribedAttribute(name);
		Datum& datum = Append(name);
		datum.SetType(Datum::DatumType::MATRIX);
		datum.Reserve(size);
		for (uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(initialValue);
		}
	}

	void Attributed::AddInternalAttribute(const string& name, const vec4& initialValue, uint32_t size)
	{
		HandlePrescribedAttribute(name);
		Datum& datum = Append(name);
		datum.SetType(Datum::DatumType::VECTOR);
		datum.Reserve(size);
		for (uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(initialValue);
		}
	}

	void Attributed::AddInternalAttribute(const string& name, const string& initialValue, uint32_t size)
	{
		HandlePrescribedAttribute(name);
		Datum& datum = Append(name);
		datum.SetType(Datum::DatumType::STRING);
		datum.Reserve(size);
		for (uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(initialValue);
		}
	}

	void Attributed::AddInternalAttribute(const string& name, RTTI* const & initialValue, uint32_t size)
	{
		HandlePrescribedAttribute(name);
		Datum& datum = Append(name);
		datum.SetType(Datum::DatumType::POINTER);
		datum.Reserve(size);
		for (uint32_t i = 0; i < size; ++i)
		{
			datum.PushBack(initialValue);
		}
	}

	void Attributed::AddNestedScope(const string& name)
	{
		HandlePrescribedAttribute(name);
		AppendScope(name);
	}

	void Attributed::AddExternalAttribute(const string& name, int32_t& address, uint32_t size)
	{
		HandlePrescribedAttribute(name);
		Append(name).SetStorage(&address, size);
	}

	void Attributed::AddExternalAttribute(const string& name, float& address, uint32_t size)
	{
		HandlePrescribedAttribute(name);
		Append(name).SetStorage(&address, size);
	}

	void Attributed::AddExternalAttribute(const string& name, mat4& address, uint32_t size)
	{
		HandlePrescribedAttribute(name);
		Append(name).SetStorage(&address, size);
	}

	void Attributed::AddExternalAttribute(const string& name, vec4& address, uint32_t size)
	{
		HandlePrescribedAttribute(name);
		Append(name).SetStorage(&address, size);
	}

	void Attributed::AddExternalAttribute(const string& name, string& address, uint32_t size)
	{
		HandlePrescribedAttribute(name);
		Append(name).SetStorage(&address, size);
	}

	void Attributed::AddExternalAttribute(const string& name, RTTI* & address, uint32_t size)
	{
		HandlePrescribedAttribute(name);
		Append(name).SetStorage(&address, size);
	}

	void Attributed::HandlePrescribedAttribute(const string & name) const
	{
		auto it = PrescribedMap.Insert(make_pair(TypeIdInstance(), make_pair(this, Vector<string>())));
		if (it->second.first == this) //first instance of this class
		{
			Vector<string>& vector = it->second.second;
			if (vector.Find(name) == vector.end())
			{
				vector.PushBack(name);
			}
		}
	}

	void Attributed::UpdateExternalAttribute(const string& name, int32_t& address, uint32_t size)
	{
		Datum *datum = Find(name);
		if (datum == nullptr || !datum->IsExternal())
		{
			throw exception("Bad name");
		}
		datum->SetStorage(&address, size);
	}

	void Attributed::UpdateExternalAttribute(const string& name, float& address, uint32_t size)
	{
		Datum *datum = Find(name);
		if (datum == nullptr || !datum->IsExternal())
		{
			throw exception("Bad name");
		}
		datum->SetStorage(&address, size);
	}

	void Attributed::UpdateExternalAttribute(const string& name, mat4& address, uint32_t size)
	{
		Datum *datum = Find(name);
		if (datum == nullptr || !datum->IsExternal())
		{
			throw exception("Bad name");
		}
		datum->SetStorage(&address, size);
	}

	void Attributed::UpdateExternalAttribute(const string& name, vec4& address, uint32_t size)
	{
		Datum *datum = Find(name);
		if (datum == nullptr || !datum->IsExternal())
		{
			throw exception("Bad name");
		}
		datum->SetStorage(&address, size);
	}

	void Attributed::UpdateExternalAttribute(const string& name, string& address, uint32_t size)
	{
		Datum *datum = Find(name);
		if (datum == nullptr || !datum->IsExternal())
		{
			throw exception("Bad name");
		}
		datum->SetStorage(&address, size);
	}

	void Attributed::UpdateExternalAttribute(const string& name, RTTI* & address, uint32_t size)
	{
		Datum *datum = Find(name);
		if (datum == nullptr || !datum->IsExternal())
		{
			throw exception("Bad name");
		}
		datum->SetStorage(&address, size);
	}
}
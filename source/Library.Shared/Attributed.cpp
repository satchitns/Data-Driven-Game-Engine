#include "pch.h"
#include "Attributed.h"
#include "TypeManager.h"

using namespace std;
using namespace glm;

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Attributed);

	Attributed::Attributed(uint64_t id)
	{
		if (!TypeManager::ContainsType(id))
		{
			throw std::exception("Type has to be registered before construction");
		}
		(*this)["this"] = static_cast<RTTI*>(this);
		Populate(id);
	}

	Attributed::Attributed(const Attributed& other)
		:Scope(other)
	{
		(*this)["this"] = static_cast<RTTI*>(this);
		UpdateExternalStorage(other.TypeIdInstance());
	}

	Attributed::Attributed(Attributed&& other)
		: Scope(std::move(other))
	{
		(*this)["this"] = static_cast<RTTI*>(this);
		UpdateExternalStorage(other.TypeIdInstance());
	}

	Attributed& Attributed::operator=(const Attributed &other)
	{
		if (this != &other)
		{
			Scope::operator=(other);
			(*this)["this"] = static_cast<RTTI*>(this);
			UpdateExternalStorage(other.TypeIdInstance());
		}
		return *this;
	}

	Attributed& Attributed::operator=(Attributed&& other)
	{
		if (this != &other)
		{
			Scope::operator=(std::move(other));
			(*this)["this"] = static_cast<RTTI*>(this);
			UpdateExternalStorage(other.TypeIdInstance());
		}
		return *this;
	}

	bool Attributed::IsAttribute(const string & name) const
	{
		return (Find(name) != nullptr);
	}

	bool Attributed::IsPrescribedAttribute(const string & name) const
	{
		const Vector<Signature>& vector = TypeManager::GetSignature(TypeIdInstance());
		for (const auto & signature : vector)
		{
			if (signature.Name() == name)
			{
				return true;
			}
		}
		return false;
	}

	Datum & Attributed::AppendAuxiliaryAttribute(const string& name)
	{
		if (IsPrescribedAttribute(name))
		{
			throw std::exception("Already prescribed");
		}
		return Append(name);
	}

	const Vector<Scope::TableElement>& Attributed::Attributes() const
	{
		return *GetVector();
	}

	const Vector<Attributed::Signature>& Attributed::PrescribedAttributes() const
	{
		return TypeManager::GetSignature(TypeIdInstance());
	}

	Vector <Scope::TableElement> Attributed::AuxiliaryAttributes() const
	{
		Vector<TableElement> temp;
		const Vector<TableElement>& fullVector = *GetVector();
		for (auto i = PrescribedAttributes().Size() + 1; i < fullVector.Size(); ++i)
		{
			temp.PushBack(fullVector[i]);
		}
		return temp;
	}

	void Attributed::Populate(uint64_t typeId)
	{
		const Vector<Signature>& signatures = TypeManager::GetSignature(typeId);
		for (const auto & signature : signatures)
		{
			Datum& datum = Append(signature.Name());
			datum.SetType(signature.Type());
			if (signature.Type() != Datum::DatumType::TABLE)
			{
				void *storageLocation = reinterpret_cast<uint8_t*>(this) + signature.Offset();
				datum.SetStorage(storageLocation, signature.Size());
			}
		}
	}

	void Attributed::UpdateExternalStorage(uint64_t typeId)
	{
		const Vector<Signature>& signatures = TypeManager::GetSignature(typeId);
		for (const auto & signature : signatures)
		{
			Datum* datum = Find(signature.Name());
			assert(datum != nullptr);
			datum->SetType(signature.Type());
			if (signature.Type() != Datum::DatumType::TABLE)
			{
				void *storageLocation = reinterpret_cast<uint8_t*>(this) + signature.Offset();
				datum->SetStorage(storageLocation, signature.Size());
			}
		}
	}

	Scope * Attributed::Clone()
	{
		return new Attributed(*this);
	}
}
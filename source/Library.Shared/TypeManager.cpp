#include "pch.h"
#include "TypeManager.h"

namespace FieaGameEngine
{

	HashMap<uint64_t, Vector<Attributed::Signature>> TypeManager::AttributeMap;

	bool TypeManager::RegisterType(uint64_t typeId, const Vector<Attributed::Signature>& signatures)
	{
		if (ContainsType(typeId))
		{
			return false;
		}
		AttributeMap[typeId] = signatures;
		return true;
	}

	const Vector<Attributed::Signature>& TypeManager::GetSignature(uint64_t typeId)
	{
		return AttributeMap.At(typeId);
	}

	bool TypeManager::ContainsType(uint64_t typeId)
	{
		return AttributeMap.ContainsKey(typeId);
	}
}
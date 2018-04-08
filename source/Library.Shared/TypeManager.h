#pragma once
#include "Vector.h"
#include "Attributed.h"

namespace FieaGameEngine
{
	class TypeManager final
	{
	public:
		TypeManager() = delete;
		TypeManager(const TypeManager&) = delete;
		TypeManager(TypeManager&&) = delete;
		TypeManager& operator=(const TypeManager&) = delete;
		TypeManager& operator=(TypeManager&&) = delete;
		~TypeManager() = default;

		/**
		*@brief Register a new type with the type manager
		*@param type ID unique ID of the type
		*@param signatures - vector of signatures for data
		**/
		static bool RegisterType(uint64_t typeId, const Vector<Attributed::Signature>& signatures);
		
		/**
		*@brief Get the signatures of a type
		*@param type ID unique ID of the type
		*@return signatures - vector of signatures for data
		**/
		static const Vector<Attributed::Signature>& GetSignature(uint64_t typeId);
		
		/**
		*@brief Check whether the type manager has the type
		*@param type ID unique ID of the type
		*@return true if type exists
		**/
		static bool ContainsType(uint64_t typeId);
	private:
		static HashMap<uint64_t, Vector<Attributed::Signature>> AttributeMap;
	};
}

#define Register(Type) TypeManager::RegisterType(Type::TypeIdClass(), Type::Signatures())
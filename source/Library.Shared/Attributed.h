#pragma once
#include "Scope.h"
#include <string>
#include <glm/glm.hpp>

namespace FieaGameEngine
{
	/**
	*@brief class that helps create a binding between C++ class and Scope. Inherits from scope and provides helper functions
	*to accomplish the binding. Has the concept of prescribed and auxiliary attributes - prescribed attributes exist for all instances,
	*auxiliary ones only for some specified instances.
	*Prescribed attributes are registered onto a static hashmap.
	**/
	class Attributed : public Scope
	{
		RTTI_DECLARATIONS(Attributed, Scope)
	public:

		class Signature
		{
		public:

			Signature(const std::string& name, Datum::DatumType type, uint32_t size = 0, uint32_t offset = 0)
				:mName(name), mType(type), mSize(size), mOffset(offset)
			{
			}

			/**
			*@brief accessor for name
			*@return name const string reference
			**/
			const std::string& Name() const
			{
				return mName;
			}

			/**
			*@brief accessor for size
			*@return size
			**/
			uint32_t Size() const
			{
				return mSize;
			}

			/**
			*@brief accessor for offset
			*@return offset
			**/
			uint32_t Offset() const
			{
				return mOffset;
			}

			/**
			*@brief accessor for type
			*@return type
			**/
			Datum::DatumType Type() const
			{
				return mType;
			}

		private:
			std::string mName;
			Datum::DatumType mType = Datum::DatumType::UNKNOWN;
			uint32_t mSize;
			uint32_t mOffset;
		};

		/**
		*@brief constructor
		*@param id - ID of child (required in order to insert a prescribed attribute containing the this pointer)
		**/
		Attributed(uint64_t id);

		virtual ~Attributed() = default;

		/**
		*@brief Copy constructor for attributed
		*@param rhs to copy from
		**/
		Attributed(const Attributed& other);

		/**
		*@brief Move constructor for attributed
		*@param rhs to move from
		**/
		Attributed(Attributed&& other);

		/**
		*@brief Copy assignment operator
		*@param rhs to copy from
		*@return reference to this
		**/
		Attributed& operator=(const Attributed &other);

		/**
		*@brief Move assignment operator
		*@param rhs to move from
		*@return reference to this
		**/
		Attributed& operator=(Attributed&& other);

		/**
		*@brief Returns true if the given name is attribute
		*@param name - name of attribute to check
		*@return true if the given name is attribute
		**/
		bool IsAttribute(const std::string& name) const;

		/**
		*@brief Returns true if the given attribute is prescribed
		*@param name - name of attribute to check
		*@return true if the given attribute is prescribed
		**/
		bool IsPrescribedAttribute(const std::string& name) const;

		/**
		*@brief Returns true if the given attribute is auxiliary
		*@param name - name of attribute to check
		*@return true if the given attribute is auxiliary
		**/
		bool IsAuxiliaryAttribute(const std::string& name) const
		{
			return ((IsAttribute(name)) && (!IsPrescribedAttribute(name)));
		}

		/**
		*@brief Appends an auxiliary attribute to the wrapped scope with the given name
		*@param name - name of attribute to append
		*@return Reference to the datum appended (or the datum with the name if it already existed as auxiliary)
		*@throws exception if name is prescribed attribute
		**/
		Datum& AppendAuxiliaryAttribute(const std::string& name);

		/**
		*@brief Accessor for all attributes
		*@return const reference to a vector of pointers to pairs of name and datum
		**/
		const Vector<TableElement>& Attributes() const;

		/**
		*@brief Accessor for prescribed attributes
		*@return const reference to a vector of names
		**/
		const Vector<Signature>& PrescribedAttributes() const;

		/**
		*@brief Accessor for auxiliary attributes
		*@return vector of pointers to pairs of name and datum
		**/
		Vector<TableElement> AuxiliaryAttributes() const;
	protected:

	private:	
		/**
		*@brief Update external prescribed attributes for a class
		*@param Type ID of class to update
		**/
		void UpdateExternalStorage(uint64_t typeId);

		/**
		*@brief Populate the scope with the prescribed attributes
		*@param Type ID of class to populate
		**/
		void Populate(uint64_t typeId);
	};
}
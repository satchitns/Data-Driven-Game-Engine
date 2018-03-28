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
		typedef std::pair<const Attributed*, Vector<std::string >> StaticPair;

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
		const Vector<std::string>& PrescribedAttributes() const;

		/**
		*@brief Accessor for auxiliary attributes
		*@return vector of pointers to pairs of name and datum
		**/
		Vector<TableElement> AuxiliaryAttributes() const;
	protected:
		/**
		*@brief hashmap to store a vector of prescribed attributes for each type inheriting from attributed
		**/
		static HashMap <uint64_t, StaticPair> PrescribedMap;

		/**
		*@brief Method to add an internal prescribed attribute
		*@param name - name of the attribute
		*@param initialValue - starting Value
		*@param size - number of elements to allocate
		**/
		void AddInternalAttribute(const std::string& name, const int32_t& initialValue, uint32_t size = 1);

		/**
		*@brief Method to add an internal prescribed attribute
		*@param name - name of the attribute
		*@param initialValue - starting Value
		*@param size - number of elements to allocate
		**/
		void AddInternalAttribute(const std::string& name, const float& initialValue, uint32_t size = 1);

		/**
		*@brief Method to add an internal prescribed attribute
		*@param name - name of the attribute
		*@param initialValue - starting Value
		*@param size - number of elements to allocate
		**/
		void AddInternalAttribute(const std::string& name, const glm::mat4& initialValue, uint32_t size = 1);

		/**
		*@brief Method to add an internal prescribed attribute
		*@param name - name of the attribute
		*@param initialValue - starting Value
		*@param size - number of elements to allocate
		**/
		void AddInternalAttribute(const std::string& name, const glm::vec4& initialValue, uint32_t size = 1);

		/**
		*@brief Method to add an internal prescribed attribute
		*@param name - name of the attribute
		*@param initialValue - starting Value
		*@param size - number of elements to allocate
		**/
		void AddInternalAttribute(const std::string& name, const std::string& initialValue, uint32_t size = 1);

		/**
		*@brief Method to add an internal prescribed attribute
		*@param name - name of the attribute
		*@param initialValue - starting Value
		*@param size - number of elements to allocate
		**/
		void AddInternalAttribute(const std::string& name, RTTI* const & initialValue, uint32_t size = 1);

		/**
		*@brief Method to add a scope within this scope
		*@param name - name of the scope's datum
		**/
		void AddNestedScope(const std::string& name);

		/**
		*@brief Method to create a scope within this scope
		*@param name - name of the scope's datum
		**/
		Datum& CreateNestedScope(const std::string& name);

		/**
		*@brief Method to add an external prescribed attribute (usually mapped to the class's data member)
		*@param name - name of the attribute
		*@param address - address of external storage (data member)
		*@param size - number of elements
		**/
		void AddExternalAttribute(const std::string& name, int32_t& address, uint32_t size = 1);

		/**
		*@brief Method to add an external prescribed attribute (usually mapped to the class's data member)
		*@param name - name of the attribute
		*@param address - address of external storage (data member)
		*@param size - number of elements
		**/
		void AddExternalAttribute(const std::string& name, float& address, uint32_t size = 1);

		/**
		*@brief Method to add an external prescribed attribute (usually mapped to the class's data member)
		*@param name - name of the attribute
		*@param address - address of external storage (data member)
		*@param size - number of elements
		**/
		void AddExternalAttribute(const std::string& name, glm::mat4& address, uint32_t size = 1);

		/**
		*@brief Method to add an external prescribed attribute (usually mapped to the class's data member)
		*@param name - name of the attribute
		*@param address - address of external storage (data member)
		*@param size - number of elements
		**/
		void AddExternalAttribute(const std::string& name, glm::vec4& address, uint32_t size = 1);

		/**
		*@brief Method to add an external prescribed attribute (usually mapped to the class's data member)
		*@param name - name of the attribute
		*@param address - address of external storage (data member)
		*@param size - number of elements
		**/
		void AddExternalAttribute(const std::string& name, std::string& address, uint32_t size = 1);

		/**
		*@brief Method to add an external prescribed attribute (usually mapped to the class's data member)
		*@param name - name of the attribute
		*@param address - address of external storage (data member)
		*@param size - number of elements
		**/
		void AddExternalAttribute(const std::string& name, RTTI* & address, uint32_t size = 1);

		/**
		*@brief Method to update address of an external prescribed attribute (usually mapped to the class's data member)
		*@param name - name of the attribute
		*@param address - address of external storage (data member)
		*@param size - number of elements
		**/
		void UpdateExternalAttribute(const std::string& name, int32_t& address, uint32_t size = 1);

		/**
		*@brief Method to update address of an external prescribed attribute (usually mapped to the class's data member)
		*@param name - name of the attribute
		*@param address - address of external storage (data member)
		*@param size - number of elements
		**/
		void UpdateExternalAttribute(const std::string& name, float& address, uint32_t size = 1);

		/**
		*@brief Method to update address of an external prescribed attribute (usually mapped to the class's data member)
		*@param name - name of the attribute
		*@param address - address of external storage (data member)
		*@param size - number of elements
		**/
		void UpdateExternalAttribute(const std::string& name, glm::mat4& address, uint32_t size = 1);

		/**
		*@brief Method to update address of an external prescribed attribute (usually mapped to the class's data member)
		*@param name - name of the attribute
		*@param address - address of external storage (data member)
		*@param size - number of elements
		**/
		void UpdateExternalAttribute(const std::string& name, glm::vec4& address, uint32_t size = 1);

		/**
		*@brief Method to update address of an external prescribed attribute (usually mapped to the class's data member)
		*@param name - name of the attribute
		*@param address - address of external storage (data member)
		*@param size - number of elements
		**/
		void UpdateExternalAttribute(const std::string& name, std::string& address, uint32_t size = 1);

		/**
		*@brief Method to update address of an external prescribed attribute (usually mapped to the class's data member)
		*@param name - name of the attribute
		*@param address - address of external storage (data member)
		*@param size - number of elements
		**/
		void UpdateExternalAttribute(const std::string& name, RTTI* & address, uint32_t size = 1);

	private:
		void HandlePrescribedAttribute(const std::string& name) const;
	};
}
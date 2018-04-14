#pragma once
#include<utility>
#include<Vector.h>
#include<Datum.h>
#include<RTTI.h>
#include<HashMap.h>

namespace FieaGameEngine
{
	/**
	*@brief Scope objects are tables that create dictionary of name-value pairs where Datum
	*objects (Refer Datum.h) are the values. Each entry in a Scope table has a name and a Datum,
	*where the Datum represents an array of values of a single type. Furthermore, an entry
	*in a Scope table can refer to another Scope table and thereby provides the means to create
	*user-defined types which are a Datum type.  So the Datum&Scope classes form a recursive pair:
	*Scopes are tables of Datum, some of which can be other tables (i.e. Scopes).
	*Also, since each Scope has a pointer to its parent, this forms a tree of Scopes.
	**/
	class Scope : public RTTI
	{
		RTTI_DECLARATIONS(Scope, RTTI)

	public:
		typedef std::pair<std::string, Datum>* TableElement;

		/**
		*@brief Default Scope constructor
		*@param capacity - number of elements in the scope's map
		**/
		explicit Scope(uint32_t capacity = 20);
		/**
		*@brief Destructor for scope
		**/
		virtual ~Scope();

		/**
		*@brief Copy constructor
		*@param other - Const  reference to scope to copy
		**/
		Scope(const Scope& other);

		/**
		*@brief Move constructor
		*@param other - Const reference to scope to move
		**/
		Scope(Scope&& other);

		/**
		*@brief Copy assignment operator
		*@param other - Const  reference to scope to copy
		*@return reference to this
		**/
		Scope& operator=(const Scope& other);

		/**
		*@brief Move assignment operator
		*@param other - Const  reference to scope to move
		*@return reference to this
		**/
		Scope& operator=(Scope&& other);

		/**
		*@brief Finds a Datum given the name
		*@param name - name of the datum to find
		*@return Pointer to datum with given name; Returns null if nothing was found
		**/
		Datum* Find(const std::string& name) const;


		/**
		*@brief Searches for the most closely-nested datum with the given name (up the scope tree)
		*@param name - name of the datum to find
		*@param foundScope out - output parameter in which the scope in which name was found is
		*returned
		*@return Pointer to datum with given name; Returns null if nothing was found
		**/
		Datum *Search(const std::string& name, Scope*& foundScope) const;

		/**
		*@brief Searches for the most closely-nested datum with the given name (up the scope tree)
		*@param name - name of the datum to find
		*@return Pointer to datum with given name; Returns null if nothing was found
		**/
		Datum *Search(const std::string& name) const;

		/**
		*@brief Creates a datum, maps it to the given name and returns a reference to it;
		*if a datum with the name already exists, a reference to that is returned instead
		*@param name - name of the datum to append/return
		*@return Reference to datum with the name (whether new or existing)
		**/
		Datum& Append(const std::string& name);

		/**
		*@brief Creates a new scope and inserts its address into the datum with the given name
		*- creates a datum if none exists
		*@param name - name of the datum to append to
		*@return Reference to new scope
		*@throws exception if a datum with the name already exists, and has non-Scope type
		**/
		Scope& AppendScope(const std::string& name);

		/**
		*@brief Reparents the passed in scope to calling scope, and inserts it at the datum
		*with the given name (creating one if it doesn't exist)
		*@param name - name of the datum to append to
		*@throws exception if a datum with the given name already exists, and has non-Scope type
		*@throws exception if self is adopted
		**/
		void Adopt(Scope* child, const std::string& name);

		/**
		*@brief Returns a pointer to the parent scope
		*@return parent scope pointer
		**/
		Scope* GetParent() const;

		/**
		*@brief Wraps append for convinience
		*@param name - name of datum to append/return
		*@return Reference to datum with given name
		**/
		Datum& operator[](const std::string& name);

		/**
		*@brief Finds the datum at the given index and returns it
		*@param index - index of the datum to return (in order of insertion)
		*@return Reference to datum at index
		*@throws exception from vector if index is incorrect
		**/
		Datum& operator[](uint32_t index);

		/**
		*@brief operator == - Compares two Scopes datum-by-datum, checking for matching
		name-datum pairs. Does not look for matching order, or matching parents.
		*@return true if scopes are equal
		**/
		bool operator==(const Scope& other) const;

		/**
		*@brief operator != - Compares two Scopes datum-by-datum, checking for matching
		name-datum pairs. Does not look for matching order, or matching parents.
		*@return false if scopes are equal
		**/
		bool operator!=(const Scope& other) const
		{
			return (!operator==(other));
		}

		/**
		*@brief Finds the name of the datum that contains the given scope's pointer
		*@param scope - scope whose name is to be found
		*@return name of datum, empty if the scope isn't nested in this datum
		**/
		std::string FindName(Scope const * scope) const;

		/**
		*@brief Finds the name of the datum that contains the given scope's pointer
		*@param scope - scope whose name is to be found
		*@param outDatum - returns datum by reference
		*@param outIndex - returns index by reference
		*@return name of datum, empty if the scope isn't nested in this datum
		**/
		std::string FindName(Scope const * scope, Datum*& outDatum, uint32_t& outIndex) const;

		/**
		*@brief Deletes all nested datums, and clears out the table
		**/
		void Clear();

		/**
		*@brief Relinquishes the scope passed in if it is a child
		*@param child - pointer to child
		*@throws exception if child is nullptr
		*@throws exception if passed-in scope is not a child of this
		**/
		void Relinquish(Scope* child);

		/**
		*@brief override for RTTI Equals, wraps ==
		*@param rhs - pointer to right side
		*@returns true if rhs is a scope, and the scopes are equal
		**/
		virtual bool Equals(const RTTI* rhs) const override;

		/**
		*@brief Returns a string representation of the scope
		*@return String representation of Scope
		**/
		virtual std::string ToString() const override;
	protected:
		const Vector<TableElement>* GetVector() const;
		virtual Scope* Clone();
	private:
		void CopyScope(const Scope& other);
		void Reparent(Scope&& other);
		Vector<TableElement> mVector;
		HashMap<std::string, Datum> mTable;
		Scope * mParent;
	};
}
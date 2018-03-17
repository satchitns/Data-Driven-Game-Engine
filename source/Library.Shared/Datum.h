#pragma once
#include<string>
#pragma warning(disable:4201)
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <cstdint>
#pragma warning(default:4201)

namespace FieaGameEngine
{
	class Scope;
	class RTTI;

	/**
	*@brief Run-time polymorphic array of elements. Can contain ints, floats, 4-vectors (glm::vec4),
	*4x4-matrices (glm::mat4), strings, pointers to custom-RTTI objects(extending from
	*FieaGameEngine::RTTI provided in RTTI.h, that provides custom run-time type information).
	*or pointers to scopes. Also provides an option to wrap external buffers to access and
	*modify existing elements using the interface.
	*
	**/
	class Datum final
	{
	public:
		enum class DatumType
		{
			UNKNOWN,
			INTEGER,
			FLOAT,
			VECTOR,
			MATRIX,
			TABLE,
			STRING,
			POINTER,
			MAX
		};

		/**
		*@brief Default constructor for datum, does not allocate memory
		**/
		explicit Datum(DatumType type = DatumType::UNKNOWN);

		/**
		*@brief Destructor, clears and frees if internal (does nothing if external)
		**/
		~Datum();

		/**
		*@brief Copy constructor
		*@param other - const reference to Datum to copy from
		**/
		Datum(const Datum& other);

		/**
		*@brief Move constructor
		*@param other - reference to Datum to move from
		**/
		Datum(Datum&& other);

		/**
		*@brief Parameterized scalar int constructor
		*@param other - const reference to int to copy from
		**/
		Datum(const int32_t& other);

		/**
		*@brief Parameterized scalar float constructor
		*@param other - const reference to float to copy from
		**/
		Datum(const float& other);

		/**
		*@brief Parameterized scalar vec4 constructor
		*@param other - const reference to vec4 to copy from
		**/
		Datum(const glm::vec4& other);

		/**
		*@brief Parameterized scalar mat4 constructor
		*@param other - const reference to mat4 to copy from
		**/
		Datum(const glm::mat4& other);

		/**
		*@brief Parameterized scalar string constructor
		*@param other - const reference to string to copy from
		**/
		Datum(const std::string& other);

		/**
		*@brief Parameterized scalar RTTI constructor
		*@param other - const reference to RTTI to copy from
		**/
		Datum(RTTI* const & other);

		/**
		*@brief Parameterized scalar Scope constructor
		*@param other - const reference to Scope to copy from
		**/
		Datum(Scope* const & other);

		/**
		*@brief Copy assignment operator
		*@param other - const reference to Datum to copy from
		*@return reference to self
		**/
		Datum& operator=(const Datum& other);

		/**
		*@brief Move assignment operator
		*@param other - reference to Datum to move from
		*@return reference to self
		**/
		Datum& operator=(Datum&& other);

		/**
		*@brief Copy assignment operator (scalar)
		*@param other - const reference to int to copy from (for zeroth element)
		*@return reference to self
		**/
		Datum& operator=(const int32_t& other);

		/**
		*@brief Copy assignment operator (scalar)
		*@param other - const reference to float to copy from (for zeroth element)
		*@return reference to self
		**/
		Datum& operator=(const float& other);

		/**
		*@brief Copy assignment operator (scalar)
		*@param other - const reference to vec4 to copy from (for zeroth element)
		*@return reference to self
		**/
		Datum& operator=(const glm::vec4& other);

		/**
		*@brief Copy assignment operator (scalar)
		*@param other - const reference to mat4 to copy from (for zeroth element)
		*@return reference to self
		**/
		Datum& operator=(const glm::mat4& other);

		/**
		*@brief Copy assignment operator (scalar)
		*@param other - const reference to string to copy from (for zeroth element)
		*@return reference to self
		**/
		Datum& operator=(const std::string& other);

		/**
		*@brief Copy assignment operator (scalar)
		*@param other - const reference to RTTI to copy from (for zeroth element)
		*@return reference to self
		**/
		Datum& operator=(RTTI* const & other);

		/**
		*@brief Copy assignment operator (scalar)
		*@param other - const reference to Scope to copy from (for zeroth element)
		*@return reference to self
		**/
		Datum& operator=(Scope* const & other);

		/**
		*@brief Returns enum-object indicating type
		*@return Type of Datum
		**/
		DatumType Type() const;

		/**
		*@brief Sets type of datum
		*@param type - Type to set to
		*@throws exception if attempting to set to unknown or max
		**/
		void SetType(DatumType type);

		/**
		*@brief Returns size as unsigned int
		*@return unsigned int indicating size
		**/
		uint32_t Size() const;

		/**
		*@brief Returns capacity as unsigned int
		*@return unsigned int indicating capacity
		**/
		uint32_t Capacity() const;

		/**
		*@brief Grows or shrinks the array according to passed in size -
		*allocates/deallocates memory and initializes elements as required,
		*capacity is set to size
		*@param size - Size to grow/shrink to
		*@throws exception if Datum is external
		**/
		void Resize(uint32_t size);

		/**
		*@brief removes all elements in the array without affecting capacity
		*@throws exception if Datum is external
		**/
		void Clear();

		/**
		*@brief Allocates memory for the datum
		*@param capacity - number of elements to allocate for
		*@throws exception if Datum is external
		**/
		void Reserve(uint32_t capacity);

		/**
		*@brief Sets storage to an external array
		*@param data - int array to set storage to
		*@count - number of elements in the array (cannot be verified)
		*@throws exception if count is 0
		*@throws exception if array is already internal and contains elements
		*@throws exception if type was already set to a different type
		*@throws exception if array is nullptr
		**/
		void SetStorage(int32_t* data, uint32_t count);

		/**
		*@brief Sets storage to an external array
		*@param data - float array to set storage to
		*@count - number of elements in the array (cannot be verified)
		*@throws exception if count is 0
		*@throws exception if array is already internal and contains elements
		*@throws exception if type was already set to a different type
		*@throws exception if array is nullptr
		**/
		void SetStorage(float* data, uint32_t count);

		/**
		*@brief Sets storage to an external array
		*@param data - vec4 array to set storage to
		*@count - number of elements in the array (cannot be verified)
		*@throws exception if count is 0
		*@throws exception if array is already internal and contains elements
		*@throws exception if type was already set to a different type
		*@throws exception if array is nullptr
		**/
		void SetStorage(glm::vec4* data, uint32_t count);

		/**
		*@brief Sets storage to an external array
		*@param data - mat4 array to set storage to
		*@count - number of elements in the array (cannot be verified)
		*@throws exception if count is 0
		*@throws exception if array is already internal and contains elements
		*@throws exception if type was already set to a different type
		*@throws exception if array is nullptr
		**/
		void SetStorage(glm::mat4* data, uint32_t count);

		/**
		*@brief Sets storage to an external array
		*@param data - RTTI array to set storage to
		*@count - number of elements in the array (cannot be verified)
		*@throws exception if count is 0
		*@throws exception if array is already internal and contains elements
		*@throws exception if type was already set to a different type
		*@throws exception if array is nullptr
		**/
		void SetStorage(RTTI** data, uint32_t count);

		/**
		*@brief Sets storage to an external array
		*@param data - string array to set storage to
		*@count - number of elements in the array (cannot be verified)
		*@throws exception if count is 0
		*@throws exception if array is already internal and contains elements
		*@throws exception if type was already set to a different type
		*@throws exception if array is nullptr
		**/
		void SetStorage(std::string* data, uint32_t count);

		/**
		*@brief Equality check for datum to another
		*@param other - const reference to other datum
		*@return bool - true if datums are equal
		**/
		bool operator==(const Datum& other) const;

		/**
		*@brief Equality check for datum to int (scalar)
		*@param other - const reference to int
		*@return bool - true if datum is scalar and equal to other
		**/
		inline bool operator==(const int32_t& other) const
		{
			return ((mType == DatumType::INTEGER) && (mSize == 1) && (mData.i[0] == other));
		}

		/**
		*@brief Equality check for datum to float (scalar)
		*@param other - const reference to float
		*@return bool - true if datum is scalar and equal to other
		**/
		inline bool Datum::operator==(const float& other) const
		{
			return ((mType == DatumType::FLOAT) && (mSize == 1) && (mData.f[0] == other));
		}

		/**
		*@brief Equality check for datum to vec4 (scalar)
		*@param other - const reference to vec4
		*@return bool - true if datum is scalar and equal to other
		**/
		inline bool operator==(const glm::vec4& other) const
		{
			return ((mType == DatumType::VECTOR) && (mSize == 1) && (mData.v[0] == other));
		}

		/**
		*@brief Equality check for datum to mat4 (scalar)
		*@param other - const reference to mat4
		*@return bool - true if datum is scalar and equal to other
		**/
		inline bool operator==(const glm::mat4& other) const
		{
			return ((mType == DatumType::MATRIX) && (mSize == 1) && (mData.m[0] == other));
		}

		/**
		*@brief Equality check for datum to string (scalar)
		*@param other - const reference to string
		*@return bool - true if datum is scalar and equal to other
		**/
		inline bool operator==(const std::string& other) const
		{
			return ((mType == DatumType::STRING) && (mSize == 1) && (mData.s[0] == other));
		}

		/**
		*@brief Equality check for datum to RTTI (scalar)
		*@param other - const reference to RTTI
		*@return bool - true if datum is scalar and equal to other
		**/
		inline bool operator==(RTTI* const & other) const
		{
			return ((mType == DatumType::POINTER) && (mSize == 1) && ((mData.p[0] == other) || (mData.p[0] != nullptr) && (mData.p[0]->Equals(other))));
		}

		/**
		*@brief Inequality check for datum to another
		*@param other - const reference to other datum
		*@return bool - false if datums are equal
		**/
		bool operator!=(const Datum& other) const;

		/**
		*@brief Inequality check for datum to int (scalar)
		*@param other - const reference to int
		*@return bool - false if datum is scalar and equal to other
		**/
		inline bool operator!=(const int32_t& other) const
		{
			return !operator==(other);
		}

		/**
		*@brief Inequality check for datum to float (scalar)
		*@param other - const reference to float
		*@return bool - false if datum is scalar and equal to other
		**/
		inline bool operator!=(const float& other) const
		{
			return !operator==(other);
		}

		/**
		*@brief Inequality check for datum to vec4 (scalar)
		*@param other - const reference to vec4
		*@return bool - false if datum is scalar and equal to other
		**/
		inline bool operator!=(const glm::vec4& other) const
		{
			return !operator==(other);
		}

		/**
		*@brief Inequality check for datum to mat4 (scalar)
		*@param other - const reference to mat4
		*@return bool - false if datum is scalar and equal to other
		**/
		inline bool operator!=(const glm::mat4& other) const
		{
			return !operator==(other);
		}

		/**
		*@brief Inequality check for datum to string (scalar)
		*@param other - const reference to string
		*@return bool - false if datum is scalar and equal to other
		**/
		inline bool operator!=(const std::string& other) const
		{
			return !operator==(other);
		}

		/**
		*@brief Inequality check for datum to RTTI (scalar)
		*@param other - const reference to RTTI
		*@return bool - false if datum is scalar and equal to other
		**/
		inline bool operator!=(RTTI* const & other) const
		{
			return !operator==(other);
		}

		/**
		*@brief Set the element at the index to the given value
		*@param value - int value to set to
		*@param index (optional, defaults to 0) - index to set at
		*@throws exception if index is outside range
		*@throws exception if type doesn't match
		**/
		void Set(const int32_t& value, uint32_t index = 0);

		/**
		*@brief Set the element at the index to the given value
		*@param value - float value to set to
		*@param index (optional, defaults to 0) - index to set at
		*@throws exception if index is outside range
		*@throws exception if type doesn't match
		**/
		void Set(const float& value, uint32_t index = 0);

		/**
		*@brief Set the element at the index to the given value
		*@param value - vector value to set to
		*@param index (optional, defaults to 0) - index to set at
		*@throws exception if index is outside range
		*@throws exception if type doesn't match
		**/
		void Set(const glm::vec4& value, uint32_t index = 0);

		/**
		*@brief Set the element at the index to the given value
		*@param value - matrix value to set to
		*@param index (optional, defaults to 0) - index to set at
		*@throws exception if index is outside range
		*@throws exception if type doesn't match
		**/
		void Set(const glm::mat4& value, uint32_t index = 0);

		/**
		*@brief Set the element at the index to the given value
		*@param value - string value to set to
		*@param index (optional, defaults to 0) - index to set at
		*@throws exception if index is outside range
		*@throws exception if type doesn't match
		**/
		void Set(const std::string& value, uint32_t index = 0);

		/**
		*@brief Set the element at the index to the given value
		*@param value - RTTI value to set to
		*@param index (optional, defaults to 0) - index to set at
		*@throws exception if index is outside range
		*@throws exception if type doesn't match
		**/
		void Set(RTTI* const & value, uint32_t index = 0);

		/**
		*@brief Set the element at the index to the given value
		*@param value - Scope value to set to
		*@param index (optional, defaults to 0) - index to set at
		*@throws exception if index is outside range
		*@throws exception if type doesn't match
		**/
		void Set(Scope* const & value, uint32_t index = 0);

		/**
		*@brief Push the given element into the array, reallocating if necessary
		*@param value - int value to push
		*@throws exception if type doesn't match
		*@throws exception if datum is external
		**/
		void PushBack(const int32_t& value);

		/**
		*@brief Push the given element into the array, reallocating if necessary
		*@param value - float value to push
		*@throws exception if type doesn't match
		*@throws exception if datum is external
		**/
		void PushBack(const float& value);

		/**
		*@brief Push the given element into the array, reallocating if necessary
		*@param value - vector value to push
		*@throws exception if type doesn't match
		*@throws exception if datum is external
		**/
		void PushBack(const glm::vec4& value);

		/**
		*@brief Push the given element into the array, reallocating if necessary
		*@param value - matrix value to push
		*@throws exception if type doesn't match
		*@throws exception if datum is external
		**/
		void PushBack(const glm::mat4& value);

		/**
		*@brief Push the given element into the array, reallocating if necessary
		*@param value - string value to push
		*@throws exception if type doesn't match
		*@throws exception if datum is external
		**/
		void PushBack(const std::string& value);

		/**
		*@brief Push the given element into the array, reallocating if necessary
		*@param value - RTTI value to push
		*@throws exception if type doesn't match
		*@throws exception if datum is external
		**/
		void PushBack(RTTI* const & value);


		/**
		*@brief Push the given element into the array, reallocating if necessary
		*@param value - Scope value to push
		*@throws exception if type doesn't match
		*@throws exception if datum is external
		**/
		void PushBack(Scope* const & value);

		/**
		*@brief Pop the last element in the array
		*@throws exception if datum is external
		*@throws exception if size is 0
		**/
		void PopBack();
		
		/**
		*@brief Finds a value and returns whether found, and the index by reference
		*@param value - value to find
		*@param index - out parameter to return index in if found
		*@return true if element was found
		**/
		bool Find(const int& value, uint32_t &index) const;

		/**
		*@brief Finds a value and returns whether found, and the index by reference
		*@param value - value to find
		*@param index - out parameter to return index in if found
		*@return true if element was found
		**/
		bool Find(const float& value, uint32_t &index) const;

		/**
		*@brief Finds a value and returns whether found, and the index by reference
		*@param value - value to find
		*@param index - out parameter to return index in if found
		*@return true if element was found
		**/
		bool Find(const std::string& value, uint32_t &index) const;

		/**
		*@brief Finds a value and returns whether found, and the index by reference
		*@param value - value to find
		*@param index - out parameter to return index in if found
		*@return true if element was found
		**/
		bool Find(const glm::mat4& value, uint32_t &index) const;

		/**
		*@brief Finds a value and returns whether found, and the index by reference
		*@param value - value to find
		*@param index - out parameter to return index in if found
		*@return true if element was found
		**/
		bool Find(const glm::vec4& value, uint32_t &index) const;

		/**
		*@brief Finds a value and returns whether found, and the index by reference
		*@param value - value to find
		*@param index - out parameter to return index in if found
		*@return true if element was found
		**/
		bool Find(RTTI* const & value, uint32_t &index) const;

		/**
		*@brief Finds a value and returns whether found, and the index by reference
		*@param value - value to find
		*@param index - out parameter to return index in if found
		*@return true if element was found
		**/
		bool Find(Scope* const & value, uint32_t &index) const;

		/**
		*@brief Finds a value and returns whether found
		*@param value - value to find
		*@return true if element was found
		**/
		bool Find(const int& value) const;

		/**
		*@brief Finds a value and returns whether found
		*@param value - value to find
		*@return true if element was found
		**/
		bool Find(const float& value) const;

		/**
		*@brief Finds a value and returns whether found
		*@param value - value to find
		*@return true if element was found
		**/
		bool Find(const std::string& value) const;

		/**
		*@brief Finds a value and returns whether found
		*@param value - value to find
		*@return true if element was found
		**/
		bool Find(const glm::mat4& value) const;

		/**
		*@brief Finds a value and returns whether found
		*@param value - value to find
		*@return true if element was found
		**/
		bool Find(const glm::vec4& value) const;

		/**
		*@brief Finds a value and returns whether found
		*@param value - value to find
		*@return true if element was found
		**/
		bool Find(RTTI* const & value) const;

		/**
		*@brief Finds a value and returns whether found
		*@param value - value to find
		*@return true if element was found
		**/
		bool Find(Scope* const & value) const;

		/**
		*@brief Removes a value at the index if it's legal
		*@param index - index to remove at
		*@return true if element was removed (or if index is illegal)
		**/
		bool Remove(uint32_t index);

		/**
		*@brief Removes the first instance of value passed in
		*@param value - value to remove
		*@return true if element was removed (i.e at least one instance exists)
		**/
		bool Remove(const int& value);

		/**
		*@brief Removes the first instance of value passed in
		*@param value - value to remove
		*@return true if element was removed (i.e at least one instance exists)
		**/
		bool Remove(const float& value);

		/**
		*@brief Removes the first instance of value passed in
		*@param value - value to remove
		*@return true if element was removed (i.e at least one instance exists)
		**/
		bool Remove(const glm::vec4& value);

		/**
		*@brief Removes the first instance of value passed in
		*@param value - value to remove
		*@return true if element was removed (i.e at least one instance exists)
		**/
		bool Remove(const glm::mat4& value);

		/**
		*@brief Removes the first instance of value passed in
		*@param value - value to remove
		*@return true if element was removed (i.e at least one instance exists)
		**/
		bool Remove(const std::string& value);

		/**
		*@brief Removes the first instance of value passed in
		*@param value - value to remove
		*@return true if element was removed (i.e at least one instance exists)
		**/
		bool Remove(RTTI* const & value);

		/**
		*@brief Removes the first instance of value passed in
		*@param value - value to remove
		*@return true if element was removed (i.e at least one instance exists)
		**/
		bool Remove(Scope* const & value);

		/**
		*@brief Templated Get to return a const reference to an element
		*specialized for all types that datum stores
		*@param index (optional, defaults to 0) - index to get
		*@return const reference to data at index
		*@throws exception if templated call is made with wrong type
		*@throws exception if index is out of bounds
		**/
		template<typename T>
		const T& Get(uint32_t index = 0) const;

		/**
		*@brief Templated Get to return a reference to an element
		*specialized for all types that datum stores
		*@param index (optional, defaults to 0) - index to get
		*@return const reference to data at index
		*@throws exception if templated call is made with wrong type
		*@throws exception if index is out of bounds
		**/
		template<typename T>
		T& Get(uint32_t index = 0);

		/**
		*@brief Set value of element at  index from string
		*@param string - string to parse
		*@param index (optional, defaults to 0) - index to set
		*@throws exception if index is out of bounds
		*@throws exception if type is pointer to RTTI
		**/
		void SetFromString(const std::string& string, uint32_t index = 0);

		/**
		*@brief Get value of element at index as string
		*@param index (optional, defaults to 0) - index to get
		*@return value at index, as string
		*@throws exception if index is out of bounds
		**/
		std::string ToString(uint32_t index = 0) const;



		/**
		*@brief [] to return references to nested scopes
		*@param index - index to return the nested scope at
		*@return Scope reference at index
		**/
		Scope& operator[](uint32_t index);

		/**
		*@brief Returns true if the datum is external
		**/
		bool Datum::IsExternal() const;

	private:
		void EnforceType(DatumType type) const;
		void EnforceAndSetType(DatumType type);
		void EnforceIndex(uint32_t index) const;
		void EnforceInternal() const;
		void GuaranteeOneSpot();
		void SetExternalAndAssignSizeAndCapacity(uint32_t count);
		void HandleFirstAssignment();
		void ReservationStrategy();

		union DatumValues
		{
			void* any;
			int32_t* i;
			float* f;
			glm::vec4* v;
			glm::mat4* m;
			std::string* s;
			RTTI** p;
			Scope** t;
		public:
			DatumValues()
				:any(nullptr)
			{

			}
		};

		static const size_t TypeSize[];
		DatumValues mData;
		uint32_t mCapacity;
		uint32_t mSize;
		bool mIsExternal;
		DatumType mType;
	};
}
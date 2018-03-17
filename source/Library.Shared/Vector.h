#pragma once
#include "DefaultIncrement.h"

namespace FieaGameEngine
{
	/**
	*@brief generic Vector class that provides contiguous storage for a set of elements of type T;
	*allows for guaranteed O(1) access.
	**/
	template<typename T, typename Increment = DefaultIncrement>
	class Vector
	{
	public:
		/**
		*@brief Iterator class that provides iterators for the vector - contains an index and an owner
		*pointer
		**/
		class Iterator
		{
			friend class Vector;
		public:
			/**
			*@brief default constructor for iterator
			**/
			Iterator();

			/**
			*@brief parameterized constructor for iterator
			*@param index - unsigned int indicating location of the element
			*@param owner - Vector that the iterator belongs to/iterates in
			**/
			Iterator(uint32_t index, Vector& owner);

			/**
			*@brief default destructor for iterator
			**/
			~Iterator() = default;

			/**
			*@brief comparison operator for iterators
			*@param other - const reference to iterator being compared to
			*@return true if the iterators are identical (same owner, same index)
			**/
			bool operator==(const Iterator& other) const;

			/**
			*@brief NotEqual operator for iterators
			*@param other - const reference to iterator being compared to
			*@return false if the iterators are identical (same owner, same index)
			**/
			bool operator!=(const Iterator& other) const;

			/**
			*@brief pre-increment operator for iterators
			*@return - Reference to incremented self
			**/
			Iterator& operator++();

			/**
			*@brief post-increment operator for iterators
			*@return copy of self before increment
			**/
			Iterator operator++(int);

			/**
			*@brief dereference operator for iterators
			*@return reference to element being pointed at
			**/
			T& operator*() const;

			/**
			*@brief copy constructor for iterators
			*@param other - iterator being copied
			**/
			Iterator(const Iterator& other);

			/**
			*@brief copy assignment operator
			*@param other - iterator being copied
			*@return reference to self
			**/
			Iterator& operator=(const Iterator& other);

		private:
			uint32_t mIndex;
			Vector* mOwner;
		};

		/**
		*@brief default constructor for Vector
		*@param capacity - amount of memory (in element units) to reserve
		**/
		explicit Vector(uint32_t capacity = 3);

		/**
		*@brief index operator
		*@param index - index of element to access
		*@return reference to element at index
		**/
		T& operator[](uint32_t index);


		/**
		*@brief index operator
		*@param index - index of element to access
		*@return const reference to element at index
		**/
		const T& operator[](uint32_t index) const;

		/**
		*@brief Removes the element at the back of the vector (capacity is retained)
		**/
		void PopBack();

		/**
		*@brief check if vector is empty
		*@return true if vector is empty
		**/
		bool IsEmpty() const;

		/**
		*@brief returns a reference to the first element
		*@return reference to the first element
		**/
		T& Front();

		/**
		*@brief returns a const reference to the first element
		*@return const reference to the first element
		**/
		const T& Front() const;

		/**
		*@brief returns a reference to the last element
		*@return reference to the last element
		**/
		T& Back();

		/**
		*@brief returns a const reference to the last element
		*@return const reference to the last element
		**/
		const T& Back() const;

		/**
		*@brief returns the number of elements in the vector
		*@return number of elements
		**/
		uint32_t Size() const;

		/**
		*@brief returns the maximum number of elements that could be stored without reallocating memory
		*@return capacity
		**/
		uint32_t Capacity() const;

		/**
		*@brief returns an iterator pointing to the first element
		*@return iterator pointing to first element
		**/
		Iterator begin() const;

		/**
		*@brief returns an iterator pointing to the element after last
		*@return iterator pointing to the element after last
		**/
		Iterator end() const;

		/**
		*@brief insert a new element at the end of the vector, relocating in memory if necessary
		*@param value - element to be inserted
		*@return iterator pointing to the inserted node
		**/
		Iterator PushBack(const T& value);

		/**
		*@brief copy constructor for vector
		*@param other - vector to be copied
		**/
		Vector(const Vector& other);

		/**
		*@brief move constructor for vector
		*@param other - vector to be moved
		**/
		Vector(Vector&& other);

		/**
		*@brief copy assignment operator for vector
		*@param other - Vector to be copied
		*@return reference to self
		**/
		Vector& operator=(const Vector& other);

		/**
		*@brief move assignment operator for vector
		*@param other - Vector to be moved
		*@return reference to self
		**/
		Vector& operator=(Vector&& other);

		/**
		*@brief Returns the element at the given index
		*@param index - index of element
		*@return reference to element at index
		**/
		T& At(uint32_t index);

		/**
		*@brief Returns the element at the given index
		*@param index - index of element
		*@return reference to element at index
		**/
		const T& At(uint32_t index) const;

		/**
		*@brief destructor for vector - frees allocated memory
		**/
		~Vector();

		/**
		*@brief reserves memory for 'capacity' number of elements and moves the previously existing
		*elements into this location
		*@param capacity - amount of memory to reserve in sizeof(element) units
		**/
		void Reserve(uint32_t capacity);

		/**
		*@brief finds an element in the vector if it exists
		*@param value - element to find
		*@return iterator pointing to the location of element (or end if not found)
		**/
		Iterator Find(const T& value) const;

		/**
		*@brief clears the vector of elements (capacity unaffected)
		**/
		void Clear();

		/**
		*@brief resizes the vector
		*@param size - size to size the vector to - delete things if smaller than old size,
		*default construct things if larger than old size
		**/
		void Resize(uint32_t size);

		/**
		*@brief removes the first element with the given value
		*@param value - element to remove
		*@return true if element was found and removed
		**/
		bool Remove(const T& value);

		/**
		*@brief removes the first element with the given value
		*@param position - iterator pointing to element to be removed
		*@return true if element was found and removed
		**/
		bool Remove(Iterator position);

		/**
		*@brief removes the elements in the given range
		*@param first - start of removed elements
		*@param last - end of removed elements
		*@return true if elements were found and removed
		**/
		bool Remove(Iterator first, Iterator last);

		/**
		*@brief shrinks the capacity down to size
		**/
		void Shrink();
	private:
		T * mData;
		uint32_t mCapacity;
		uint32_t mSize;
		Increment mIncrementor;
	};
}

#include "Vector.inl"
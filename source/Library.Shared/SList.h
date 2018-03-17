#pragma once

namespace FieaGameEngine
{
	/**
	*Templated singly linked list.
	**/
	template<typename T>
	class SList
	{
	private:
		class Node
		{
		public:
			Node();
			Node(const T& value, Node* next = nullptr);
			T mValue;
			Node* mNext;
		};

	public:
		/**
		*Iterator class to iterate through the SList.
		**/
		class Iterator
		{
			friend class SList;
		public:
			/**
			*@brief Default constructor for SList iterator.
			**/
			Iterator();

			/**
			*@brief Destructor for SList iterator.
			**/
			~Iterator() = default;

			/**
			*@brief pre-increment operator for SList iterator. Makes iterator point to
			*next item in SList first before executing the statement.
			*@return iterator reference after incrementing.
			**/
			Iterator& operator++();

			/**
			*@brief post-increment operator for SList iterator. Makes iterator point to
			*next item in SList after executing the statement.
			*@param dummy int param for distinguishing signature.
			*@return Iterator object without incrementing.
			**/
			Iterator operator++(int);

			/**
			*@brief equality comparison operator for SList iterators.
			*@param other - iterator to compare with.
			*@return boolean which is true if the iterators are equal.
			**/
			bool operator==(const Iterator& other) const;

			/**
			*@brief inequality comparison operator for SList iterators.
			*@param other - iterator to compare with.
			*@return boolean which is true if the iterators are not equal.
			**/
			bool operator!=(const Iterator& other) const;

			/**
			*@brief dereference operator for SList iterators.
			*@return Reference to templated object that the node being pointed to by
			*the iterator contains.
			*@throws standard exception if end is being pointed to.
			**/
			T& operator*();

			/**
			*@brief dereference operator for const SList iterators.
			*@return const reference to templated object that the node being pointed to by
			*the iterator contains.
			*@throws standard exception if end is being pointed to.
			**/
			const T& operator*() const;

		private:
			Node * mNode;
			const SList* mOwner;

			/**
			*@brief Parameterized constructor for SList iterator.
			*@param node - Node to point to.
			*@param owner - Reference to SList that owns the iterator.
			**/
			Iterator(Node* node, const SList& owner);
		};

		/**
		*@brief Default constructor for SList. Creates an empty list.
		**/
		SList();

		/**
		*@brief SList destructor performs teardown by calling Clear.
		**/
		~SList();

		/**
		*@brief SList Copy constructor.
		*@param other - SList to be copied.
		**/
		SList(const SList<T>& other);

		/**
		*@brief SList Move constructor.
		*@param other - SList to be moved.
		**/
		SList(SList<T>&& other);

		/**
		*@brief SList copy assignment operator.
		*@param other - SList to be copied from.
		**/
		SList<T>& operator=(const SList<T>& other);

		/**
		*@brief SList move assignment operator.
		*@param other - SList to be moved from.
		**/
		SList<T>& operator=(SList<T>&& other);

		/**
		*@brief PushBack appends a new element to the list at the back.
		*@param value - This is the value of type T that needs
		*to be pushed in.
		*@return Iterator to back.
		**/
		Iterator PushBack(const T& value);

		/**
		*@brief PushFront appends a new element to the list at the front.
		*@param value - This is the value of type T that needs
		*to be pushed in.
		*@return Iterator to front.
		**/
		Iterator PushFront(const T& value);

		/**
		*@brief PopFront removes an element from the front of the list.
		*@throws standard exception if list is empty.
		**/
		void PopFront();

		/**
		*@brief PopBack removes an element from the back of the list.
		*@throws standard exception if list is empty.
		**/
		void PopBack();

		/**
		*@brief IsEmpty returns a boolean indicating whether the list is empty.
		*@return bool indicating whether list is empty.
		**/
		bool IsEmpty() const;

		/**
		*@brief Front returns a reference to the element at the front of the list.
		*@return reference to element at the front of the list.
		*@throws standard exception if list is empty.
		**/
		T& Front();

		/**
		*@brief Back returns a reference to the element at the back of the list.
		*@return reference to element at the back of the list.
		*@throws standard exception if list is empty.
		**/
		T& Back();

		/**
		*@brief Front const returns a const reference to the element at the front of the list.
		*@return const reference to element at the front of the list.
		*@throws standard exception if list is empty.
		**/
		const T& Front() const;

		/**
		*@brief Back const returns a const reference to the element at the back of the list.
		*@return const reference to element at the back of the list.
		*@throws standard exception if list is empty.
		**/
		const T& Back() const;

		/**
		*@brief Size returns the number of elements in the list (i.e. the size).
		*@return size of the list.
		**/
		uint32_t Size() const;

		/**
		*@brief Clear removes all the elements in the list.
		**/
		void Clear();

		/**
		*@brief Returns an iterator pointing to the beginning of the list.
		*@return iterator pointing to the beginning of the list.
		**/
		Iterator begin() const;

		/**
		*@brief Returns an iterator pointing to the end of the list.
		*@return iterator pointing to the end of the list.
		**/
		Iterator end() const;

		/**
		*@brief Appends the given value after the position specified by the iterator.
		*@param iterator - const Iterator reference pointing to the node to insert the element after.
		*@param value - Value of the node to be inserted as a const reference.
		**/
		void InsertAfter(const Iterator& iterator, const T& value);

		/**
		*@brief Finds an item in the list and returns an iterator pointing to its node
		*(first match).
		*@param value - item to find.
		*@return iterator pointing to the item's node.
		**/
		Iterator Find(const T& value) const;

		/**
		*@brief Removes an item from the list (first match).
		*@param value - The value of the item to be removed.
		*@return true if element was found.
		**/
		bool Remove(const T& value);

		/**
		*@brief Removes an item from the list (first match).
		*@param value - The value of the item to be removed.
		*@return true if element was found.
		**/
		bool Remove(Iterator position);

	private:
		Node * mFront;
		Node* mBack;
		uint32_t mSize;
	};
}

#include "SList.inl"
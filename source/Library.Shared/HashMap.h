#pragma once

namespace FieaGameEngine
{
	/**
	*@brief Generic hash functor for given value
	*@param
	**/
	template<typename T>
	class DefaultHash
	{
	public:
		/**
		*@brief Generic hash for given key
		*@param key - const reference to the data to be hashed
		*@return unsigned 32 bit hash value
		**/
		uint32_t operator()(const T& key) const;
	};

	/**
	*@brief Generic compare functor for given value
	*@param
	**/
	template<typename T>
	class DefaultCompare
	{
	public:
		/**
		*@brief Generic compare functor for given value
		*@param lhs - Left hand side of comparison
		*@param rhs - right hand side of comparison
		*@return bool indicating if they're equal
		**/
		bool operator()(const T& lhs, const T& rhs) const;
	};

	template<>
	class DefaultHash<char*>
	{
	public:
		uint32_t operator()(const char* key) const;
	};

	template<>
	class DefaultHash<std::string>
	{
	public:
		uint32_t operator()(const std::string& key) const;
	};

	template<>
	class DefaultCompare<char*>
	{
	public:
		bool operator()(const char* lhs, const char* rhs) const;
	};

	uint32_t BernsteinHash(const uint8_t* dataPtr, size_t size);

	/**
	@brief Generic hashMap class that stores key-value pairs and provides look-ups based on keys.
	*The keys are hashed to a unique value (i.e a key can have only one hash - but multiple keys may hash to the same hash value)
	*in order to decide where among the N buckets in the map to store the pair.
	*Insertions and look-ups are fast (~O[1]), depending on the provided hash functor.
	**/
	template<typename TKey, typename TData, typename HashFunctor = DefaultHash<TKey>, typename CompareFunctor = DefaultCompare<TKey>>
	class HashMap final
	{
	public:
		typedef std::pair<TKey, TData> PairType;
		typedef SList<PairType> ChainType;
		typedef Vector<ChainType> BucketType;
		/**
		*@brief Iterator to go through the hash-map element by element
		**/
		class Iterator final
		{
			friend class HashMap;
		public:
			/**
			*@brief Default constructor for iterator
			**/
			Iterator();
			Iterator(const Iterator& other) = default;
			Iterator& operator=(const Iterator& other) = default;
			~Iterator() = default;
			/**
			*@brief Pre-increment operator
			*@return reference to iterator after incrementing
			*@throws std::exception if iterator doesn't have owner
			**/
			Iterator& operator++();
			/**
			*@brief Post-increment operator, increments but returns old value
			*@return Iterator
			*@throws std::exception if iterator doesn't have owner
			**/
			Iterator operator++(int);
			/**
			*@brief Dereference operator
			*@return reference to a pair
			*@throws std::exception if iterator doesn't have owner
			**/
			PairType& operator*();
			/**
			*@brief Arrow operator
			*@return pointer to a pair
			*@throws std::exception if iterator doesn't have owner
			**/
			PairType* operator->();
			/**
			*@brief const Dereference operator
			*@return const reference to a pair
			*@throws std::exception if iterator doesn't have owner
			**/
			const PairType& operator*() const;
			/**
			*@brief const Arrow operator
			*@return const pointer to a pair
			*@throws std::exception if iterator doesn't have owner
			**/
			const PairType* operator->() const;
			/**
			*@brief Comparison operator
			*@param other - Iterator to compare with
			*@return true if they're equal
			**/
			inline bool operator==(const Iterator& other) const;
			/**
			*@brief NotEqual operator
			*@param other - Iterator to compare with
			*@return false if they're equal
			**/
			inline bool operator!=(const Iterator& other) const;
		private:
			Iterator(typename ChainType::Iterator chainIterator, uint32_t bucketIndex, const HashMap& owner);
			typename ChainType::Iterator mChainIterator;
			uint32_t mBucketIndex;
			const HashMap* mOwner;
		};
		/**
		*@brief Default hashmap constructor
		*@param size - number of buckets in the hash map
		*@return true if they're equal
		*@throws std::exception if size is 0
		**/
		explicit HashMap(uint32_t size = 11);
		HashMap(const std::initializer_list<PairType>& list);
		~HashMap() = default;
		HashMap(const HashMap& other) = default;
		HashMap& operator=(const HashMap& other) = default;
		HashMap(HashMap&& other);
		HashMap& operator=(HashMap&& other);
		/**
		*@brief Finds a key-value pair based on the key
		*@param key - const reference to key to be found
		*@return Iterator to key value pair if found, end if not found
		**/
		Iterator Find(const TKey& key) const;
		/**
		*@brief Finds a key-value pair based on the key
		*@param key - const reference to key to be found
		*@return Iterator to key value pair if found, end if not found
		**/
		Iterator Insert(const PairType& entry);
		/**
		*@brief Finds a key-value pair based on the key
		*@param key - const reference to key to be found
		*@param valueInserted - returns a bool by reference indicating if value was inserted
		*@return Iterator to key value pair if found, end if not found
		**/
		Iterator Insert(const PairType& entry, bool& valueInserted);
		/**
		*@brief Index operator - returns a reference to the data associated with the key (creates a pair if it doesn't exist)
		*@param key - const reference to key to return the Data to
		*@return reference to the data associated with the key
		**/
		TData& operator[](const TKey& key);
		/**
		*@brief returns a reference to the data associated with the key
		*@param key - const reference to key to return the Data to
		*@return reference to the data associated with the key
		*@throws exception if key wasn't found
		**/
		TData& At(const TKey& key);
		/**
		*@brief returns a const reference to the data associated with the key
		*@param key - const reference to key to return the Data to
		*@return const reference to the data associated with the key
		*@throws exception if key wasn't found
		**/
		const TData& At(const TKey& key) const;
		/**
		*@brief Removes the element associated with the key if it was found
		*@param key - const reference to key whose pair to delete
		*@return whether the pair was found (and hence deleted)
		**/
		bool Remove(const TKey& key);
		/**
		*@brief Removes all elements from the map
		**/
		void Clear();
		/**
		*@brief Returns the number of elements in the map
		*@return the number of elements in the map
		**/
		uint32_t Size() const;

		/**
		*@brief Returns a bool indicating if the map contains the key
		*@param key - const reference to key to test for
		*@return whether the key was found
		**/
		bool ContainsKey(const TKey& key) const;

		/**
		*@brief Returns a bool indicating if the map contains the key
		*@param key - const reference to key to test for
		*@param out outIt - reference with which to return an iterator
		*to the key-value pair if it does exist already
		*@return whether the key was found
		**/
		bool ContainsKey(const TKey& key, Iterator& outIt) const;

		/**
		*@brief returns iterator to the first element in the map
		*@return iterator to the first element in the map
		**/
		Iterator begin();
		/**
		*@brief returns const iterator to the first element in the map
		*@return const iterator to the first element in the map
		**/
		const Iterator begin() const;
		/**
		*@brief returns iterator meaning not-in-map
		*@return iterator meaning not-in-map
		**/
		Iterator end();
		/**
		*@brief returns const iterator meaning not-in-map
		*@return const iterator meaning not-in-map
		**/
		const Iterator end() const;
	private:
		Iterator Find(const TKey& key, uint32_t& hash) const;
		BucketType mBuckets;
		uint32_t mSize;
		HashFunctor mFunctor;
		CompareFunctor mCompare;
	};
}

#include "HashMap.inl"
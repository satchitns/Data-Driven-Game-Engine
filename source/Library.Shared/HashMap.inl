#pragma once

namespace FieaGameEngine
{
#pragma region Map
	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	HashMap<TKey, TData, HashFunctor, CompareFunctor>::HashMap(uint32_t size)
		:mSize(0), mBuckets(0)
	{
		if (size < 1)
		{
			throw std::exception("Invalid size");
		}
		mBuckets.Resize(size);
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	HashMap<TKey, TData, HashFunctor, CompareFunctor>::HashMap(const std::initializer_list<PairType>& list)
		:HashMap(static_cast<uint32_t>(list.size()))
	{
		for (const auto &value : list)
		{
			Insert(value);
		}
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	typename HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator HashMap<TKey, TData, HashFunctor, CompareFunctor>::Insert(const PairType& entry)
	{
		bool dummy;
		return Insert(entry, dummy);
	}
	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	typename HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator HashMap<TKey, TData, HashFunctor, CompareFunctor>::Insert(const PairType& entry, bool& valueInserted)
	{
		uint32_t bucketNum;
		Iterator searchResults = Find(entry.first, bucketNum);
		if (searchResults == end())
		{
			ChainType::Iterator it = mBuckets[bucketNum].PushBack(entry);
			++mSize;
			valueInserted = true;
			return Iterator(it, bucketNum, *this);
		}
		else
		{
			valueInserted = false;
			return searchResults;
		}
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	typename HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator HashMap<TKey, TData, HashFunctor, CompareFunctor>::Find(const TKey& key) const
	{
		uint32_t dummyNum;
		return Find(key, dummyNum);
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	typename HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator HashMap<TKey, TData, HashFunctor, CompareFunctor>::Find(const TKey& key, uint32_t& hash) const
	{
		hash = mFunctor(key) % mBuckets.Size();
		const ChainType* keyChain = &mBuckets[hash];
		for (auto it = keyChain->begin(); it != keyChain->end(); ++it)
		{
			if (mCompare(key, (*it).first))
			{
				return Iterator(it, hash, *this);
			}
		}
		return end();
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	typename HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator HashMap<TKey, TData, HashFunctor, CompareFunctor>::begin()
	{
		return (const_cast<const HashMap<TKey, TData, HashFunctor, CompareFunctor>&>(*this)).begin();
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	const typename HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator HashMap<TKey, TData, HashFunctor, CompareFunctor>::begin() const
	{
		uint32_t startIndex = 0;
		while (startIndex < mBuckets.Size() && mBuckets[startIndex].Size() == 0)
		{
			++startIndex;
		}
		if (startIndex == mBuckets.Size())
		{
			return end();
		}
		return Iterator(mBuckets[startIndex].begin(), startIndex, *this);
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	typename HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator HashMap<TKey, TData, HashFunctor, CompareFunctor>::end()
	{
		return (const_cast<const HashMap<TKey, TData, HashFunctor, CompareFunctor>&>(*this)).end();
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	const typename HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator HashMap<TKey, TData, HashFunctor, CompareFunctor>::end() const
	{
		return Iterator(mBuckets[mBuckets.Size() - 1].end(), mBuckets.Size(), *this);
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	TData& HashMap<TKey, TData, HashFunctor, CompareFunctor>::operator[](const TKey& key)
	{

		return Insert(PairType(key, TData()))->second;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	TData& HashMap<TKey, TData, HashFunctor, CompareFunctor>::At(const TKey& key)
	{
		return const_cast<TData&>(const_cast<const HashMap<TKey, TData, HashFunctor, CompareFunctor>&>(*this).At(key));
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	const TData& HashMap<TKey, TData, HashFunctor, CompareFunctor>::At(const TKey& key) const
	{
		Iterator it;
		if (!ContainsKey(key, it))
		{
			throw std::exception("Key not found");
		}
		return it->second;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	bool HashMap<TKey, TData, HashFunctor, CompareFunctor>::Remove(const TKey& key)
	{
		auto it = Find(key);
		if (it == end())
		{
			return false;
		}
		bool removed = mBuckets[it.mBucketIndex].Remove(it.mChainIterator);
		if (removed)
		{
			--mSize;
		}
		return removed;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	void HashMap<TKey, TData, HashFunctor, CompareFunctor>::Clear()
	{
		for (uint32_t i = 0; i < mBuckets.Size(); ++i)
		{
			mBuckets[i].Clear();
			mSize = 0;
		}
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	uint32_t  HashMap<TKey, TData, HashFunctor, CompareFunctor>::Size() const
	{
		return mSize;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	HashMap<TKey, TData, HashFunctor, CompareFunctor>::HashMap(HashMap&& other)
		:mBuckets(std::move(other.mBuckets)), mSize(other.mSize), mFunctor(other.mFunctor), mCompare(other.mCompare)
	{
		other.mSize = 0;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	HashMap<TKey, TData, HashFunctor, CompareFunctor>& HashMap<TKey, TData, HashFunctor, CompareFunctor>::operator=(HashMap&& other)
	{
		if (this != &other)
		{
			mBuckets = std::move(other.mBuckets);
			mSize = other.mSize;
			mFunctor = other.mFunctor;
			mCompare = other.mCompare;
			other.mSize = 0;
		}
		return *this;
	}
#pragma endregion

#pragma region Iterator

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator::Iterator() :
		mChainIterator(ChainType::Iterator()), mBucketIndex(0), mOwner(nullptr)
	{

	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator::Iterator(typename ChainType::Iterator chainIterator, uint32_t bucketIndex, const HashMap& owner)
		:mChainIterator(chainIterator), mBucketIndex(bucketIndex), mOwner(&owner)
	{

	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	bool HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator::operator==(const Iterator& other) const
	{
		return((mChainIterator == other.mChainIterator) && (mBucketIndex == other.mBucketIndex) && (mOwner == other.mOwner));
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	bool HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator::operator!=(const Iterator& other) const
	{
		return !(operator==(other));
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	typename HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator::operator++(int)
	{
		auto temp = *this;
		operator++();
		return temp;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	typename HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator& HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Owner not defined");
		}
		if (*this != mOwner->end())
		{
			++mChainIterator;
			if (mChainIterator == mOwner->mBuckets[mBucketIndex].end())	//if we reached the end of that Chain, find the next non-empty chain
			{
				bool found = false;
				++mBucketIndex; //we need at least the next chain
				for (; mBucketIndex < mOwner->mBuckets.Size(); ++mBucketIndex)
				{
					if (mOwner->mBuckets[mBucketIndex].Size() > 0)
					{
						found = true;
						break;
					}
				}
				if (found == false) //if there are no more non-empty chains, we've reached the end
				{
					*this = mOwner->end();
				}
				else //if not, return the first element of the next non-empty chain
				{
					mChainIterator = mOwner->mBuckets[mBucketIndex].begin();
				}
			}
		}
		return *this;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	typename HashMap<TKey, TData, HashFunctor, CompareFunctor>::PairType& HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator::operator*()
	{
		return const_cast<PairType&>((const_cast<const HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator&>(*this)).operator*());
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	const typename HashMap<TKey, TData, HashFunctor, CompareFunctor>::PairType& HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator::operator*() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Owner not defined");
		}
		return *mChainIterator;
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	typename HashMap<TKey, TData, HashFunctor, CompareFunctor>::PairType* HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator::operator->()
	{
		return const_cast<PairType*>((const_cast<const HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator&>(*this)).operator->());
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	const typename HashMap<TKey, TData, HashFunctor, CompareFunctor>::PairType* HashMap<TKey, TData, HashFunctor, CompareFunctor>::Iterator::operator->() const
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Owner not defined");
		}
		return &(*mChainIterator);
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	bool HashMap<TKey, TData, HashFunctor, CompareFunctor>::ContainsKey(const TKey& key) const
	{
		Iterator it;
		return ContainsKey(key, it);
	}

	template<typename TKey, typename TData, typename HashFunctor, typename CompareFunctor>
	bool HashMap<TKey, TData, HashFunctor, CompareFunctor>::ContainsKey(const TKey& key, Iterator& outIt) const
	{
		auto it = Find(key);
		if (it == end())
		{
			return false;
		}
		outIt = it;
		return true;
	}
#pragma endregion

	template<typename T>
	uint32_t DefaultHash<T>::operator()(const T& key) const
	{

		const uint8_t* data = reinterpret_cast<const uint8_t*>(&key);
		return BernsteinHash(data, sizeof(key));
	}

	template<typename T>
	bool DefaultCompare<T>::operator()(const T& lhs, const T& rhs) const
	{
		return (lhs == rhs);
	}
}
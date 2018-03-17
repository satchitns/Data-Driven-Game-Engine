#pragma once


namespace FieaGameEngine
{
#pragma region SList methods
	template<typename T>
	SList<T>::SList()
		:mFront(nullptr), mBack(nullptr), mSize(0)
	{

	}

	template<typename T>
	SList<T>::~SList()
	{
		Clear();
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::PushFront(const T& value)
	{
		mFront = new Node(value, mFront);
		if (mBack == nullptr) //list was empty before
		{
			mBack = mFront;
		}
		mSize++;
		return Iterator(mFront, *this);
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::PushBack(const T& value)
	{
		Node* rearNode = new Node(value);
		if (mBack == nullptr) //list was empty before
		{
			mFront = rearNode;
		}
		else
		{
			mBack->mNext = rearNode;
		}
		mBack = rearNode;
		mSize++;
		return Iterator(rearNode, *this);
	}

	template<typename T>
	void SList<T>::PopFront()
	{
		if (IsEmpty())
		{
			throw std::exception("List is empty\n");
		}
		Node* temp = mFront;
		mFront = mFront->mNext;
		if (mFront == nullptr) //list is now empty
		{
			mBack = nullptr;
		}
		delete temp;
		--mSize;
	}

	template<typename T>
	void SList<T>::PopBack()
	{
		if (IsEmpty())
		{
			throw std::exception("List is empty\n");
		}
		auto it = begin();
		if (it.mNode->mNext == nullptr) //one node case
		{
			delete it.mNode;
			mFront = nullptr;
			mBack = nullptr;
		}
		else //multi-node case
		{
			while (it.mNode->mNext->mNext != nullptr)
			{
				++it;
			}
			Node* temp = it.mNode->mNext;
			it.mNode->mNext = nullptr;
			delete temp;
			mBack = it.mNode;
		}
		--mSize;
	}

	template<typename T>
	bool SList<T>::IsEmpty() const
	{
		if (mFront == nullptr)
		{
			return true;
		}
		return false;
	}

	template<typename T>
	const T& SList<T>::Front() const
	{
		if (IsEmpty())
		{
			throw std::exception("List is empty\n");
		}
		return mFront->mValue;
	}

	template<typename T>
	const T& SList<T>::Back() const
	{
		if (IsEmpty())
		{
			throw std::exception("List is empty\n");
		}
		return mBack->mValue;
	}

	template<typename T>
	T& SList<T>::Front()
	{
		if (IsEmpty())
		{
			throw std::exception("List is empty\n");
		}
		return mFront->mValue;
	}

	template<typename T>
	T& SList<T>::Back()
	{
		if (IsEmpty())
		{
			throw std::exception("List is empty\n");
		}
		return mBack->mValue;
	}

	template<typename T>
	uint32_t SList<T>::Size() const
	{
		return mSize;
	}

	template<typename T>
	SList<T>::SList(const SList& other)
		: mSize(0), mFront(nullptr), mBack(nullptr)
	{
		for (auto &value : other)
		{
			PushBack(value);
		}
	}

	template<typename T>
	SList<T>::SList(SList&& other)
		: mSize(other.mSize), mFront(other.mFront), mBack(other.mBack)
	{
		other.mFront = other.mBack = nullptr;
		other.mSize = 0;
	}

	template<typename T>
	SList<T>& SList<T>::operator=(const SList& other)
	{
		if (this != &other)
		{
			Clear();
			for (auto &value : other)
			{
				PushBack(value);
			}
		}
		return *this;
	}

	template<typename T>
	SList<T>& SList<T>::operator=(SList&& other)
	{
		if (this != &other)
		{
			Clear();
			mFront = other.mFront;
			mBack = other.mBack;
			mSize = other.mSize;
			other.mFront = other.mBack = nullptr;
			other.mSize = 0;
		}
		return *this;
	}


	template<typename T>
	void SList<T>::Clear()
	{
		Node* temp = nullptr;
		while (mFront != nullptr)
		{
			temp = mFront;
			mFront = mFront->mNext;
			delete temp;
		}
		mSize = 0;
		mFront = mBack = nullptr;
	}
#pragma endregion

#pragma region Node methods
	template<typename T>
	SList<T>::Node::Node()
		:mNext(nullptr)
	{

	}

	template<typename T>
	SList<T>::Node::Node(const T& value, Node* next = nullptr)
		: mValue(value), mNext(next)
	{

	}
#pragma endregion

#pragma region Iterator methods
	template<typename T>
	SList<T>::Iterator::Iterator()
		:mNode(nullptr), mOwner(nullptr)
	{

	}

	template<typename T>
	SList<T>::Iterator::Iterator(Node* node, const SList& owner)
		:mNode(node), mOwner(&owner)
	{

	}

	template<typename T>
	typename SList<T>::Iterator& SList<T>::Iterator::operator++()
	{
		if (mNode == nullptr)
		{
			throw std::exception("Iterator reached end of list, cannot be incremented.");
		}
		mNode = mNode->mNext;
		return *this;
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::Iterator::operator++(int)
	{
		if (mNode == nullptr)
		{
			throw std::exception("Iterator reached end of list, cannot be incremented.");
		}
		Iterator temp = *this;
		mNode = mNode->mNext;
		return temp;
	}

	template<typename T>
	inline bool SList<T>::Iterator::operator==(const Iterator& other) const
	{
		return ((mOwner == other.mOwner) && (mNode == other.mNode));
	}

	template<typename T>
	inline bool SList<T>::Iterator::operator!=(const Iterator& other) const
	{

		return !(operator==(other));
	}

	template<typename T>
	T& 	SList<T>::Iterator::operator*()
	{
		if (mNode == nullptr)
		{
			throw std::exception("Not a valid element.");
		}
		return mNode->mValue;
	}

	template<typename T>
	const T& SList<T>::Iterator::operator*() const
	{
		if (mNode == nullptr)
		{
			throw std::exception("Not a valid element.");
		}
		return mNode->mValue;
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::begin() const
	{
		return Iterator(mFront, *this);
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::end() const
	{
		return Iterator(nullptr, *this);
	}

	template<typename T>
	void SList<T>::InsertAfter(const Iterator& iterator, const T& value)
	{
		if (iterator.mOwner != this)
		{
			throw std::exception("Iterator not owned by list.");
		}
		if ((iterator.mNode == nullptr) || (iterator.mNode == mBack))
		{
			PushBack(value);
		}
		else
		{
			Node* newNode = new Node(value, iterator.mNode->mNext);
			iterator.mNode->mNext = newNode;
			mSize++;
		}
	}

	template<typename T>
	typename SList<T>::Iterator SList<T>::Find(const T& value) const
	{
		Iterator it = begin();
		for (; it != end(); ++it)
		{
			if (*it == value)
			{
				break;
			}
		}
		return it;
	}

	template<typename T>
	bool SList<T>::Remove(const T& value)
	{
		Iterator previousNode(nullptr, *this), it;
		for (it = begin(); it != end(); ++it)
		{
			if (*it == value)
			{
				Node* temp = it.mNode;
				if (previousNode.mNode == nullptr) //first element is being removed
				{
					mFront = mFront->mNext;
				}
				else
				{
					previousNode.mNode->mNext = it.mNode->mNext;
					if (it.mNode->mNext == nullptr) //last node  is being removed
					{
						mBack = previousNode.mNode;
					}
				}
				delete temp;
				mSize--;
				return true;
			}
			previousNode = it;
		}
		return false;
	}

	template<typename T>
	bool SList<T>::Remove(Iterator position)
	{
		if (position.mOwner != this)
		{
			throw std::exception("Wrong owner");
		}

		if (position == end())
		{
			return false;
		}

		if (position.mNode->mNext == nullptr)
		{
			PopBack();
		}
		else
		{
			position.mNode->mValue = std::move(position.mNode->mNext->mValue);
			Node* temp = position.mNode->mNext->mNext;
			delete position.mNode->mNext;
			position.mNode->mNext = temp;
			if (position.mNode->mNext == nullptr)
			{
				mBack = position.mNode;
			}
			--mSize;
		}
		return true;
	}
#pragma endregion
}
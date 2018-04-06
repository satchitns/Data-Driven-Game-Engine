#pragma once
#include "Vector.h" 
#include <cassert>

namespace FieaGameEngine
{
#pragma region Vector
	template<typename T, typename Increment>
	Vector<T, Increment>::Vector(uint32_t capacity)
		:mSize(0), mData(nullptr), mCapacity(0)
	{
		if (capacity > 0)
		{
			Reserve(capacity);
			mCapacity = capacity;
		}
	}

	template<typename T, typename Increment>
	inline Vector<T, Increment>::Vector(const std::initializer_list<T>& list)
		:Vector(static_cast<uint32_t>(list.size()))
	{
		for (const auto &value : list)
		{
			PushBack(value);
		}
	}

	template<typename T, typename Increment>
	Vector<T, Increment>::~Vector()
	{
		Clear();
		free(mData);
	}

	template<typename T, typename Increment>
	uint32_t Vector<T, Increment>::Size() const
	{
		return mSize;
	}

	template<typename T, typename Increment>
	uint32_t Vector<T, Increment>::Capacity() const
	{
		return mCapacity;
	}

	template<typename T, typename Increment>
	typename Vector<T, Increment>::Iterator Vector<T, Increment>::PushBack(const T& value)
	{
		Reserve(std::_Max_value(1U, mIncrementor(mSize, mCapacity)));
		new (mData + mSize)T(value);
		return Iterator(mSize++, *this);
	}

	template<typename T, typename Increment>
	void Vector<T, Increment>::Reserve(uint32_t capacity)
	{
		if (capacity > mCapacity)
		{
			T* newMem = reinterpret_cast<T*>(malloc(sizeof(T) * capacity));
			uint32_t numBytesToCopy = mSize * sizeof(T);
			memcpy_s(newMem, numBytesToCopy, mData, numBytesToCopy);
			free(mData);
			mData = newMem;
			mCapacity = capacity;
		}
	}

	template<typename T, typename Increment>
	typename Vector<T, Increment>::Iterator Vector<T, Increment>::begin() const
	{
		return Iterator(0, const_cast<Vector<T, Increment>&>(*this));
	}

	template<typename T, typename Increment>
	typename Vector<T, Increment>::Iterator Vector<T, Increment>::end() const
	{
		return Iterator(mSize, const_cast<Vector<T, Increment>&>(*this));
	}

	template<typename T, typename Increment>
	T& Vector<T, Increment>::operator[](uint32_t index)
	{
		if (index >= mSize)
		{
			throw std::out_of_range("Array index out of bounds\n");
		}
		return mData[index];
	}

	template<typename T, typename Increment>
	const T& Vector<T, Increment>::operator[](uint32_t index) const
	{
		if (index >= mSize)
		{
			throw std::out_of_range("Array index out of bounds\n");
		}
		return mData[index];
	}

	template<typename T, typename Increment>
	bool Vector<T, Increment>::IsEmpty() const
	{
		return (mSize == 0);
	}

	template<typename T, typename Increment>
	T& Vector<T, Increment>::At(uint32_t index)
	{
		return operator[](index);
	}

	template<typename T, typename Increment>
	const T& Vector<T, Increment>::At(uint32_t index) const
	{
		return operator[](index);
	}

	template<typename T, typename Increment>
	void Vector<T, Increment>::PopBack()
	{
		if (mSize == 0)
		{
			throw std::exception("Vector is empty\n");
		}
		mData[--mSize].~T();
	}

	template<typename T, typename Increment>
	T& Vector<T, Increment>::Front()
	{
		if (IsEmpty())
		{
			throw std::exception("Empty vector");
		}
		return mData[0];
	}

	template<typename T, typename Increment>
	const T& Vector<T, Increment>::Front() const
	{
		if (IsEmpty())
		{
			throw std::exception("Empty vector");
		}
		return mData[0];
	}

	template<typename T, typename Increment>
	T& Vector<T, Increment>::Back()
	{
		if (IsEmpty())
		{
			throw std::exception("Empty vector");
		}
		return mData[(mSize - 1)];
	}

	template<typename T, typename Increment>
	const T& Vector<T, Increment>::Back() const
	{
		if (IsEmpty())
		{
			throw std::exception("Empty vector");
		}
		return mData[(mSize - 1)];
	}

	template<typename T, typename Increment>
	typename Vector<T, Increment>::Iterator Vector<T, Increment>::Insert(const T & value, uint32_t index)
	{
		assert(index <= mSize);
		if (index == mSize)
		{
			return PushBack(value);
		}
		Reserve(std::_Max_value(1U, mIncrementor(mSize, mCapacity)));
		uint32_t numBytesToShift = (mSize - index) * sizeof(T);
		memmove_s(&mData[index + 1], numBytesToShift, &mData[index], numBytesToShift);
		new (mData + index)T(value);
		++mSize;
		return Iterator(index, *this);
	}

	template<typename T, typename Increment>
	Vector<T, Increment>::Vector(const Vector& other)
		:Vector(other.mCapacity)
	{
		for (auto &value : other)
		{
			PushBack(value);
		}
	}

	template<typename T, typename Increment>
	Vector<T, Increment>::Vector(Vector&& other)
		:mCapacity(other.mCapacity), mData(other.mData), mSize(other.mSize), mIncrementor(other.mIncrementor)
	{
		other.mCapacity = other.mSize = 0;
		other.mData = nullptr;
	}

	template<typename T, typename Increment>
	Vector<T, Increment>& Vector<T, Increment>::operator=(const Vector& other)
	{
		if (this != &other)
		{
			Clear();
			Shrink();
			Reserve(other.mCapacity); //doesn't do anything if mCapacity >= other.mCapacity
			for (auto &value : other)
			{
				PushBack(value);
			}
		}
		return *this;
	}

	template<typename T, typename Increment>
	Vector<T, Increment>& Vector<T, Increment>::operator=(Vector&& other)
	{
		if (this != &other)
		{
			Clear();
			free(mData);
			mData = other.mData;
			mCapacity = other.mCapacity;
			mSize = other.mSize;
			mIncrementor = other.mIncrementor;
			other.mData = nullptr;
			other.mSize = other.mCapacity = 0;
		}
		return *this;
	}

	template<typename T, typename Increment>
	void Vector<T, Increment>::Clear()
	{
		for (auto &element : *this)
		{
			element.~T();
		}
		mSize = 0;
	}

	template<typename T, typename Increment>
	typename Vector<T, Increment>::Iterator Vector<T, Increment>::Find(const T& value) const
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

	template<typename T, typename Increment>
	bool Vector<T, Increment>::Remove(const T& value)
	{
		return Remove(Find(value));
	}

	template<typename T, typename Increment>
	bool Vector<T, Increment>::Remove(Iterator position)
	{
		if (position.mOwner != this)
		{
			throw std::exception("Iterator does not belong to vector");
		}
		if (position == end())
		{
			return false;
		}
		uint32_t numBytesToShift = (mSize - 1 - position.mIndex) * sizeof(T);
		mData[position.mIndex].~T();
		memmove_s(&mData[position.mIndex], numBytesToShift, &mData[position.mIndex + 1], numBytesToShift);
		--mSize;
		return true;
	}

	template<typename T, typename Increment>
	bool Vector<T, Increment>::RemoveAt(uint32_t position)
	{
		assert(position < mSize);
		uint32_t numBytesToShift = (mSize - 1 - position) * sizeof(T);
		mData[position].~T();
		memmove_s(&mData[position], numBytesToShift, &mData[position + 1], numBytesToShift);
		--mSize;
		return true;
	}

	template<typename T, typename Increment>
	bool  Vector<T, Increment>::Remove(Iterator first, Iterator last)
	{
		if ((first.mOwner != this) || (last.mOwner != this) || (first == end()) || (first.mIndex > last.mIndex))
		{
			throw std::exception("Iterator does not belong to vector");
		}
		uint32_t numBytesToShift = (mSize - 1 - last.mIndex) * sizeof(T);
		for (uint32_t i = first.mIndex; i <= last.mIndex; ++i)
		{
			mData[i].~T();
		}
		memmove_s(&mData[first.mIndex], numBytesToShift, &mData[last.mIndex + 1], numBytesToShift);
		mSize -= (last.mIndex - first.mIndex + 1); //inclusive bounds; so 1 added
		return true;
	}

	template<typename T, typename Increment>
	void Vector<T, Increment>::Shrink()
	{
		if (mSize < mCapacity)
		{
			uint32_t numBytesToCopy = mSize * sizeof(T);
			T* newMem = reinterpret_cast<T*>(malloc(numBytesToCopy));
			memcpy_s(newMem, numBytesToCopy, mData, numBytesToCopy);
			free(mData);
			mData = newMem;
			mCapacity = mSize;
		}
	}
#pragma endregion

#pragma region Iterator
	template<typename T, typename Increment>
	Vector<T, Increment>::Iterator::Iterator()
		:mIndex(0), mOwner(nullptr)
	{

	}

	template<typename T, typename Increment>
	Vector<T, Increment>::Iterator::Iterator(uint32_t index, Vector& owner)
		:mIndex(index), mOwner(&owner)
	{

	}

	template<typename T, typename Increment>
	Vector<T, Increment>::Iterator::Iterator(const Iterator& other)
		:mIndex(other.mIndex), mOwner(other.mOwner)
	{

	}

	template<typename T, typename Increment>
	T& Vector<T, Increment>::Iterator::operator*() const
	{
		if (mOwner == nullptr || mIndex >= mOwner->Size())
		{
			throw std::exception("Cannot dereference end\n");
		}
		return mOwner->mData[mIndex];
	}

	template<typename T, typename Increment>
	bool Vector<T, Increment>::Iterator::operator==(const Iterator& other) const
	{
		return ((mOwner == other.mOwner) && (mIndex == other.mIndex));
	}

	template<typename T, typename Increment>
	bool  Vector<T, Increment>::Iterator::operator!=(const Iterator& other) const
	{
		return !(operator==(other));
	}

	template<typename T, typename Increment>
	typename Vector<T, Increment>::Iterator& Vector<T, Increment>::Iterator::operator++()
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Iterator for nothing");
		}
		if (mIndex < mOwner->Size())
		{
			++mIndex;
		}
		return *this;
	}

	template<typename T, typename Increment>
	typename Vector<T, Increment>::Iterator Vector<T, Increment>::Iterator::operator++(int)
	{
		if (mOwner == nullptr)
		{
			throw std::exception("Iterator for nothing");
		}
		Iterator temp = *this;
		operator++();
		return temp;
	}

	template<typename T, typename Increment>
	typename Vector<T, Increment>::Iterator& Vector<T, Increment>::Iterator::operator=(const Iterator& other)
	{
		if (this != &other)
		{
			mIndex = other.mIndex;
			mOwner = other.mOwner;
		}
		return *this;
	}

	template<typename T, typename Increment>
	void  Vector<T, Increment>::Resize(uint32_t size)
	{
		if (size < mSize)
		{
			for (; size < mSize; ++size)
			{
				mData[size].~T();
			}
			Shrink();
		}
		else
		{
			if (size > mCapacity)
			{
				Reserve(size);
			}
			for (uint32_t i = mSize; i < size; ++i)
			{
				new (mData + i)T();
			}
		}
		mSize = size;
	}
#pragma endregion
}
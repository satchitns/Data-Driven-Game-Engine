#include "pch.h"
#pragma warning(disable:4201)
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#pragma warning(default:4201)
#include "Scope.h"
#include "RTTI.h"
#include "Datum.h"

using namespace glm;
using namespace std;

namespace FieaGameEngine
{
	const size_t Datum::TypeSize[] =
	{
		0,					//unknown
		sizeof(int32_t),	//integer
		sizeof(float),		//float
		sizeof(vec4),		//vector
		sizeof(mat4),		//matrix
		sizeof(Scope*),		//table
		sizeof(string),		//string
		sizeof(RTTI*),		//pointer
		0					//max 
	};

	Datum::Datum(DatumType type)
		:mSize(0), mCapacity(0), mIsExternal(false), mType(type)
	{

	}

	Datum::Datum(const Datum& other)
		: mSize(other.mSize), mCapacity(0), mIsExternal(other.mIsExternal), mType(other.mType)
	{
		if (mIsExternal)
		{
			mData = other.mData;
			mCapacity = other.mCapacity;
		}
		else
		{
			mData.any = nullptr;
			Reserve(other.mCapacity);
			if (mType == DatumType::STRING)
			{
				mSize = 0;
				for (uint32_t i = 0; i < other.mSize; ++i)
				{
					PushBack(other.mData.s[i]);
				}
			}
			else
			{
				uint32_t size = static_cast<uint32_t>(mSize * TypeSize[static_cast<int>(mType)]);
				memcpy_s(mData.any, size, other.mData.any, size);
			}
		}
	}

	Datum::Datum(Datum&& other)
		: mSize(other.mSize), mCapacity(other.mCapacity), mIsExternal(other.mIsExternal), mType(other.mType), mData(std::move(other.mData))
	{
		other.mSize = other.mCapacity = 0;
		other.mIsExternal = false;
		other.mType = DatumType::UNKNOWN;
		other.mIsExternal = false;
		other.mData.any = nullptr;
	}

	Datum::Datum(const int32_t& other)
		:mSize(1), mCapacity(0), mIsExternal(false), mType(DatumType::INTEGER)
	{
		mData.any = nullptr;
		GuaranteeOneSpot();
		mData.i[0] = other;
	}

	Datum::Datum(const float& other)
		:mSize(1), mCapacity(0), mIsExternal(false), mType(DatumType::FLOAT)
	{
		mData.any = nullptr;
		GuaranteeOneSpot();
		mData.f[0] = other;
	}

	Datum::Datum(const glm::vec4& other)
		:mSize(1), mCapacity(0), mIsExternal(false), mType(DatumType::VECTOR)
	{
		mData.any = nullptr;
		GuaranteeOneSpot();
		mData.v[0] = other;
	}

	Datum::Datum(const glm::mat4& other)
		:mSize(1), mCapacity(0), mIsExternal(false), mType(DatumType::MATRIX)
	{
		mData.any = nullptr;
		GuaranteeOneSpot();
		mData.m[0] = other;
	}

	Datum::Datum(const std::string& other)
		: mSize(1), mCapacity(0), mIsExternal(false), mType(DatumType::STRING)
	{
		mData.any = nullptr;
		GuaranteeOneSpot();
		new (mData.s) string(other);
	}

	Datum::Datum(RTTI* const & other)
		: mSize(1), mCapacity(0), mIsExternal(false), mType(DatumType::POINTER)
	{
		mData.any = nullptr;
		GuaranteeOneSpot();
		mData.p[0] = other;
	}

	Datum::Datum(Scope* const & other)
		: mSize(1), mCapacity(0), mIsExternal(false), mType(DatumType::TABLE)
	{
		mData.any = nullptr;
		GuaranteeOneSpot();
		mData.t[0] = other;
	}

	Datum& Datum::operator=(const Datum& other)
	{
		if (this != &other)
		{
			if (!mIsExternal)
			{
				Clear();
				free(mData.any);
			}

			mData.any = nullptr;
			mSize = other.mSize;
			mIsExternal = other.mIsExternal;
			mType = other.mType;

			if (mIsExternal)
			{
				mData = other.mData;
				mCapacity = other.mCapacity;
			}
			else
			{
				Reserve(other.mCapacity);
				if (mType == DatumType::STRING)
				{
					mSize = 0;
					for (uint32_t i = 0; i < other.mSize; ++i)
					{
						PushBack(other.mData.s[i]);
					}
				}
				else
				{
					uint32_t size = static_cast<uint32_t>(mSize * TypeSize[static_cast<int>(mType)]);
					memcpy_s(mData.any, size, other.mData.any, size);
				}
			}
		}
		return *this;
	}

	Datum& Datum::operator=(Datum&& other)
	{
		if (this != &other)
		{
			if (!mIsExternal)
			{
				Clear();
				free(mData.any);
			}
			mSize = other.mSize;
			mCapacity = other.mCapacity;
			mIsExternal = other.mIsExternal;
			mType = other.mType;
			mData = std::move(other.mData);

			other.mSize = other.mCapacity = 0;
			other.mIsExternal = false;
			other.mType = DatumType::UNKNOWN;
			other.mIsExternal = false;
			other.mData.any = nullptr;
		}
		return *this;
	}

	Datum& Datum::operator=(const int32_t& other)
	{
		EnforceAndSetType(DatumType::INTEGER);
		GuaranteeOneSpot();
		mData.i[0] = other;
		HandleFirstAssignment();
		return *this;
	}

	Datum& Datum::operator=(const float& other)
	{
		EnforceAndSetType(DatumType::FLOAT);
		GuaranteeOneSpot();
		mData.f[0] = other;
		HandleFirstAssignment();
		return *this;
	}

	Datum& Datum::operator=(const glm::vec4& other)
	{
		EnforceAndSetType(DatumType::VECTOR);
		GuaranteeOneSpot();
		mData.v[0] = other;
		HandleFirstAssignment();
		return *this;
	}

	Datum& Datum::operator=(const glm::mat4& other)
	{
		EnforceAndSetType(DatumType::MATRIX);
		GuaranteeOneSpot();
		mData.m[0] = other;
		HandleFirstAssignment();
		return *this;
	}

	Datum& Datum::operator=(const std::string& other)
	{
		EnforceAndSetType(DatumType::STRING);
		GuaranteeOneSpot();
		if (mSize != 0)
		{
			mData.s[0] = other;
		}
		else //externals can't have size 0
		{
			new (mData.s)string(other);
			++mSize;
		}
		return *this;
	}

	Datum& Datum::operator=(RTTI* const & other)
	{
		EnforceInternal();
		EnforceAndSetType(DatumType::POINTER);
		GuaranteeOneSpot();
		mData.p[0] = other;
		HandleFirstAssignment();
		return *this;
	}

	Datum& Datum::operator=(Scope* const & other)
	{
		EnforceInternal();
		EnforceAndSetType(DatumType::TABLE);
		GuaranteeOneSpot();
		mData.t[0] = other;
		HandleFirstAssignment();
		return *this;
	}

	Datum::~Datum()
	{
		if (!mIsExternal)
		{
			Clear();
			free(mData.any);
		}
	}

	uint32_t Datum::Size() const
	{
		return mSize;
	}

	uint32_t Datum::Capacity() const
	{
		return mCapacity;
	}

	typename Datum::DatumType Datum::Type() const
	{
		return mType;
	}

	void Datum::SetType(DatumType type)
	{
		if (type == DatumType::UNKNOWN || type == DatumType::MAX)
		{
			throw std::exception("Bad type");
		}

		if (mType == DatumType::UNKNOWN || mType == type)
		{
			mType = type;
		}
		else
		{
			throw std::exception("Type was already set");
		}
	}

	void Datum::Reserve(uint32_t capacity)
	{
		EnforceInternal();
		if (capacity > mCapacity)
		{
			mData.any = realloc(mData.any, capacity * TypeSize[static_cast<int>(mType)]);
			mCapacity = capacity;
		}
	}

	void Datum::Resize(uint32_t size)
	{
		EnforceInternal();

		if (size < mSize)
		{
			if (mType == DatumType::STRING)
			{
				for (uint32_t i = 0; i < mSize; ++i)
				{
					mData.s[i].~string();
				}
			}
		}

		mData.any = realloc(mData.any, size * TypeSize[static_cast<int>(mType)]);

		for (uint32_t i = mSize; i < size; i++)
		{
			switch (mType)
			{
			case DatumType::STRING:
				new (mData.s + i) string();
				break;
			case DatumType::VECTOR:
				new (mData.v + i) vec4();
				break;
			case DatumType::MATRIX:
				new (mData.m + i) mat4();
				break;
			default:
				break;
			}
		}
		mSize = mCapacity = size;
	}

	void Datum::SetStorage(void * data, uint32_t count)
	{
		switch (mType)
		{
		case DatumType::INTEGER:
			SetStorage(reinterpret_cast<int*>(data), count);
			break;
		case DatumType::FLOAT:
			SetStorage(reinterpret_cast<float*>(data), count);
			break;
		case DatumType::STRING:
			SetStorage(reinterpret_cast<std::string*>(data), count);
			break;
		case DatumType::MATRIX:
			SetStorage(reinterpret_cast<glm::mat4*>(data), count);
			break;
		case DatumType::VECTOR:
			SetStorage(reinterpret_cast<glm::vec4*>(data), count);
			break;
		case DatumType::POINTER:
			SetStorage(reinterpret_cast<RTTI**>(data), count);
			break;
		case DatumType::UNKNOWN: case DatumType::MAX:
			throw std::exception("Type not set");
			break;
		}
	}

	void Datum::SetStorage(int32_t* data, uint32_t count)
	{
		if (data == nullptr)
		{
			throw std::exception("Empty array");
		}
		SetExternalAndAssignSizeAndCapacity(count);
		EnforceAndSetType(DatumType::INTEGER);
		mData.i = data;
	}

	void  Datum::SetStorage(float* data, uint32_t count)
	{
		if (data == nullptr)
		{
			throw std::exception("Empty array");
		}
		SetExternalAndAssignSizeAndCapacity(count);
		EnforceAndSetType(DatumType::FLOAT);
		mData.f = data;
	}

	void  Datum::SetStorage(glm::vec4* data, uint32_t count)
	{
		if (data == nullptr)
		{
			throw std::exception("Empty array");
		}
		SetExternalAndAssignSizeAndCapacity(count);
		EnforceAndSetType(DatumType::VECTOR);
		mData.v = data;
	}

	void  Datum::SetStorage(glm::mat4* data, uint32_t count)
	{
		if (data == nullptr)
		{
			throw std::exception("Empty array");
		}
		SetExternalAndAssignSizeAndCapacity(count);
		EnforceAndSetType(DatumType::MATRIX);
		mData.m = data;
	}

	void  Datum::SetStorage(RTTI** data, uint32_t count)
	{
		if (data == nullptr)
		{
			throw std::exception("Empty array");
		}
		SetExternalAndAssignSizeAndCapacity(count);
		EnforceAndSetType(DatumType::POINTER);
		mData.p = data;
	}

	void  Datum::SetStorage(std::string* data, uint32_t count)
	{
		if (data == nullptr)
		{
			throw std::exception("Empty array");
		}
		SetExternalAndAssignSizeAndCapacity(count);
		EnforceAndSetType(DatumType::STRING);
		mData.s = data;
	}

	void Datum::Clear()
	{
		EnforceInternal();
		for (uint32_t i = 0; i < mSize; i++)
		{
			if (mType == DatumType::STRING)
			{
				mData.s[i].~string();
			}
		}
		mSize = 0;
	}

	template<>
	const int32_t& Datum::Get<int32_t>(uint32_t index) const
	{
		EnforceType(DatumType::INTEGER);
		EnforceIndex(index);
		return mData.i[index];
	}

	template<>
	const float& Datum::Get<float>(uint32_t index) const
	{
		EnforceType(DatumType::FLOAT);
		EnforceIndex(index);
		return mData.f[index];
	}

	template<>
	const vec4& Datum::Get<vec4>(uint32_t index) const
	{
		EnforceType(DatumType::VECTOR);
		EnforceIndex(index);
		return mData.v[index];
	}
	template<>
	const mat4& Datum::Get<mat4>(uint32_t index) const
	{
		EnforceType(DatumType::MATRIX);
		EnforceIndex(index);
		return mData.m[index];
	}

	template<>
	const string& Datum::Get<string>(uint32_t index) const
	{
		EnforceType(DatumType::STRING);
		EnforceIndex(index);
		return mData.s[index];
	}

	template<>
	RTTI* const & Datum::Get<RTTI*>(uint32_t index) const
	{
		EnforceType(DatumType::POINTER);
		EnforceIndex(index);
		return mData.p[index];
	}

	template<>
	Scope* const & Datum::Get<Scope*>(uint32_t index) const
	{
		EnforceType(DatumType::TABLE);
		EnforceIndex(index);
		return mData.t[index];
	}

	template<>
	int32_t& Datum::Get<int32_t>(uint32_t index)
	{
		return const_cast<int32_t&>(const_cast<const Datum&>(*this).Get<int32_t>(index));
	}

	template<>
	float& Datum::Get<float>(uint32_t index)
	{
		return const_cast<float&>(const_cast<const Datum&>(*this).Get<float>(index));
	}

	template<>
	vec4& Datum::Get<vec4>(uint32_t index)
	{
		return const_cast<vec4&>(const_cast<const Datum&>(*this).Get<vec4>(index));
	}
	template<>
	mat4& Datum::Get<mat4>(uint32_t index)
	{
		return const_cast<mat4&>(const_cast<const Datum&>(*this).Get<mat4>(index));
	}

	template<>
	string& Datum::Get<string>(uint32_t index)
	{
		return const_cast<string&>(const_cast<const Datum&>(*this).Get<string>(index));
	}

	template<>
	RTTI* & Datum::Get<RTTI*>(uint32_t index)
	{
		return const_cast<RTTI*&>(const_cast<const Datum&>(*this).Get<RTTI*>(index));
	}

	template<>
	Scope* & Datum::Get<Scope*>(uint32_t index)
	{
		return const_cast<Scope*&>(const_cast<const Datum&>(*this).Get<Scope*>(index));
	}

	void Datum::Set(const int32_t& value, uint32_t index)
	{
		EnforceIndex(index);
		EnforceAndSetType(DatumType::INTEGER);
		mData.i[index] = value;
	}

	void Datum::Set(const float& value, uint32_t index)
	{
		EnforceIndex(index);
		EnforceAndSetType(DatumType::FLOAT);
		mData.f[index] = value;
	}
	void Datum::Set(const glm::vec4& value, uint32_t index)
	{
		EnforceIndex(index);
		EnforceAndSetType(DatumType::VECTOR);
		mData.v[index] = value;
	}

	void Datum::Set(const glm::mat4& value, uint32_t index)
	{
		EnforceIndex(index);
		EnforceAndSetType(DatumType::MATRIX);
		mData.m[index] = value;
	}

	void Datum::Set(const std::string& value, uint32_t index)
	{
		EnforceIndex(index);
		EnforceAndSetType(DatumType::STRING);
		mData.s[index] = value;
	}

	void Datum::Set(RTTI* const & value, uint32_t index)
	{
		EnforceIndex(index);
		EnforceAndSetType(DatumType::POINTER);
		mData.p[index] = value;
	}

	void Datum::Set(Scope* const & value, uint32_t index)
	{
		EnforceIndex(index);
		EnforceAndSetType(DatumType::TABLE);
		mData.t[index] = value;
	}

	void Datum::PushBack(const int32_t& value)
	{
		EnforceInternal();
		EnforceAndSetType(DatumType::INTEGER);
		ReservationStrategy();
		mData.i[mSize++] = value;
	}

	void Datum::PushBack(const float& value)
	{
		EnforceInternal();
		EnforceAndSetType(DatumType::FLOAT);
		ReservationStrategy();
		mData.f[mSize++] = value;
	}

	void Datum::PushBack(const glm::vec4& value)
	{
		EnforceInternal();
		EnforceAndSetType(DatumType::VECTOR);
		ReservationStrategy();
		mData.v[mSize++] = value;
	}

	void Datum::PushBack(const glm::mat4& value)
	{
		EnforceInternal();
		EnforceAndSetType(DatumType::MATRIX);
		ReservationStrategy();
		mData.m[mSize++] = value;
	}

	void Datum::PushBack(const std::string& value)
	{
		EnforceInternal();
		EnforceAndSetType(DatumType::STRING);
		ReservationStrategy();
		new (mData.s + mSize)string(value);
		mSize++;
	}

	void Datum::PushBack(RTTI* const & value)
	{
		EnforceInternal();
		EnforceAndSetType(DatumType::POINTER);
		ReservationStrategy();
		mData.p[mSize++] = value;
	}

	void Datum::PushBack(Scope* const & value)
	{
		EnforceInternal();
		EnforceAndSetType(DatumType::TABLE);
		ReservationStrategy();
		mData.t[mSize++] = value;
	}

	void Datum::PopBack()
	{
		EnforceInternal();
		EnforceIndex(0);
		if (mType == DatumType::STRING)
		{
			mData.s[mSize - 1].~string();
		}
		mSize--;
	}

	bool Datum::Find(const int& value, uint32_t &index) const
	{
		EnforceType(DatumType::INTEGER);
		bool found = false;
		for (uint32_t i = 0; i < mSize; ++i)
		{
			if (mData.i[i] == value)
			{
				found = true;
				index = i;
				break;
			}
		}
		return found;
	}

	bool Datum::Find(const float& value, uint32_t &index) const
	{
		EnforceType(DatumType::FLOAT);
		bool found = false;
		for (uint32_t i = 0; i < mSize; ++i)
		{
			if (mData.f[i] == value)
			{
				found = true;
				index = i;
				break;
			}
		}
		return found;
	}

	bool Datum::Find(const std::string& value, uint32_t &index) const
	{
		EnforceType(DatumType::STRING);
		bool found = false;
		for (uint32_t i = 0; i < mSize; ++i)
		{
			if (mData.s[i] == value)
			{
				found = true;
				index = i;
				break;
			}
		}
		return found;
	}

	bool Datum::Find(const glm::mat4& value, uint32_t &index) const
	{
		EnforceType(DatumType::MATRIX);
		bool found = false;
		for (uint32_t i = 0; i < mSize; ++i)
		{
			if (mData.m[i] == value)
			{
				found = true;
				index = i;
				break;
			}
		}
		return found;
	}

	bool Datum::Find(const glm::vec4& value, uint32_t &index) const
	{
		EnforceType(DatumType::VECTOR);
		bool found = false;
		for (uint32_t i = 0; i < mSize; ++i)
		{
			if (mData.v[i] == value)
			{
				found = true;
				index = i;
				break;
			}
		}
		return found;
	}

	bool Datum::Find(RTTI* const & value, uint32_t &index) const
	{
		EnforceType(DatumType::POINTER);
		bool found = false;
		for (uint32_t i = 0; i < mSize; ++i)
		{
			if (mData.p[i] != nullptr && mData.p[i]->Equals(value))
			{
				found = true;
				index = i;
				break;
			}
		}
		return found;
	}

	bool Datum::Find(Scope* const & value, uint32_t &index) const
	{
		EnforceType(DatumType::TABLE);
		bool found = false;
		for (uint32_t i = 0; i < mSize; ++i)
		{
			if (mData.t[i] != nullptr && mData.t[i]->Equals(value))
			{
				found = true;
				index = i;
				break;
			}
		}
		return found;
	}

	bool Datum::Find(const int& value) const
	{
		uint32_t index;
		return Find(value, index);
	}

	bool Datum::Find(const float& value) const
	{
		uint32_t index;
		return Find(value, index);
	}

	bool Datum::Find(const std::string& value) const
	{
		uint32_t index;
		return Find(value, index);
	}

	bool Datum::Find(const glm::mat4& value) const
	{
		uint32_t index;
		return Find(value, index);
	}

	bool Datum::Find(const glm::vec4& value) const
	{
		uint32_t index;
		return Find(value, index);
	}

	bool Datum::Find(RTTI* const & value) const
	{
		uint32_t index;
		return Find(value, index);
	}

	bool Datum::Find(Scope* const & value) const
	{
		uint32_t index;
		return Find(value, index);
	}

	bool Datum::Remove(uint32_t index)
	{
		bool removed = false;
		if (index < mSize)
		{
			size_t size = (mSize - 1 - index) * TypeSize[static_cast<uint32_t>(mType)];
			switch (mType)
			{
			case DatumType::INTEGER:
				memmove_s(&mData.i[index], size, &mData.i[index + 1], size);
				break;
			case DatumType::FLOAT:
				memmove_s(&mData.f[index], size, &mData.f[index + 1], size);
				break;
			case DatumType::MATRIX:
				memmove_s(&mData.m[index], size, &mData.m[index + 1], size);
				break;
			case DatumType::VECTOR:
				memmove_s(&mData.v[index], size, &mData.v[index + 1], size);
				break;
			case DatumType::STRING:
				mData.s[index].~string();
				memmove_s(&mData.s[index], size, &mData.s[index + 1], size);
				break;
			case DatumType::POINTER:
				memmove_s(&mData.p[index], size, &mData.p[index + 1], size);
				break;
			case DatumType::TABLE:
				memmove_s(&mData.t[index], size, &mData.t[index + 1], size);
				break;
			default:
				break;
			}
			removed = true;
			--mSize;
		}
		return removed;
	}

	bool Datum::Remove(const int& value)
	{
		uint32_t index;
		bool found = Find(value, index);
		if (!found)
		{
			return false;
		}
		return Remove(index);
	}

	bool Datum::Remove(const float& value)
	{
		uint32_t index;
		bool found = Find(value, index);
		if (!found)
		{
			return false;
		}
		return Remove(index);
	}

	bool Datum::Remove(const glm::vec4& value)
	{
		uint32_t index;
		bool found = Find(value, index);
		if (!found)
		{
			return false;
		}
		return Remove(index);
	}

	bool Datum::Remove(const glm::mat4& value)
	{
		uint32_t index;
		bool found = Find(value, index);
		if (!found)
		{
			return false;
		}
		return Remove(index);
	}

	bool Datum::Remove(const std::string& value)
	{
		uint32_t index;
		bool found = Find(value, index);
		if (!found)
		{
			return false;
		}
		return Remove(index);
	}

	bool Datum::Remove(RTTI* const & value)
	{
		uint32_t index;
		bool found = Find(value, index);
		if (!found)
		{
			return false;
		}
		return Remove(index);
	}

	bool Datum::Remove(Scope* const & value)
	{
		uint32_t index;
		bool found = Find(value, index);
		if (!found)
		{
			return false;
		}
		return Remove(index);
	}

	void Datum::SetFromString(const std::string& string, uint32_t index)
	{
		EnforceIndex(index);
		if (mType == DatumType::POINTER || mType == DatumType::TABLE)
		{
			throw std::exception("Not enough type information");
		}
		switch (mType)
		{
		case DatumType::FLOAT:
			mData.f[index] = stof(string);
			break;
		case DatumType::INTEGER:
			mData.i[index] = stoi(string);
			break;
		case DatumType::STRING:
			mData.s[index] = string;
			break;
		case DatumType::MATRIX:
			sscanf_s(string.c_str(),
				"mat4x4((%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f), (%f, %f, %f, %f))",
				&mData.m[index][0][0], &mData.m[index][0][1], &mData.m[index][0][2], &mData.m[index][0][3],
				&mData.m[index][1][0], &mData.m[index][1][1], &mData.m[index][1][2], &mData.m[index][1][3],
				&mData.m[index][2][0], &mData.m[index][2][1], &mData.m[index][2][2], &mData.m[index][2][3],
				&mData.m[index][3][0], &mData.m[index][3][1], &mData.m[index][3][2], &mData.m[index][3][3]);
			break;
		case DatumType::VECTOR:
			sscanf_s(string.c_str(),
				"vec4(%f, %f, %f, %f)",
				&mData.v[index].x,
				&mData.v[index].y,
				&mData.v[index].z,
				&mData.v[index].w);
			break;
		default:
			break;
		}
	}

	std::string Datum::ToString(uint32_t index) const
	{
		EnforceIndex(index);
		switch (mType)
		{
		case DatumType::FLOAT:
			return to_string(mData.f[index]);
		case DatumType::INTEGER:
			return to_string(mData.i[index]);
		case DatumType::STRING:
			return mData.s[index];
		case DatumType::POINTER:
		case DatumType::TABLE:
			if (mData.p[index] == nullptr)
			{
				throw exception("Bad pointer");
			}
			return mData.p[index]->ToString();
		case DatumType::MATRIX:
			return glm::to_string(mData.m[index]);
		case DatumType::VECTOR:
			return glm::to_string(mData.v[index]);
		default:
			return "";
		}
	}

	bool Datum::operator==(const Datum& other) const
	{
		if ((other.mType == mType) && (other.mSize == mSize))
		{
			if (mType == DatumType::STRING)
			{
				bool equal = true;
				for (uint32_t i = 0; i < mSize; ++i)
				{
					if (mData.s[i] != other.mData.s[i])
					{
						equal = false;
						break;
					}
				}
				return equal;
			}
			else if (mType == DatumType::POINTER || mType == DatumType::TABLE)
			{
				bool equal = true;
				for (uint32_t i = 0; i < mSize; ++i)
				{
					if ((mData.p[i] != other.mData.p[i]) && (mData.p[i] != nullptr) && !mData.p[i]->Equals(other.mData.p[i]))
					{
						equal = false;
						break;
					}
				}
				return equal;
			}
			return(!memcmp(other.mData.any, mData.any, mSize * TypeSize[static_cast<int>(mType)]));
		}
		return false;
	}

	bool Datum::operator!=(const Datum& other) const
	{
		return !operator==(other);
	}

	void Datum::EnforceType(DatumType type) const
	{
		if (mType != type)
		{
			throw std::exception("Wrong type");
		}
	}

	void Datum::EnforceAndSetType(DatumType type)
	{
		if ((mType != DatumType::UNKNOWN) && (mType != type))
		{
			throw std::exception("Wrong type");
		}
		mType = type;
	}

	void Datum::EnforceIndex(uint32_t index) const
	{
		if (index >= mSize)
		{
			throw std::exception("Index out of range");
		}
	}

	void Datum::GuaranteeOneSpot()
	{
		if (!mIsExternal) //externals can't have size 0, auto-enforced
		{
			Reserve(1);
		}
	}

	void Datum::SetExternalAndAssignSizeAndCapacity(uint32_t count)
	{
		if (count == 0)
		{
			throw std::exception("Bad size");
		}
		if (!mIsExternal && mCapacity > 0)
		{
			throw std::exception("Cannot assign external");
		}
		mSize = mCapacity = count;
		mIsExternal = true;
	}

	void Datum::ReservationStrategy()
	{
		if (mCapacity == 0)
		{
			Reserve(1);
		}
		else if (mSize == mCapacity)
		{
			Reserve(mSize * 2);
		}
	}

	void Datum::EnforceInternal() const
	{
		if (mIsExternal)
		{
			throw std::exception("Illegal operation on external data");
		}
	}

	void Datum::HandleFirstAssignment()
	{
		if (mSize == 0)
		{
			mSize = 1;
		}
	}

	Scope& Datum::operator[](std::uint32_t index)
	{
		Scope* tempScope = Get<Scope*>(index);
		if (tempScope == nullptr)
		{
			throw exception("Cannot dereference nullptr");
		}
		return *tempScope;
	}

	bool Datum::IsExternal() const
	{
		return mIsExternal;
	}
}
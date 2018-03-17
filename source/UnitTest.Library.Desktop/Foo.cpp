#include "stdafx.h"
#include "Foo.h"

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(Foo)

		Foo::Foo()
		:mData(0), mOtherData(new int())
	{

	}

	Foo::~Foo()
	{
		delete mOtherData;
	}
	int Foo::Data() const
	{
		return mData;
	}
	int* Foo::OtherData() const
	{
		return mOtherData;
	}
	Foo& Foo::operator=(const Foo& other)
	{
		if (this != &other)
		{
			mData = other.mData;
			*mOtherData = *(other.mOtherData);
		}
		return *this;
	}

	bool Foo::operator==(const Foo& other) const
	{
		if (this == &other)
		{
			return true;
		}
		else if ((mData == other.mData) && (*mOtherData == *(other.mOtherData)))
		{
			return true;
		}
		return false;
	}

	bool Foo::operator!=(const Foo& other) const
	{
		return !(*this == other);
	}
	Foo::Foo(const Foo& other)
		:mOtherData(new int())
	{
		mData = other.mData;
		*mOtherData = *(other.mOtherData);
	}

	Foo::Foo(int data, int otherData)
		:mData(data), mOtherData(new int(otherData))
	{

	}

	bool Foo::Equals(const RTTI* rhs) const
	{
		if (rhs != nullptr && rhs->Is(TypeIdClass()))
		{
			Foo* otherFoo = rhs->As<Foo>();
			if (otherFoo == this)
			{
				return true;
			}
			return operator==(*otherFoo);
		}
		return false;
	}
}


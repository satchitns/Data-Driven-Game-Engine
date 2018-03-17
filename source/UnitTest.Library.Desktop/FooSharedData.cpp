#include "stdafx.h"
#include "FooSharedData.h"
#include "Foo.h"

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(FooSharedData)

	FooSharedData::FooSharedData(Foo& fooToStore)
	{
		mFoo = &fooToStore;
	}

	FooSharedData::~FooSharedData()
	{
		if (mIsClone)
		{
			delete mFoo;
		}
	}

	void FooSharedData::SetData(int data) const
	{
		mFoo->mData = data;
	}

	void FooSharedData::SetOtherData(int otherData) const
	{
		*(mFoo->mOtherData) = otherData;
	}

	Foo & FooSharedData::GetFoo()
	{
		return *mFoo;
	}

	FieaGameEngine::JsonParseMaster::SharedData * FooSharedData::Clone() const
	{
		FooSharedData *clone = new FooSharedData(*this);
		clone->mIsClone = true;
		clone->mFoo = new Foo(*mFoo);
		return clone;
	}
}
#pragma once
#include "JsonParseMaster.h"

namespace UnitTestLibraryDesktop
{
	class Foo;
	class FooSharedData : public FieaGameEngine::JsonParseMaster::SharedData
	{
		RTTI_DECLARATIONS(FooSharedData, FieaGameEngine::JsonParseMaster::SharedData)
	public:
		FooSharedData(Foo& fooToStore);
		FooSharedData(const FooSharedData&) = default;
		FooSharedData& operator=(const FooSharedData&) = default;
		virtual ~FooSharedData();
		void SetData(int data) const;
		void SetOtherData(int otherData) const;
		Foo& GetFoo();
		virtual SharedData* Clone() const override;
	private:
		Foo * mFoo;
	};
}
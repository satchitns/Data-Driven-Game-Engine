#pragma once

namespace UnitTestLibraryDesktop
{
	class Foo final : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(Foo, RTTI)
			friend class FooSharedData;
	public:
		Foo();
		~Foo();
		int Data() const;
		int* OtherData() const;
		Foo& operator=(const Foo& other);
		bool operator==(const Foo& other) const;
		bool operator!=(const Foo& other) const;
		Foo(const Foo& other);
		Foo(int data, int otherData = 0);
		virtual bool Equals(const RTTI* rhs) const override;
	private:
		int mData;
		int* mOtherData;
	};
}



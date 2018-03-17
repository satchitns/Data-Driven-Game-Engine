#include "stdafx.h"
#include "AttributedFoo.h"

using namespace std;
using namespace glm;

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(AttributedFoo)

		AttributedFoo::AttributedFoo()
		:Attributed(TypeIdInstance()), mFoo(new Foo())
	{
		InitializeScope();
	}


	AttributedFoo::~AttributedFoo()
	{
		delete mFoo;
	}

	AttributedFoo::AttributedFoo(const AttributedFoo& other)
		:Attributed(other), mCount(other.mCount), mHealth(other.mHealth), mName(other.mName),
		mPosition(other.mPosition), mTransform(other.mTransform), mFoo(new Foo())
	{
		std::copy(begin(other.intArray), end(other.intArray), intArray);
		std::copy(begin(other.floatArray), end(other.floatArray), floatArray);
		std::copy(begin(other.strArray), end(other.strArray), strArray);
		std::copy(begin(other.vecArray), end(other.vecArray), vecArray);
		std::copy(begin(other.matArray), end(other.matArray), matArray);
		*mFoo = *other.mFoo;
		UpdateScope();
	}

	AttributedFoo::AttributedFoo(AttributedFoo&& other)
		:Attributed(move(other)), mCount(other.mCount), mHealth(other.mHealth), mName(move(other.mName)),
		mPosition(other.mPosition), mTransform(other.mTransform)
	{
		std::move(begin(other.intArray), end(other.intArray), intArray);
		std::move(begin(other.floatArray), end(other.floatArray), floatArray);
		std::move(begin(other.strArray), end(other.strArray), strArray);
		std::move(begin(other.vecArray), end(other.vecArray), vecArray);
		std::move(begin(other.matArray), end(other.matArray), matArray);
		mFoo = other.mFoo;
		other.mCount = 0;
		other.mHealth = 0.0f;
		other.mFoo = nullptr;
		other.mPosition = vec4(0);
		other.mTransform = mat4(0);
		UpdateScope();
	}

	AttributedFoo& AttributedFoo::operator=(const AttributedFoo& other)
	{
		if (this != &other)
		{
			Attributed::operator=(other);
			mCount = other.mCount;
			mHealth = other.mHealth;
			mName = other.mName;
			mPosition = other.mPosition;
			mTransform = other.mTransform;
			std::copy(begin(other.intArray), end(other.intArray), intArray);
			std::copy(begin(other.floatArray), end(other.floatArray), floatArray);
			std::copy(begin(other.strArray), end(other.strArray), strArray);
			std::copy(begin(other.vecArray), end(other.vecArray), vecArray);
			std::copy(begin(other.matArray), end(other.matArray), matArray);
			*mFoo = *other.mFoo;
			UpdateScope();
		}
		return *this;
	}

	AttributedFoo& AttributedFoo::operator=(AttributedFoo&& other)
	{
		if (this != &other)
		{
			Attributed::operator=(std::move(other));
			mCount = other.mCount;
			mHealth = other.mHealth;
			mPosition = other.mPosition;
			mTransform = other.mTransform;
			mName = move(other.mName);
			std::move(begin(other.intArray), end(other.intArray), intArray);
			std::move(begin(other.floatArray), end(other.floatArray), floatArray);
			std::move(begin(other.strArray), end(other.strArray), strArray);
			std::move(begin(other.vecArray), end(other.vecArray), vecArray);
			std::move(begin(other.matArray), end(other.matArray), matArray);
			delete mFoo;
			mFoo = other.mFoo;			
			other.mFoo = nullptr;
			other.mCount = 0;
			other.mHealth = 0.0f;
			other.mPosition = vec4(0);
			other.mTransform = mat4(0);
			UpdateScope();
		}
		return *this;
	}

	void AttributedFoo::InitializeScope()
	{
		AddExternalAttribute("Count", mCount);
		AddExternalAttribute("Health", mHealth);
		AddExternalAttribute("Position", mPosition);
		AddExternalAttribute("Transform", mTransform);
		AddExternalAttribute("Name", mName);
		AddExternalAttribute("Foo", mFoo);

		AddExternalAttribute("IntArray", intArray[0], 3);
		AddExternalAttribute("FloatArray", floatArray[0], 3);
		AddExternalAttribute("MatArray", matArray[0], 3);
		AddExternalAttribute("VecArray", vecArray[0], 3);
		AddExternalAttribute("StrArray", strArray[0], 3);

		AddInternalAttribute("Count1", 0, 1);
		AddInternalAttribute("Health1", 100.0f, 1);
		AddInternalAttribute("Position1", vec4(0), 1);
		AddInternalAttribute("Transform1", mat4(1), 1);
		AddInternalAttribute("Aliases1", "hi", 3);
		RTTI * foo = new Foo(1, 1);
		AddInternalAttribute("Foo1", foo, 1);
		delete foo;

		AddNestedScope("NestedScope");
	}

	void AttributedFoo::UpdateScope()
	{
		UpdateExternalAttribute("Count", mCount);
		UpdateExternalAttribute("Health", mHealth);
		UpdateExternalAttribute("Position", mPosition);
		UpdateExternalAttribute("Transform", mTransform);
		UpdateExternalAttribute("Name", mName);
		UpdateExternalAttribute("Foo", mFoo);

		UpdateExternalAttribute("IntArray", intArray[0], 3);
		UpdateExternalAttribute("FloatArray", floatArray[0], 3);
		UpdateExternalAttribute("MatArray", matArray[0], 3);
		UpdateExternalAttribute("VecArray", vecArray[0], 3);
		UpdateExternalAttribute("StrArray", strArray[0], 3);
	}

	void AttributedFoo::BadUpdateScope1()
	{
		UpdateExternalAttribute("Count1", mCount);
	}

	void AttributedFoo::BadUpdateScope2()
	{
		UpdateExternalAttribute("Health1", mHealth);
	}

	void AttributedFoo::BadUpdateScope3()
	{
		UpdateExternalAttribute("Position1", mPosition);
	}

	void AttributedFoo::BadUpdateScope4()
	{
		UpdateExternalAttribute("Transform1", mTransform);
	}

	void AttributedFoo::BadUpdateScope5()
	{
		UpdateExternalAttribute("Aliases1", mName, 1);
	}

	void AttributedFoo::BadUpdateScope6()
	{
		UpdateExternalAttribute("Foo1", mFoo);
	}
}
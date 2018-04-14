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
		}
		return *this;
	}

	FieaGameEngine::Scope* AttributedFoo::Clone()
	{
		return new AttributedFoo(*this);
	}
}
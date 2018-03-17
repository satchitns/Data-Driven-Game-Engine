#pragma once
#include "Attributed.h"
#include "Foo.h"

namespace UnitTestLibraryDesktop
{
	class AttributedFoo final : public FieaGameEngine::Attributed
	{
		RTTI_DECLARATIONS(AttributedFoo, Attributed)
	public:
		AttributedFoo();
		~AttributedFoo();
		AttributedFoo(const AttributedFoo& other);
		AttributedFoo(AttributedFoo&& other);
		AttributedFoo& operator=(const AttributedFoo& other);
		AttributedFoo& operator=(AttributedFoo&& other);
		int32_t mCount, intArray[3];
		float mHealth, floatArray[3];
		glm::vec4 mPosition, vecArray[3];
		glm::mat4 mTransform, matArray[3];
		std::string mName, strArray[3];
		RTTI* mFoo;
		void BadUpdateScope1();
		void BadUpdateScope2();
		void BadUpdateScope3();
		void BadUpdateScope4();
		void BadUpdateScope5();
		void BadUpdateScope6();
	private:
		void InitializeScope();
		void UpdateScope();
	};
}
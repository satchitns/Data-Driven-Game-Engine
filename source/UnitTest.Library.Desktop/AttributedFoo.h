#pragma once
#include "Attributed.h"
#include "Foo.h"
#include "Vector.h"

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
		static FieaGameEngine::Vector<Signature> Signatures()
		{
			return FieaGameEngine::Vector<Signature>
			{
				Signature("Count", FieaGameEngine::Datum::DatumType::INTEGER, 1, offsetof(AttributedFoo, mCount)),
					Signature("IntArray", FieaGameEngine::Datum::DatumType::INTEGER, 3, offsetof(AttributedFoo, intArray)),
					Signature("Health", FieaGameEngine::Datum::DatumType::FLOAT, 1, offsetof(AttributedFoo, mHealth)),
					Signature("FloatArray", FieaGameEngine::Datum::DatumType::FLOAT, 3, offsetof(AttributedFoo, floatArray)),
					Signature("Position", FieaGameEngine::Datum::DatumType::VECTOR, 1, offsetof(AttributedFoo, mPosition)),
					Signature("VecArray", FieaGameEngine::Datum::DatumType::VECTOR, 3, offsetof(AttributedFoo, vecArray)),
					Signature("Transform", FieaGameEngine::Datum::DatumType::MATRIX, 1, offsetof(AttributedFoo, mTransform)),
					Signature("MatArray", FieaGameEngine::Datum::DatumType::MATRIX, 3, offsetof(AttributedFoo, matArray)),
					Signature("Name", FieaGameEngine::Datum::DatumType::STRING, 1, offsetof(AttributedFoo, mName)),
					Signature("StrArray", FieaGameEngine::Datum::DatumType::STRING, 3, offsetof(AttributedFoo, strArray)),
					Signature("Foo", FieaGameEngine::Datum::DatumType::POINTER, 1, offsetof(AttributedFoo, mFoo))
			};
		}
	protected:
		virtual Scope* Clone() override;
	};
}
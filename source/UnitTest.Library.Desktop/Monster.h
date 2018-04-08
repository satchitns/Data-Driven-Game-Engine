#pragma once
#include "Entity.h"
namespace UnitTestLibraryDesktop
{
	class Monster final : public FieaGameEngine::Entity
	{
		RTTI_DECLARATIONS(Monster, FieaGameEngine::Entity)
	public:
		Monster();
		virtual ~Monster() = default;
		Monster(const Monster& other) = delete;
		Monster& operator=(const Monster& other) = delete;
		Monster(Monster&& other) = default;
		Monster& operator=(Monster&& other) = default;

		static FieaGameEngine::Vector<Signature> Signatures()
		{
			return Entity::Signatures().Append({
			Signature("Health", FieaGameEngine::Datum::DatumType::INTEGER, 1, offsetof(Monster, mHealth))
			});
		}
	private:
		int mHealth = 0;
	};
}
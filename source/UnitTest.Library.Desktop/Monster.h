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
		Monster(Monster&& other);
		Monster& operator=(Monster&& other);
	private:
		int mHealth = 0;
	};
}
#include "stdafx.h"
#include "Monster.h"

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(Monster)
	Monster::Monster()
		:Entity(TypeIdInstance())
	{
		AddExternalAttribute("Health", mHealth);
	}

	Monster::Monster(Monster&& other)
		: Entity(std::move(other)), mHealth(std::move(other.mHealth))
	{
		UpdateExternalAttribute("Health", mHealth);
	}

	Monster& Monster::operator=(Monster&& other)
	{
		if (this != &other)
		{
			Entity::operator=(std::move(other));
			mHealth = std::move(other.mHealth);
			UpdateExternalAttribute("Health", mHealth);
		}
		return *this;
	}
}
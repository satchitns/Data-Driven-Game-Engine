#include "stdafx.h"
#include "Monster.h"

namespace UnitTestLibraryDesktop
{
	RTTI_DEFINITIONS(Monster)
	Monster::Monster()
		:Entity(TypeIdInstance())
	{
	}
}
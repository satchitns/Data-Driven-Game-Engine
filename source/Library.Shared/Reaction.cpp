#include "pch.h"
#include "Reaction.h"

namespace FieaGameEngine
{
	RTTI_DEFINITIONS(Reaction)

	Reaction::Reaction(uint64_t typeId)
		:ActionList(typeId)
	{

	}
}
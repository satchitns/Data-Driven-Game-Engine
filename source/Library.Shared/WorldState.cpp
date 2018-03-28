#include "pch.h"
#include "WorldState.h"

namespace FieaGameEngine
{
	class GameTime* WorldState::GetGameTime() const
	{
		return mGameTime;
	}

	void WorldState::SetGameTime(GameTime & gameTime)
	{
		mGameTime = &gameTime;
	}
}

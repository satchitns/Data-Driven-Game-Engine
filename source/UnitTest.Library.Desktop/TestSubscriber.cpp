#include "stdafx.h"
#include "TestSubscriber.h"
#include "EventPublisher.h"
#include "Monster.h"
#include"Event.h"

namespace UnitTestLibraryDesktop
{
	void TestSubscriber::Notify(const FieaGameEngine::EventPublisher & event)
	{
		if (event.Is(FieaGameEngine::Event<Monster>::TypeIdClass()))
		{
			mMonsterCount++;
		}
		event;
		mCount++;
	}
}

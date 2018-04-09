#pragma once
#include "EventSubscriber.h"

namespace UnitTestLibraryDesktop
{
	class TestSubscriber final : public FieaGameEngine::EventSubscriber
	{
	public:
		TestSubscriber() = default;
		~TestSubscriber() = default;
		void Notify(const FieaGameEngine::EventPublisher& event) override;
		int mCount = 0;
		int mMonsterCount = 0;
	};
}


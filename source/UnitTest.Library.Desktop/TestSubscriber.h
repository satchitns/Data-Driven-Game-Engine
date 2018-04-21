#pragma once
#include "EventSubscriber.h"
#include "EventQueue.h"
#include "GameTime.h"

namespace UnitTestLibraryDesktop
{

	class TestSubscriber final : public FieaGameEngine::EventSubscriber
	{
	public:
		TestSubscriber() = default;
		~TestSubscriber() = default;
		void Notify(const FieaGameEngine::EventPublisher& event) override;
		int mCount = 0;
		int mIntCount = 0;
		int mMonsterCount = 0;
		FieaGameEngine::EventQueue* queue;
		FieaGameEngine::GameTime time;
	};
}


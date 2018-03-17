#pragma once
namespace UnitTestLibraryDesktop
{
	class TestRTTI final : public FieaGameEngine::RTTI
	{
		RTTI_DECLARATIONS(TestRTTI, RTTI)
	public:
		TestRTTI();
		~TestRTTI();
	};
}


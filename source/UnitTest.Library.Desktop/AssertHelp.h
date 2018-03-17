#pragma once
#include "stdafx.h"
#include "Foo.h"

namespace Microsoft::VisualStudio::CppUnitTestFramework
{
	template<>
	std::wstring ToString<UnitTestLibraryDesktop::Foo>(const UnitTestLibraryDesktop::Foo& t)
	{
		RETURN_WIDE_STRING(t.Data());
	}
}
// © Copyright 2010 - 2020 BlackTopp Studios Inc.
/* This file is part of The Mezzanine Engine.

    The Mezzanine Engine is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    The Mezzanine Engine is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
*/
/* The original authors have included a copy of the license specified above in the
   'Docs' folder. See 'gpl.txt'
*/
/* We welcome the use of the Mezzanine engine to anyone, including companies who wish to
   Build professional software and charge for their product.

   However there are some practical restrictions, so if your project involves
   any of the following you should contact us and we will try to work something
   out:
    - DRM or Copy Protection of any kind(except Copyrights)
    - Software Patents You Do Not Wish to Freely License
    - Any Kind of Linking to Non-GPL licensed Works
    - Are Currently In Violation of Another Copyright Holder's GPL License
    - If You want to change our code and not add a few hundred MB of stuff to
        your distribution

   These and other limitations could cause serious legal problems if you ignore
   them, so it is best to simply contact us or the Free Software Foundation, if
   you have any questions.

   Joseph Toppi - toppij@gmail.com
   John Blackwood - makoenergy02@gmail.com
*/
#ifndef Mezz_Test_ConsoleLogicTests_h
#define Mezz_Test_ConsoleLogicTests_h

/// @file
/// @brief A header full of tests for functions that make the console stuff work correctly.

// Add other headers you need here
#include "MezzTest.h"

using Mezzanine::String;
using Mezzanine::Testing::TestResult;
using Mezzanine::Testing::Char8ToTestResults;
using Mezzanine::Testing::GetUsageString;

AUTOMATIC_TEST_GROUP(ConsoleLogicTests, ConsoleLogic)
{
    TEST_EQUAL("Char8ToTestResults::TIsSuccess", TestResult::Success, Char8ToTestResults('T'))
    TEST_EQUAL("Char8ToTestResults::tIsSuccess", TestResult::Success, Char8ToTestResults('t'))
    TEST_EQUAL("Char8ToTestResults::YIsSuccess", TestResult::Success, Char8ToTestResults('Y'))
    TEST_EQUAL("Char8ToTestResults::yIsSuccess", TestResult::Success, Char8ToTestResults('y'))

    TEST_EQUAL("Char8ToTestResults::FIsFailure", TestResult::Failed, Char8ToTestResults('F'))
    TEST_EQUAL("Char8ToTestResults::fIsFailure", TestResult::Failed, Char8ToTestResults('f'))
    TEST_EQUAL("Char8ToTestResults::NIsFailure", TestResult::Failed, Char8ToTestResults('N'))
    TEST_EQUAL("Char8ToTestResults::nIsFailure", TestResult::Failed, Char8ToTestResults('n'))

    TEST_EQUAL("Char8ToTestResults::CIsCancelled", TestResult::Cancelled, Char8ToTestResults('C'))
    TEST_EQUAL("Char8ToTestResults::cIsCancelled", TestResult::Cancelled, Char8ToTestResults('c'))

    TEST_EQUAL("Char8ToTestResults::WIsWarning", TestResult::Warning, Char8ToTestResults('W'))
    TEST_EQUAL("Char8ToTestResults::wIsWarning", TestResult::Warning, Char8ToTestResults('w'))

    TEST_EQUAL("Char8ToTestResults::UIsInconclusive", TestResult::Inconclusive, Char8ToTestResults('U'))
    TEST_EQUAL("Char8ToTestResults::uIsInconclusive", TestResult::Inconclusive, Char8ToTestResults('u'))
    TEST_EQUAL("Char8ToTestResults::IIsInconclusive", TestResult::Inconclusive, Char8ToTestResults('I'))
    TEST_EQUAL("Char8ToTestResults::iIsInconclusive", TestResult::Inconclusive, Char8ToTestResults('i'))

    TEST_EQUAL("Char8ToTestResults::QIsUnknown",                TestResult::Unknown, Char8ToTestResults('Q'))
    TEST_EQUAL("Char8ToTestResults::+IsUnknown",                TestResult::Unknown, Char8ToTestResults('+'))
    TEST_EQUAL("Char8ToTestResults::~IsUnknown",                TestResult::Unknown, Char8ToTestResults('~'))
    TEST_EQUAL("Char8ToTestResults::TabIsUnknown",              TestResult::Unknown, Char8ToTestResults('\t'))
    TEST_EQUAL("Char8ToTestResults::NullTerminatorIsUnknown",   TestResult::Unknown, Char8ToTestResults('\0'))

    // This fails to build on Ubuntu because it is a multi-byte char. Does it fail on windows too?
    //TEST_EQUAL("Char8ToTestResults::ñIsUnknown",
    //           Mezzanine::Testing::TestResult::Unknown,
    //           Mezzanine::Testing::Char8ToTestResults('ñ'));

    // This fails to build on Ubuntu because it is beyond the range of a signed sign char. Does it fail on
    // windows too?
    //TEST_EQUAL("Char8ToTestResults::RawByte244IsUnknown",
    //           Mezzanine::Testing::TestResult::Unknown,
    //          Mezzanine::Testing::Char8ToTestResults(244));

    // This shit fails to build as it should.
    //TEST_EQUAL("Char8ToTestResults::💩IsUnknown",
    //           Mezzanine::Testing::TestResult::Inconclusive,
    //           Mezzanine::Testing::Char8ToTestResults('💩'));

    Mezzanine::Testing::CoreTestGroup FakeTestGroup;
    ConsoleLogicTests ConsoleLogicInstance;
    FakeTestGroup[ConsoleLogicInstance.Name()] = &ConsoleLogicInstance;

    TEST_STRING_CONTAINS("Usage::Usage",         String("Usage"),        GetUsageString("Tester", FakeTestGroup))
    TEST_STRING_CONTAINS("Usage::HasTestNaem",   ConsoleLogicInstance.Name(), GetUsageString("Tester", FakeTestGroup))
    TEST_STRING_CONTAINS("GetPrintableTestList", ConsoleLogicInstance.Name(), GetPrintableTestList(80, FakeTestGroup))

    // Lets test usage, because we can
    {
        Mezzanine::Testing::OutputBufferGuard CerrGuard(std::cerr);
        Mezzanine::Testing::Usage("Foo", FakeTestGroup)
        TEST_STRING_CONTAINS("Usage::EmitsNameToCerr", String("Foo"), CerrGuard.GetSunkOutput())
    }
}

#endif

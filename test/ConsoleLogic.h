// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
/// @brief A Test header suitable for copying as a starting point for tests.

// Add other headers you need here
#include "MezzTest.h"

SAVE_WARNING_STATE
SUPPRESS_CLANG_WARNING("-Wweak-vtables") // We really don't care, because this will this will be recompiled each test.
SUPPRESS_CLANG_WARNING("-Wpadded") // Test classes will likely need to be padded and we don't care.

/// @brief The test suite will find all classes identified as tests in CMake, but they need to look like this.
/// @details This class's name is the filename + "Tests"
class ConsoleLogicTests : public Mezzanine::Testing::UnitTestGroup
{
    public:
        /// @brief Default Constructor
        ConsoleLogicTests() = default;

        /// @brief Default Deconstructor
        virtual ~ConsoleLogicTests() = default;

        // This is used as the name of the test on the command prompt
        virtual Mezzanine::String Name() override
            { return "consolelogic"; }

        void RunAutomaticTests() override
        {
            TEST_EQUAL("Char8ToTestResults::TIsSuccess",
                       Mezzanine::Testing::TestResult::Success,
                       Mezzanine::Testing::Char8ToTestResults('T'));
            TEST_EQUAL("Char8ToTestResults::tIsSuccess",
                       Mezzanine::Testing::TestResult::Success,
                       Mezzanine::Testing::Char8ToTestResults('t'));
            TEST_EQUAL("Char8ToTestResults::YIsSuccess",
                       Mezzanine::Testing::TestResult::Success,
                       Mezzanine::Testing::Char8ToTestResults('Y'));
            TEST_EQUAL("Char8ToTestResults::yIsSuccess",
                       Mezzanine::Testing::TestResult::Success,
                       Mezzanine::Testing::Char8ToTestResults('y'));

            TEST_EQUAL("Char8ToTestResults::FIsFailure",
                       Mezzanine::Testing::TestResult::Failed,
                       Mezzanine::Testing::Char8ToTestResults('F'));
            TEST_EQUAL("Char8ToTestResults::fIsFailure",
                       Mezzanine::Testing::TestResult::Failed,
                       Mezzanine::Testing::Char8ToTestResults('f'));
            TEST_EQUAL("Char8ToTestResults::NIsFailure",
                       Mezzanine::Testing::TestResult::Failed,
                       Mezzanine::Testing::Char8ToTestResults('N'));
            TEST_EQUAL("Char8ToTestResults::nIsFailure",
                       Mezzanine::Testing::TestResult::Failed,
                       Mezzanine::Testing::Char8ToTestResults('n'));

            TEST_EQUAL("Char8ToTestResults::CIsCancelled",
                       Mezzanine::Testing::TestResult::Cancelled,
                       Mezzanine::Testing::Char8ToTestResults('C'));
            TEST_EQUAL("Char8ToTestResults::cIsCancelled",
                       Mezzanine::Testing::TestResult::Cancelled,
                       Mezzanine::Testing::Char8ToTestResults('c'));

            TEST_EQUAL("Char8ToTestResults::WIsWarning",
                       Mezzanine::Testing::TestResult::Warning,
                       Mezzanine::Testing::Char8ToTestResults('W'));
            TEST_EQUAL("Char8ToTestResults::wIsWarning",
                       Mezzanine::Testing::TestResult::Warning,
                       Mezzanine::Testing::Char8ToTestResults('w'));

            TEST_EQUAL("Char8ToTestResults::UIsInconclusive",
                       Mezzanine::Testing::TestResult::Inconclusive,
                       Mezzanine::Testing::Char8ToTestResults('U'));
            TEST_EQUAL("Char8ToTestResults::uIsInconclusive",
                       Mezzanine::Testing::TestResult::Inconclusive,
                       Mezzanine::Testing::Char8ToTestResults('u'));
            TEST_EQUAL("Char8ToTestResults::IIsInconclusive",
                       Mezzanine::Testing::TestResult::Inconclusive,
                       Mezzanine::Testing::Char8ToTestResults('I'));
            TEST_EQUAL("Char8ToTestResults::iIsInconclusive",
                       Mezzanine::Testing::TestResult::Inconclusive,
                       Mezzanine::Testing::Char8ToTestResults('i'));

            TEST_EQUAL("Char8ToTestResults::QIsUnknown",
                       Mezzanine::Testing::TestResult::Unknown,
                       Mezzanine::Testing::Char8ToTestResults('Q'));
            TEST_EQUAL("Char8ToTestResults::+IsUnknown",
                       Mezzanine::Testing::TestResult::Unknown,
                       Mezzanine::Testing::Char8ToTestResults('+'));
            TEST_EQUAL("Char8ToTestResults::~IsUnknown",
                       Mezzanine::Testing::TestResult::Unknown,
                       Mezzanine::Testing::Char8ToTestResults('~'));
            TEST_EQUAL("Char8ToTestResults::TabIsUnknown",
                       Mezzanine::Testing::TestResult::Unknown,
                       Mezzanine::Testing::Char8ToTestResults('\t'));
            TEST_EQUAL("Char8ToTestResults::NullTerminatorIsUnknown",
                       Mezzanine::Testing::TestResult::Unknown,
                       Mezzanine::Testing::Char8ToTestResults('\0'));

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
            FakeTestGroup["consolelogic"] = &ConsoleLogicInstance;

            TEST_STRING_CONTAINS("Usage::Usage",
                                 Mezzanine::String("Usage"),
                                 Mezzanine::Testing::GetUsageString("Tester", FakeTestGroup));
            TEST_STRING_CONTAINS("Usage::HasConsoleLogic",
                                 Mezzanine::String("consolelogic"),
                                 Mezzanine::Testing::GetUsageString("Tester", FakeTestGroup));
            TEST_STRING_CONTAINS("GetPrintableTestList::HasConsoleLogic",
                                 Mezzanine::String("consolelogic"),
                                 Mezzanine::Testing::GetPrintableTestList(80, FakeTestGroup));

            // Lets test usage, because we can
            {
                Mezzanine::Testing::OutputBufferGuard CerrGuard(std::cerr);

                TEST_EQUAL("Usage::ReturnFailureCode",
                           static_cast<int>(Mezzanine::Testing::ExitInvalidArguments),
                           Mezzanine::Testing::Usage("Foo",FakeTestGroup));

                TEST_STRING_CONTAINS("Usage::EmitsNameToCerr",
                                     Mezzanine::String("Foo"),
                                     CerrGuard.GetSunkOutput());

            }
        }
        bool HasAutomaticTests() const override
            { return true; }


        //void RunSubprocessTest(const Mezzanine::String& Arg) // Add this parameter back in if you need it.
        void RunSubprocessTest(const Mezzanine::String&) override
        {
            // Tests with a high risk of crashing can go here

            // If you ever need to debug stuff in here be sure to pass "debugtests" to test executable

            // To enable this test group have HasSubprocessTest() return true
        }
        bool HasSubprocessTest() const override
            { return false; }


        void RunInteractiveTests() override
        {
            // Tests that require the mushy and weak flesh of human (Until our machine overlords rise up).

            // To enable this test group have HasInteractiveTests() return true
        }
        bool HasInteractiveTests() const override
            { return false; }


};

RESTORE_WARNING_STATE
#endif

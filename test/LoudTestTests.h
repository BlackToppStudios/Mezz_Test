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
#ifndef Mezz_Test_LoudTests_h
#define Mezz_Test_LoudTests_h

/// @file
/// @brief Header for unit tests for the testing framework, but only the time sensitive parts

#include "MezzTest.h"

#include <stdexcept>
#include <thread>


/// @brief TestTests to verify that Warnings works correctly and emit messages.
/// @details This class is not called directly by the Unit Test framework and is just used by LoudTestTests
AUTOMATIC_TEST_GROUP(WarningLoudTestTests, LoudWarnTests)
{
    // Here are some examples of test that should warn and do so with loud output
    TEST_TIMED("TestTimedWarning", std::chrono::microseconds(5000), std::chrono::microseconds(1000), []{});
    TEST_TIMED_UNDER("TestTimedUnderWarning", std::chrono::microseconds(1),
               []{ std::this_thread::sleep_for( std::chrono::milliseconds(5) ); });
}

/// @brief TestTests to verify that Failure works correctly and emit.
/// @details This class is not called directly by the Unit Test framework and is just used by LoudTestTests
AUTOMATIC_TEST_GROUP(NegativeLoudTestTests, LoudFailTests)
{
    // This group should serve as examples of failing tests and do so with loud output.
    TEST("DefaultTestFailing", false);
    TEST_EQUAL("EqualityTestFailing", 1, 2);
    TEST_EQUAL_EPSILON("EqualEpsilonFailing", 0.1, 0.2);
    TEST_EQUAL_MULTI_EPSILON("EqualMultiEpsilonFailing", 0.1, 1.2, 2);
    TEST_RESULT("TestResultFailing", Mezzanine::Testing::TestResult::Failed);
    TEST_THROW("TestThrowFailing", std::invalid_argument, []{ throw std::out_of_range("pass"); });
    TEST_THROW("TestThrowFailingNonException", std::invalid_argument, []{ throw std::string("pass"); });
    TEST_NO_THROW("TestNoThrowFailing", []{ throw std::invalid_argument("Fail"); });
    TEST_NO_THROW("TestNoThrowFailingNonException", []{ throw std::string("Fail"); });
    TEST_STRING_CONTAINS("TestStringContainsFailing", Mezzanine::String("Foo"), Mezzanine::String("Fubar"));
}

/// @brief This is the actual Test class. This tests our Test Macros that are time sensitive.
BENCHMARK_THREAD_TEST_GROUP(LoudTestTests, LoudTests)
{
    TestLog << "--=================-- --=================--\nNow some tests that print failure messages in Success\n";

    // Warning Tests
    class WarningLoudTestTests Warnifier;
    Warnifier();
    Mezzanine::String WarnifierOutput(Warnifier.GetTestLog());
    TestLog << WarnifierOutput << '\n';
    TEST("WarningsWereLoud", WarnifierOutput.empty() == false);
    for(const Mezzanine::Testing::TestData& SingleResult : Warnifier)
        { TEST_EQUAL(SingleResult.TestName, Mezzanine::Testing::TestResult::Warning, SingleResult.Results); }

    // Failing Tests
    class NegativeLoudTestTests Negation;
    Negation();
    Mezzanine::String NegationOutput(Negation.GetTestLog());
    TestLog << NegationOutput << '\n';
    TEST("FailuresWereLoud", NegationOutput.empty() == false);
    for(const Mezzanine::Testing::TestData& SingleResult : Negation)
        { TEST_EQUAL(SingleResult.TestName, Mezzanine::Testing::TestResult::Failed, SingleResult.Results); }

    TestLog << "\nEnd of tests that print failure in Success.\n--=================-- --=================--\n";
}

#endif

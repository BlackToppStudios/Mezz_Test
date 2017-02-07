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
#ifndef Mezz_Test_TheActualTests_h
#define Mezz_Test_TheActualTests_h

/// @file
/// @brief Header for unit tests for the testing framework.

// Add other headers you need here
#include "MezzTest.h"

#include <stdexcept>
#include <thread>

SAVE_WARNING_STATE
SUPPRESS_CLANG_WARNING("-Wweak-vtables") // We really don't care, because this will this will be recompiled each test.
SUPPRESS_CLANG_WARNING("-Wpadded") // Test classes will likely need to be padded and we don't care.

// This class is not called directly by the Unit Test framework and is just used by
// TestTests to verify that Failing works correctly. Every test here should fail.
class NegativeTestTests : public Mezzanine::Testing::UnitTestGroup
{
    public:
        NegativeTestTests() = default;
        virtual ~NegativeTestTests() = default;
        virtual Mezzanine::String Name() override
            { return "negativetest"; }

        void RunAutomaticTests() override
        {
            // This group should serve as examples of failing tests.
            TEST("DefaultTestFailing", false);
            TEST_EQUAL("EqualityTestFailing", 1, 2);
            TEST_EQUAL_EPSILON("EqualEpsilonFailing", 0.1, 0.2);
            TEST_EQUAL_MULTI_EPSILON("EqualMultiEpsilonFailing", 0.1, 1.2, 2);
            TEST_RESULT("TestResultFailing", Mezzanine::Testing::TestResult::Failed);
            TEST_THROW("TestThrowFailing", std::invalid_argument, []{ throw std::out_of_range("pass"); });
            TEST_NO_THROW("TestNoThrowFailing", []{ throw std::invalid_argument("Fail"); });
            TEST_STRING_CONTAINS("TestStringContainsFailing", Mezzanine::String("Foo"), Mezzanine::String("Fubar"));
        }
        bool HasAutomaticTests() const override
            { return true; }
};

// This class is not called directly by the Unit Test framework and is just used by
// TestTests to verify that Warnings works correctly.
class WarningTestTests : public Mezzanine::Testing::UnitTestGroup
{
    public:
        WarningTestTests() = default;
        virtual ~WarningTestTests() = default;

        // This is used as the name of the test on the command prompt
        virtual Mezzanine::String Name() override
            { return "test"; }

        void RunAutomaticTests() override
        {
            // Here are some examples of test that should warn.
            TEST_WARN("WarningTestWarning", false);
            TEST_RESULT("TestResultWarning", Mezzanine::Testing::TestResult::Warning);
            TEST_TIMED("TestTimedWarning", std::chrono::microseconds(5000), std::chrono::microseconds(1000), []{});
            TEST_TIMED_UNDER("TestTimedUnderWarning", std::chrono::microseconds(1),
                       []{ std::this_thread::sleep_for( std::chrono::milliseconds(5) ); });
        }
        bool HasAutomaticTests() const override
            { return true; }
};

class TestTests : public Mezzanine::Testing::UnitTestGroup
{
    public:
        TestTests() = default;
        virtual ~TestTests() = default;

        // This is used as the name of the test on the command prompt
        virtual Mezzanine::String Name() override
            { return "test"; }

        void RunAutomaticTests() override
        {
            // Positive tests This should serve as examples for how to use this and get tests that passed.
            TEST("DefaultTestPassing", true);
            TEST_EQUAL("EqualityTestPassing", 1, 1);
            TEST_WARN("WarningTestPassing", true);
            TEST_EQUAL_EPSILON("EqualEpsilonPassing", 0.1, 0.1);
            TEST_EQUAL_MULTI_EPSILON("EqualMultiEpsilonPassing", 0.1, 0.1*1.00, 2);
            TEST_RESULT("TestResultPassing", Mezzanine::Testing::TestResult::Success);
            TEST_THROW("TestThrowPassing", std::invalid_argument, []{ throw std::invalid_argument("pass"); });
            TEST_NO_THROW("TestNoThrowPassing", []{});
            TEST_STRING_CONTAINS("TestStringContains", Mezzanine::String("Foo"), Mezzanine::String("Foobar"));

            #ifdef MEZZ_Windows
                TEST_TIMED("TestTimedPassing", std::chrono::microseconds(200000), std::chrono::microseconds(60000),
                       []{ std::this_thread::sleep_for( std::chrono::milliseconds(200) ); });
            #else
                TEST_TIMED("TestTimedPassing", std::chrono::microseconds(5000), std::chrono::microseconds(5000),
                       []{ std::this_thread::sleep_for( std::chrono::milliseconds(5) ); });
            #endif

            TEST_TIMED_UNDER("TestTimedUnderPassing", std::chrono::microseconds(5000), []{ });

            // AddTestResult is the function that runs all these test macros. This should verify that calling those
            // macros twice with the same name fails
            TEST_THROW("TwoIdenticalTestNamesFail", std::runtime_error,
                [this]{
                    AddTestResult( Mezzanine::Testing::TestData( ("TwoIdenticalTestNamesFailImpl"),
                                                                 (Mezzanine::Testing::TestResult::Success),
                                                                 "RunAutomaticTests", __FILE__, __LINE__));
                    AddTestResult( Mezzanine::Testing::TestData( ("TwoIdenticalTestNamesFailImpl"),
                                                                 (Mezzanine::Testing::TestResult::Success),
                                                                 "RunAutomaticTests", __FILE__, __LINE__));
                }
            );

            // From here down we are running the results of the other test result groups and assembling their results
            // if we let the normal machinery of the unit test framework do this they would fail the tests, because
            // these are supposed to fail or do other interesting things.

            // Failing Tests
            class NegativeTestTests Negation;
            Negation.RunAutomaticTests();
            for(const Mezzanine::Testing::TestData& SingleResult : Negation)
                { TEST_EQUAL(SingleResult.TestName, Mezzanine::Testing::TestResult::Failed, SingleResult.Results); }

            // Warning Tests
            class WarningTestTests Warnifier;
            Warnifier.RunAutomaticTests();
            for(const Mezzanine::Testing::TestData& SingleResult : Warnifier)
                { TEST_EQUAL(SingleResult.TestName, Mezzanine::Testing::TestResult::Warning, SingleResult.Results); }

        }
        bool HasAutomaticTests() const override
            { return true; }


        //void RunSubprocessTest(const Mezzanine::String& Arg) // Add this parameter back in if you need it.
        void RunSubprocessTest(const Mezzanine::String&) override
        {
            TEST("IsolatedSubProcessTestPassing", true);
        }
        bool HasSubprocessTest() const override
            { return true; }


        void RunInteractiveTests() override
            {}
        bool HasInteractiveTests() const override
            { return false; }
};

RESTORE_WARNING_STATE
#endif

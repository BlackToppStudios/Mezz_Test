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

#include "MezzTest.h"

#include <stdexcept>
#include <thread>

SAVE_WARNING_STATE
SUPPRESS_VC_WARNING(4625) // BS about implicit copy constructors, despite explicit deletion in parent class.
SUPPRESS_VC_WARNING(5026) // more BS about move constructors implicitly removed
SUPPRESS_VC_WARNING(5027) // Why are there garbage warnings like these three even in vs?
                          // Any time you don't use the test group macros you might need to handle VS warnings.


// This class is not called directly by the Unit Test framework and is just used by
// TestTests to verify that Failing works correctly. Every test here should fail.
class MEZZ_LIB NegativeTestTests : public Mezzanine::Testing::AutomaticTestGroup
{
     public:
        virtual void operator ()() override;
        virtual Mezzanine::String Name() const override
            { return "NegativeTests"; }

        /// @brief Don't print Failures that are supposed to happen.
        virtual Mezzanine::Boole EmitIntermediaryTestResults() const override
            { return false; }
};

void NegativeTestTests::operator ()()
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

// This class is not called directly by the Unit Test framework and is just used by
/// @brief TestTests to verify that Warnings works correctly.
class MEZZ_LIB WarningTestTests : public Mezzanine::Testing::AutomaticTestGroup
{
    public:
        virtual void operator()() override;
        virtual Mezzanine::String Name() const override
            { return "WarningTests"; }

        /// @brief Don't print Warnings that are supposed to happen.
        virtual Mezzanine::Boole EmitIntermediaryTestResults() const override
            { return false; }
};

void WarningTestTests::operator ()()
{
    // Here are some examples of test that should warn.
    TEST_WARN("WarningTestWarning", false);
    TEST_RESULT("TestResultWarning", Mezzanine::Testing::TestResult::Warning);
}

/// @brief This is the actual Test class. This tests our Test Macros.
class MEZZ_LIB TestTests : public Mezzanine::Testing::AutomaticTestGroup
{
    public:
        virtual void operator()() override;
        virtual Mezzanine::String Name() const override
            { return "Tests"; }
};

void TestTests::operator ()()
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

    // Failing Tests
    class NegativeTestTests Negation;
    Negation();
    for(const Mezzanine::Testing::TestData& SingleResult : Negation)
        { TEST_EQUAL(SingleResult.TestName, Mezzanine::Testing::TestResult::Failed, SingleResult.Results); }
    TEST_EQUAL("GetWorstShouldReturnFailure", Mezzanine::Testing::TestResult::Failed, Negation.GetWorstResults());

    // Warning Tests
    class WarningTestTests Warnifier;
    Warnifier();
    for(const Mezzanine::Testing::TestData& SingleResult : Warnifier)
        { TEST_EQUAL(SingleResult.TestName, Mezzanine::Testing::TestResult::Warning, SingleResult.Results); }
    TEST_EQUAL("GetWorstShouldReturnWarning", Mezzanine::Testing::TestResult::Warning, Warnifier.GetWorstResults());

    int TestCount = 0;
    int ConstTestCount = 0;
    for(UnitTestGroup::iterator iter = Warnifier.begin(); iter != Warnifier.end(); iter++)
        { TestCount++; }
    for(UnitTestGroup::const_iterator iter = Warnifier.cbegin(); iter != Warnifier.cend(); iter++)
        { ConstTestCount++; }

    TEST_EQUAL("ConstAndNormalIterationOverTestGroupSame", TestCount, ConstTestCount);
}

RESTORE_WARNING_STATE

#endif

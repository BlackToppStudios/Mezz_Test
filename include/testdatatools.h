// © Copyright 2010 - 2016 BlackTopp Studios Inc.
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
#ifndef Mezz_Test_testdatatools_h
#define Mezz_Test_testdatatools_h

/// @file
/// @brief TestData, TestDataStorage and UnitTestGroup class definitions.

#include "datatypes.h"

#include "testdata.h"

#include <limits>

namespace Mezzanine
{
    namespace Testing
    {
        #ifndef TEST
            /// @def TEST
            /// @brief The easiest way to add a test to the currently running UnitTestGroup.
            /// This captures test location meta data and should be considered the default way to record tests
            /// @note This calls a member function on the UnitTestGroup class, so it can only be used in UnitTestGroup
            /// Functions like UnitTestGroup::RunInteractiveTests or UnitTestGroup::RunAutomaticTests.
            /// @param Name The name of the current test
            /// @param Conditional A boolean operand of some kind
            #ifdef __FUNCTION__
                #define TEST(Name, Conditional);                                                                       \
                    Test((Name), (Conditional),                                                                        \
                         Mezzanine::Testing::TestResult::Failed, Mezzanine::Testing::TestResult::Success,              \
                         __FUNCTION__, __FILE__, __LINE__ );
            #else
                #define TEST(Name, Conditional);                                                                       \
                    Test((Name), (Conditional),                                                                        \
                         Mezzanine::Testing::TestResult::Failed, Mezzanine::Testing::TestResult::Success,              \
                         __func__, __FILE__, __LINE__ );
            #endif
        #endif

        #ifndef TEST_EQUAL
            /// @def TEST_EQUAL
            /// @brief The simplest test for comparing two values in a currently running UnitTestGroup.
            /// This captures test location meta data and should be considered the default way to record equality tests.
            /// This uses the types == operator and if inequal uses the types << operator(ostream&) to emit an error
            /// message displaying the received results and the actual results.
            /// @note This calls a member function on the UnitTestGroup class, so it can only be used in UnitTestGroup
            /// Functions like UnitTestGroup::RunInteractiveTests or UnitTestGroup::RunAutomaticTests.
            /// @param Name The name of the current test.
            /// @param ExpectedResults The canonical valute
            /// @param ActualResults
            #ifdef __FUNCTION__
                #define TEST_EQUAL(Name, ExpectedResults, ActualResults);                                              \
                    TestEqual((Name), (ExpectedResults), (ActualResults),                                              \
                              Mezzanine::Testing::TestResult::Failed, Mezzanine::Testing::TestResult::Success,         \
                              __FUNCTION__, __FILE__, __LINE__ );
            #else
                #define TEST_EQUAL(Name, ExpectedResults, ActualResults);                                              \
                    TestEqual((Name), (ExpectedResults), (ActualResults),                                              \
                              Mezzanine::Testing::TestResult::Failed, Mezzanine::Testing::TestResult::Success,         \
                              __func__, __FILE__, __LINE__ );
            #endif
        #endif

        #ifndef TEST_EQUAL_EPSILON
            /// @def TEST_EQUAL_EPSILON
            /// @brief Compare types that might flucatuate because of the impreciion of floating point values.
            /// @details
            /// @param Name The name of the current test.
            /// @param ExpectedResults The amount expected, plus or minus the std::numerics_limits epsilon of the type.
            /// @param ActualResults The tested amount that the test process actually produced.
            #ifdef __FUNCTION__
                #define TEST_EQUAL_EPSILON(Name, ExpectedResults, ActualResults);                                      \
                TestEqualEpsilon((Name), (ExpectedResults), (ActualResults), 1,                                        \
                                 Mezzanine::Testing::TestResult::Failed, Mezzanine::Testing::TestResult::Success,      \
                                 __FUNCTION__, __FILE__, __LINE__ );
            #else
                #define TEST_EQUAL_EPSILON(Name, ExpectedResults, ActualResults);                                      \
                TestEqualEpsilon((Name), (ExpectedResults), (ActualResults), 1,                                        \
                                 Mezzanine::Testing::TestResult::Failed, Mezzanine::Testing::TestResult::Success,      \
                                 __func__, __FILE__, __LINE__ );
            #endif
        #endif

        #ifndef TEST_EQUAL_MULTI_EPSILON
            /// @def TEST_EQUAL_MULTI_EPSILON
            /// @details This is only rarely required. TEST_EQUAL_EPSILON should be preferred as this can spuriously
            /// pass.
            /// @param Name The name of the current test
            /// @param ExpectedResults The amount expected, plus or minus the std::numerics_limits epsilon of the type.
            /// @param ActualResults The tested amount that the test process actually produced.
            /// @param EpsilonFactor How many times rounding could occur that could round to the epsilon, so that it
            /// can be accounted for.
            #ifdef __FUNCTION__
                #define TEST_EQUAL_MULTI_EPSILON(Name, ExpectedResults, ActualResults, EpsilonFactor);                 \
                TestEqualEpsilon((Name), (ExpectedResults), (ActualResults), EpsilonFactor,                            \
                                 Mezzanine::Testing::TestResult::Failed, Mezzanine::Testing::TestResult::Success,      \
                                 __FUNCTION__, __FILE__, __LINE__ );
            #else
                #define TEST_EQUAL_MULTI_EPSILON(Name, ExpectedResults, ActualResults, EpsilonFactor);                 \
                TestEqualEpsilon((Name), (ExpectedResults), (ActualResults), EpsilonFactor,                            \
                                 Mezzanine::Testing::TestResult::Failed, Mezzanine::Testing::TestResult::Success,      \
                                 __func__, __FILE__, __LINE__ );
            #endif
        #endif

        #ifndef TEST_WARN
            /// @def TEST_WARN
            /// @brief Just like TEST but if the test fails only a warning is added.
            /// This captures test location meta data and should be considered the default way to record tests that warn
            /// instead of failing.
            /// @note This calls a member function on the UnitTestGroup class, so it can only be used in UnitTestGroup
            /// Functions, like UnitTestGroup::RunInteractiveTests or UnitTestGroup::RunAutomaticTests
            /// @param Cond A boolean operand of some kind
            /// @param Name The name of the current test
            #ifdef __FUNCTION__
                #define TEST_WARN(Name, Conditional)                                                                   \
                        Test( (Name), (Conditional),                                                                   \
                            Mezzanine::Testing::TestResult::Warning, Mezzanine::Testing::TestResult::Success,          \
                            __FUNCTION__, __FILE__, __LINE__ );
            #else
                #define TEST_WARN(Name, Conditional)                                                                   \
                        Test( (Name), (Conditional),                                                                   \
                            Mezzanine::Testing::TestResult::Warning, Mezzanine::Testing::TestResult::Success,          \
                            __func__, __FILE__, __LINE__ );
            #endif
        #endif

        #ifndef TEST_RESULT
            /// @def TEST_RESULT
            /// @brief An easy way to add a test and associated data to the currently running UnitTestGroup
            /// This captures test location meta data and should be considered a good way to record tests that do not
            /// easily break down to a single conditional.
            /// @note This calls a member function on the UnitTestGroup class, so it can only be used in UnitTestGroup
            /// Functions like UnitTestGroup::RunInteractiveTests or UnitTestGroup::RunAutomaticTests
            /// @param ExistingResult A TestResult To be added directy
            /// @param Name The name of the current test
            #ifdef __FUNCTION__
                #define TEST_RESULT(Name, ExistingResult)                                                              \
                        AddTestResult( Mezzanine::Testing::TestData( (Name), (ExistingResult),                         \
                        __FUNCTION__, __FILE__, __LINE__)) ;
            #else
                #define TEST_RESULT(Name, ExistingResult)                                                              \
                        AddTestResult( Mezzanine::Testing::TestData( (Name), (ExistingResult),                         \
                        __func__, __FILE__, __LINE__)) ;
            #endif

        #endif

        #ifndef TEST_THROW
            /// @def TEST_THROW
            /// @brief An easy way to add a test whether or not a function/code snippet throws exceptions (or whatever)
            /// the way planned.
            /// @details This captures test location meta data and should be considered the default way to capture
            /// exception tests.
            /// @note This calls a member function on the UnitTestGroup class, so it can only be used in UnitTestGroup
            /// Functions like UnitTestGroup::RunInteractiveTests or UnitTestGroup::RunAutomaticTests
            /// @param ExpectThrown The type of the thing that should be thrown
            /// @param CodeThatThrows A lambda or other callable code that throws an exception.
            /// @param Name The name of the current test
            #ifdef __FUNCTION__
                #define TEST_THROW(Name, ExpectThrown, CodeThatThrows);                                                \
                    TestThrow<ExpectThrown> ((Name), (CodeThatThrows),                                                 \
                         Mezzanine::Testing::TestResult::Failed, Mezzanine::Testing::TestResult::Success,              \
                         __FUNCTION__, __FILE__, __LINE__ );
            #else
                #define TEST_THROW(Name, ExpectThrown, CodeThatThrows);                                                \
                    TestThrow<ExpectThrown> ((Name), (CodeThatThrows),                                                 \
                         Mezzanine::Testing::TestResult::Failed, Mezzanine::Testing::TestResult::Success,              \
                         __func__, __FILE__, __LINE__ );
            #endif
        #endif

        #ifndef TEST_NO_THROW
            /// @def TEST_THROW
            /// @brief An easy way to add a test whether or not a function/code snippet does not throw exceptions (or
            /// whatever) the way planned.
            /// @details This captures test location meta data and should be considered the default way to capture
            /// exception tests.
            /// @note This calls a member function on the UnitTestGroup class, so it can only be used in UnitTestGroup
            /// Functions like UnitTestGroup::RunInteractiveTests or UnitTestGroup::RunAutomaticTests
            /// @param CodeThatMightThrow The type of the thing that should be thrown
            /// @param Name The name of the current test
            #ifdef __FUNCTION___
                #define TEST_NO_THROW(Name, CodeThatThrows);                                                           \
                    TestNoThrow((Name), (CodeThatThrows),                                                              \
                         Mezzanine::Testing::TestResult::Failed, Mezzanine::Testing::TestResult::Success,              \
                         __FUNCTION__, __FILE__, __LINE__ );
                #else
                #define TEST_NO_THROW(Name, CodeThatThrows);                                                           \
                    TestNoThrow((Name), (CodeThatThrows),                                                              \
                         Mezzanine::Testing::TestResult::Failed, Mezzanine::Testing::TestResult::Success,              \
                         __func__, __FILE__, __LINE__ );
            #endif
        #endif

        #ifndef TEST_TIMED
            /// @def TEST_TIMED
            /// @brief An easy way to add a test whether or not a function/code snippet takes approximately the expected
            /// amount of time.
            /// @details This starts a timer just before the CodeToTime is execute and stops that time right after it
            /// finishes.
            /// @note This calls a member function on the UnitTestGroup class, so it can only be used in UnitTestGroup
            /// Functions like UnitTestGroup::RunInteractiveTests or UnitTestGroup::RunAutomaticTests
            /// @param CodeToTime The code to time
            /// @param ExpectedTime The Expected amount if time in microseconds.
            /// @param Variance A fraction 0.00 of how far off, long or short, the execution time can be and still pass.
            /// For example .02 is 2%
            /// @param Name The name of the current test
            #ifdef __FUNCTION__
                #define TEST_TIMED(CodeToTime, ExpectedTime, Variance, Name)                                    \
                {                                                                                               \
                    TimedTest TESTDuration;                                                                     \
                    CodeToTime;                                                                                 \
                    MaxInt TESTLength = TESTDuration.GetLength();                                               \
                    MaxInt TESTTargetTime = ExpectedTime;                                                       \
                    MaxInt TESTVariance = Variance * double(ExpectedTime);                                      \
                    if( MaxInt(TESTTargetTime-TESTVariance) < TESTLength &&                                     \
                        TESTLength < MaxInt(TESTTargetTime+TESTVariance))                                       \
                    { AddTestResult( TestData( (Name), Testing::Success, __FUNCTION__, __FILE__, __LINE__) ); } \
                    else                                                                                        \
                    { AddTestResult( TestData( (Name), Testing::Failed, __FUNCTION__, __FILE__, __LINE__) ); }  \
                }
            #else
                #define TEST_TIMED(CodeToTime, ExpectedTime, Variance, Name)                                \
                {                                                                                           \
                    TimedTest TESTDuration;                                                                 \
                    CodeToTime;                                                                             \
                    MaxInt TESTLength = TESTDuration.GetLength();                                           \
                    MaxInt TESTTargetTime = ExpectedTime;                                                   \
                    MaxInt TESTVariance = Variance * double(ExpectedTime);                                  \
                    if( MaxInt(TESTTargetTime-TESTVariance) < TESTLength &&                                 \
                        TESTLength < MaxInt(TESTTargetTime+TESTVariance))                                   \
                    { AddTestResult( TestData( (Name), Testing::Success, __func__, __FILE__, __LINE__) ); } \
                    else                                                                                    \
                    { AddTestResult( TestData( (Name), Testing::Failed, __func__, __FILE__, __LINE__) ); }  \
                }
            #endif
        #endif
    }// Testing
}// Mezzanine

#endif

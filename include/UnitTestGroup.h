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
#ifndef Mezz_Test_UnitTestGroup_h
#define Mezz_Test_UnitTestGroup_h

/// @file
/// @brief UnitTestGroup class definitions.


#include "TestData.h"
#include "TestEnumerations.h"

#include <vector>
#include <chrono>

namespace Mezzanine
{
    namespace Testing
    {


        //SAVE_WARNING_STATE
        //SUPPRESS_CLANG_WARNING("-Wpadded")
        //RESTORE_WARNING_STATE

        // @brief Print all the groups that exist in a given CoreTestGroup
        // @param TestGroups The group whose constents names with be printed
        // @return ExitSuccess on success.
        //int PrintList(CoreTestGroup &TestGroups);

        SAVE_WARNING_STATE
        SUPPRESS_CLANG_WARNING("-Wpadded") // Testing code is not sensitive to care about 1 byte of padding
                                           // If we ever profile then we should disable this.
        SUPPRESS_CLANG_WARNING("-Wweak-vtables")

        ////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        /// @brief A single group of tests, suitable for being all the tests of a small subsystem or single class.
        class MEZZ_LIB UnitTestGroup
        {
            public:
                /// @brief The type use to store the results of tests, largely used to clarify derived types.
                typedef std::vector<TestData> TestDataStorageType;

            private:
                /// @brief The test macros will all store their data here.
                TestDataStorageType TestDataStorage;

            protected:
                /// @brief A place for each test to send its logs.
                /// @detail This should be strictly preferred to cout because this is thread safe.
                static thread_local std::stringstream TestLog;

            public:
                /// @brief Default virtual deconstructor to allow for inheritance.
                virtual ~UnitTestGroup() = default;

                ////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Test class methods, Every test class must implement these.

                /// @brief Execution operator runs tests in derived classes.
                virtual void operator()() = 0;

                /// @brief Get the name of the test group for command line args and other uesr interaction.
                /// @return Any string that uniquely identifies a test.
                virtual Mezzanine::String Name() const = 0;


                ////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Policy class methods, Test policy classes will implement these. 90% of tests classes ignore these.

                /// @brief Tell Test macros and AddResult whether or not to print immediately when called.
                /// @note If a UnitTestGroup returns true it really shouldn't return true in RequiresSubProcess because
                /// the calling process will not know how to
                /// @return Defaults to true, but can be set to false to prevent printing of intermediary results, that
                /// might not be useful. For example TestTests sets this to false when testing Test macro failures.
                virtual Boole EmitIntermediaryTestResults() const;

                /// @brief Some things must be run in a subprocess all on their own. Override this if you need it.
                /// @return False by default, a test class should override this if it might divide by zero, segfault or
                /// otherwise take down the test suite as a whole.
                virtual Boole RequiresSubProcess() const;

                /// @brief Is this safe to run alongside a trillion other tests?
                /// @details To be MultiThreadSafe a test suite needs to access no singletons, the test suite must
                /// access no globals, must not be time sensitive, must not create an undue number of threads, must not
                /// call any function with a static variable or use a class with static variable unless whatever shared
                /// mutable state somehow has its access synchronized.
                /// @return Defaults to true, anything tinkering with time or other not thread safe should override this
                /// to return false.
                virtual Boole IsMultiThreadSafe() const;

                /// @brief Is it safe to run this test class in parrelel to other tests if done so in a process.
                /// @details If your test grabs a GPU, Sound card or other hardware context, it is probably not
                /// multiprocess safe. If you just spawn a bunch of thread or might segfault this should be safe.
                /// @return defaults to true. All tests that aren't should override this and return false.
                virtual Boole IsMultiProcessSafe() const;

                /// @brief Decides it the test should run as part of a default invocation of the test suite.
                /// @return defaults to true. All tests that are interactive or take an inordinate amount of time should
                /// override this and return false.
                virtual Boole ShouldRunAutomatically() const;

                //////////////////////////////////////////////////////
                // MetaPolicy methods, don't override these, they use the policy methods.

                /// @brief Must this test run while nothing else is running?
                /// @return True if the process is not thread or process safe or
                /// (!IsMultiThreadSafe() && !IsMultiThreadSafe()).
                Boole MustBeSerialized() const;

                /// @brief CAn this be run while other tests are running?
                /// @return True if the process is either thread or process safe or
                /// (IsMultiThreadSafe() || IsMultiThreadSafe()).
                Boole CanBeParallel() const;

                ////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Make all UnitTestGroups look like a container of TestDatas

                /// @brief A mutable iterator type suitable for use
                typedef TestDataStorageType::iterator iterator;
                /// @brief A Immutable iterator type suitable for use
                typedef TestDataStorageType::const_iterator const_iterator;

                /// @return A mutable iterator to the beginning of the TestData.
                iterator begin();
                /// @return A Immutable iterator to the beginning of the TestData.
                const_iterator begin() const;
                /// @return Be absolutely certian to get an Immutable iterator to the beginning of the TestData.
                const_iterator cbegin() const;

                /// @return A mutable iterator to the beginning of the TestData.
                iterator end();
                /// @return A Immutable iterator to the beginning of the TestData.
                const_iterator end() const;
                /// @return Be absolutely certian to get an Immutable iterator to the beginning of the TestData.
                const_iterator cend() const;

                /// @brief If you have exactly the name you want in the report, use this the add the TestData.
                /// @param CurrentTest The New test results.
                void AddTestResultWithoutName(TestData&& CurrentTest);

                /// @brief Use this to add test results to the final report.
                /// @param CurrentTest The New test results.
                void AddTestResult(TestData&& CurrentTest);

                ////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Other useful stuff.

                /// @return Scan every test and find the worst result among all test data.
                TestResult GetWorstResults() const;

                Mezzanine::String GetTestLog() const;

                ////////////////////////////////////////////////////////////////////////////////////////////////////////
                // Test Macro Functions Backing

                /// @brief Interpret Boolean value as a test result. Also Prepends the name of the current test, as
                /// returned by Name() + "::", to ease test scoping.
                /// @warning IfFalse comes first in the argument list, This is because the common test cases have
                /// IfTrue = Testing::Success while IfFalse makes sense as other things
                /// @param TestCondition The test itself or the results of it.
                /// @param TestName The comple name of the test, used to track results.
                /// @param IfFalse Defaults to Testing::Failed but can be whatever Testing::TestResult you want if a
                /// false passed as the TestCondition.
                /// @param IfTrue Defaults to Testing::Success but can be whatever Testing::TestResult you want if a
                /// true passed as the TestCondition.
                /// @param FuncName The function the test was called from, if blank
                /// @param File To make tracking down failures easier the file name of the test can
                /// be passed in, if not set an empty string is used
                /// @param Line To make tracking down failures easier the line number of the test
                /// can  be passed in, if not set an empty string is used.
                /// @return A TestResult containing the actual test result.
                virtual TestResult Test(const String& TestName,
                                        bool TestCondition,
                                        TestResult IfFalse = Testing::TestResult::Failed,
                                        TestResult IfTrue = Testing::TestResult::Success,
                                        const String& FuncName = "",
                                        const String& File = "",
                                        Mezzanine::Whole Line = 0);

                /// @copydoc Test
                /// @brief Test if an expected value and given value are equal.
                /// @param ExpectedResults The amount that the ActualResults must have to pass the test.
                /// @param ActualResults The actual results produced by the testing process.
                /// @tparam ExpectedResultsType A type that supports comparison with the ActualResultsType on the left
                /// of the == operator and output to ostreams.
                /// @tparam ActualResultsType A type that supports comparison with the ExpectedResultsType on the right
                /// of the == operator and output to ostreams.
                template <typename ExpectedResultsType, typename ActualResultsType>
                void TestEqual(const String& TestName,
                               ExpectedResultsType ExpectedResults,
                               ActualResultsType ActualResults,
                               TestResult IfFalse = Testing::TestResult::Failed,
                               TestResult IfTrue = Testing::TestResult::Success,
                               const String& FuncName = "",
                               const String& File = "",
                               Mezzanine::Whole Line = 0)
                {
                    TestResult Result = Test( TestName, (ExpectedResults == ActualResults),
                        IfFalse, IfTrue, FuncName, File, Line);
                    if(EmitIntermediaryTestResults() && Mezzanine::Testing::TestResult::Success != Result)
                    {
                        TestLog << "Test - " << TestName << " failed: "
                                << "Expected '" << ExpectedResults << "' "
                                << "but actually Received '" << ActualResults << "'."
                                << std::endl;
                    }
                 }

                /// @copydoc TestEqual
                /// @brief Test if an expected value and given value are equal or so close with floating point values
                /// as to not matter.
                /// @param EpsilonCount How many multiples of the epsilon should the comparison allowed to be off? A
                /// could rule of thumb is one per floating point operation that could introduce rounding unless you
                /// have some other reason for tighter tolerances.
                /// @tparam ExpectedResultsType A type that supports comparison with the ActualResultsType on the left
                /// and right of the <= operator with ActualResultsType. ExpectedResultsType must output to ostreams
                /// via operator <<. This needs to have a specialization for std::numberic_limits<>::epsilon()
                /// implemented and that epsilon must be of the same type as or a similar enough type that it meets all
                /// these criteria as well (at this point you should just read the template, its not that complex and
                /// would be worth your time if your type is complex enought that its epsilon is a different type).
                /// ExpectedResultsType results type must also support multiplication with a
                /// PreciseReal which is normally the largest and most precise primitive floating point type. @n @n
                /// So pretty much it needs to be a completely fleshed out or a completely primite floating type.
                /// @tparam ActualResultsType A type that supports comparison with the ExpectedResultsType on the right
                /// and left <= operator with ExpectedResultsType. ActualResultsType output to ostreams.
                template <typename ExpectedResultsType, typename ActualResultsType>
                void TestEqualEpsilon(const String& TestName,
                                      ExpectedResultsType ExpectedResults,
                                      ActualResultsType ActualResults,
                                      Mezzanine::UInt32 EpsilonCount,
                                      TestResult IfFalse = Testing::TestResult::Failed,
                                      TestResult IfTrue = Testing::TestResult::Success,
                                      const String& FuncName = "",
                                      const String& File = "",
                                      Mezzanine::Whole Line = 0)
                {
                    auto Epsilon(std::numeric_limits<ExpectedResultsType>::epsilon());
                    Boole Within{ (ExpectedResults-Epsilon*PreciseReal(EpsilonCount)) <= ActualResults &&
                                            (ActualResults <= ExpectedResults+Epsilon*PreciseReal(EpsilonCount)) };
                    TestResult Result = Test( TestName, Within, IfFalse, IfTrue, FuncName, File, Line);
                    if(EmitIntermediaryTestResults() && Mezzanine::Testing::TestResult::Success != Result)
                    {
                        TestLog << "Test - " << TestName << " failed: "
                                << "Expected '" << ExpectedResults << "' "
                                << "but actually Received '" << ActualResults << "'."
                                << std::endl;
                    }
                }

                /// @copydoc Test
                /// @brief Test that a given piece of code throws the expected kind of exception.
                /// @param TestCallable A lambda or functor to call that ought to throw an exception.
                /// @tparam ExceptionType The type of the exception to catch for a passing test.
                template<typename ExceptionType>
                void TestThrow(const String& TestName,
                               std::function<void()> TestCallable,
                               TestResult IfFalse = Testing::TestResult::Failed,
                               TestResult IfTrue = Testing::TestResult::Success,
                               const String& FuncName = "",
                               const String& File = "",
                               Mezzanine::Whole Line = 0)
                {
                    Boole Passed{false};
                    try
                        { TestCallable(); }
                    catch (const ExceptionType&)
                        { Passed = true; }
                    catch (const std::exception& e)
                    {
                        if(EmitIntermediaryTestResults())
                            { TestLog << "Caught Unexpected Exception: " << e.what() << std::endl; }
                    }
                    catch (...)
                    {
                        if(EmitIntermediaryTestResults())
                            { TestLog << "Caught Unexpected Exception not derived of std::expection." << std::endl; }
                    }
                    Test(TestName, Passed, IfFalse, IfTrue, FuncName, File, Line);
                }

                /// @copydoc Test
                /// @brief Test that a given piece of code does not throw.
                /// @param TestCallable A lambda or functor to call that should not throw an exception.
                void TestNoThrow(const String& TestName,
                                 std::function<void()> TestCallable,
                                 TestResult IfFalse = Testing::TestResult::Failed,
                                 TestResult IfTrue = Testing::TestResult::Success,
                                 const String& FuncName = "",
                                 const String& File = "",
                                 Mezzanine::Whole Line = 0);

                /// @copydoc Test
                /// @brief Tests that a thing takes a specific amount of time
                /// @param Expected The amount of microseconds this should take.
                /// @param MaxVariance How many microseconds high or low is this allowed to be.
                /// @param TestCallable A lambda or functor to call that should have predictable performance.
                void TestTimed(const String& TestName,
                               std::chrono::microseconds Expected,
                               std::chrono::microseconds MaxVariance,
                               std::function<void()> TestCallable,
                               TestResult IfFalse = Testing::TestResult::Failed,
                               TestResult IfTrue = Testing::TestResult::Success,
                               const String& FuncName = "",
                               const String& File = "",
                               Mezzanine::Whole Line = 0);

                /// @copydoc Test
                /// @brief Tests that a thing takes under a given amount of time.
                /// @details For this to make sense (in most situations) the work actually needs to be done twice. Once
                /// under some old algorithm and once under some new algorithm. Then you can see if your algorithm that
                /// you worked to optimize stay optimized on new machines.
                /// @n @n
                /// Though less common this might make sense if trying to execute under some hard deadline.
                /// @param TestCallable A lambda or functor to call that should have predictable performance.
                /// @param MaxAcceptable The amount of microseconds this should take less than.
                void TestTimedUnder(const String& TestName,
                                    std::chrono::microseconds MaxAcceptable,
                                    std::function<void()> TestCallable,
                                    TestResult IfFalse = Testing::TestResult::Failed,
                                    TestResult IfTrue = Testing::TestResult::Success,
                                    const String& FuncName = "",
                                    const String& File = "",
                                    Mezzanine::Whole Line = 0);

                /// @copydoc Test
                /// @brief Test that one thing contains the other. Intended for strings.
                /// @param ExpectedNeedle This tests searches for this needle in the ActualHaystack.
                /// @param ActualHaystack The string like container that must container the needle or the test fails.
                /// @tparam ExpectedNeedleType A type that can be passed to the ActualHaystackType#find method and work.
                /// @tparam ActualHaystackType A type that implements a find method and returns ExpectedNeedleType::npos
                /// when the ExpectedNeedleType instance passed is not found.
                template <typename ExpectedNeedleType, typename ActualHaystackType>
                void TestStringContains(const String& TestName,
                                        ExpectedNeedleType ExpectedNeedle,
                                        ActualHaystackType ActualHaystack,
                                        TestResult IfFalse = Testing::TestResult::Failed,
                                        TestResult IfTrue = Testing::TestResult::Success,
                                        const String& FuncName = "",
                                        const String& File = "",
                                        Mezzanine::Whole Line = 0)
                {
                    TestResult Result = Test( TestName,
                                              (ExpectedNeedleType::npos != ActualHaystack.find(ExpectedNeedle)),
                                              IfFalse, IfTrue, FuncName, File, Line);
                    if(EmitIntermediaryTestResults() && Mezzanine::Testing::TestResult::Success != Result)
                    {
                        TestLog << "Test - " << TestName << " failed: "
                                << "Expected to find '" << ExpectedNeedle << "' "
                                << "but haystack was '" << ActualHaystack << "'."
                                << std::endl;
                    }
                }

        };
        RESTORE_WARNING_STATE

        /// @brief Sort a container of TestDatas and search it for the worst test results.
        /// @return Scan every test and find the worst result among all test data.
        TestResult GetWorstResults(const UnitTestGroup::TestDataStorageType& ToSearch);

        /// @brief A group of testnames and the Actual class that implements those test(s).
        typedef std::map<Mezzanine::String, UnitTestGroup*> CoreTestGroup;
        /// @brief The type of things in a CoreTestGroup
        typedef std::pair<Mezzanine::String, UnitTestGroup*> CoreTestGroupEntry;

    }// Testing
}// Mezzanine

#endif

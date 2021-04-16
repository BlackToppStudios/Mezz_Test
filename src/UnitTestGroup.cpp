// © Copyright 2010 - 2021 BlackTopp Studios Inc.
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

/// @file
/// @brief The definition of the string manipulation functions the unit tests use

#include "UnitTestGroup.h"
#include "MezzTest.h"
#include "TimingTools.h"

#include <vector>
#include <iostream>

using std::chrono::microseconds;

namespace Mezzanine
{
    namespace Testing
    {
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Policy class methods, Test policy classes will implement these. 90% of tests classes ignore these.

        Boole UnitTestGroup::EmitIntermediaryTestResults() const
            { return true; }

        Boole UnitTestGroup::IsMultiThreadSafe() const
            { return true; }

        Boole UnitTestGroup::IsMultiProcessSafe() const
            { return true; }

        Boole UnitTestGroup::ShouldRunAutomatically() const
            { return true; }

        //////////////////////////////////////////////////////
        // MetaPolicy methods, don't override these, they use the policy methods.

        Boole UnitTestGroup::MustBeSerialized() const
            { return !IsMultiThreadSafe() && !IsMultiProcessSafe(); }

        Boole UnitTestGroup::CanBeParallel() const
            { return IsMultiThreadSafe() || IsMultiProcessSafe(); }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Make all UnitTestGroups look like a container of TestDatas
        UnitTestGroup::iterator UnitTestGroup::begin()
            { return TestDataStorage.begin(); }

        UnitTestGroup::const_iterator UnitTestGroup::begin() const
            { return cbegin(); }

        UnitTestGroup::const_iterator UnitTestGroup::cbegin() const
            { return TestDataStorage.cbegin(); }

        UnitTestGroup::iterator UnitTestGroup::end()
            { return TestDataStorage.end(); }

        UnitTestGroup::const_iterator UnitTestGroup::end() const
            { return cend(); }

        UnitTestGroup::const_iterator UnitTestGroup::cend() const
            { return TestDataStorage.cend(); }
        
        void UnitTestGroup::AddTestResultWithoutName(TestData&& CurrentTest)
        {
            std::sort(TestDataStorage.begin(), TestDataStorage.end());
            if(std::binary_search(TestDataStorage.begin(), TestDataStorage.end(), CurrentTest))
            {
                throw std::runtime_error("Multiple tests have the same name, but cannot: " + CurrentTest.TestName);
            } else {
                if(EmitIntermediaryTestResults()) { TestLog << CurrentTest; }
                TestDataStorage.emplace_back(CurrentTest);
            }
        }

        void UnitTestGroup::AddTestResult(TestData&& CurrentTest)
        {
            CurrentTest.TestName = Name() + "::" + CurrentTest.TestName;
            AddTestResultWithoutName(std::move(CurrentTest));
        }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Other useful stuff.

        TestResult UnitTestGroup::GetWorstResults() const
            { return Mezzanine::Testing::GetWorstResults(TestDataStorage); }

        String UnitTestGroup::GetTestLog() const
            { return TestLog.str(); }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Test Macro Functions Backing
        TestResult UnitTestGroup::Test(const String& TestName, bool TestCondition,
                                 TestResult IfFalse,
                                 TestResult IfTrue,
                                 const String& FuncName,
                                 const String& File,
                                 Whole Line )
        {
            TestResult Result;
            if(TestCondition)
            {
                Result = IfTrue;
            }else{
                Result = IfFalse;
            }
            AddTestResult( TestData(TestName, Result, FuncName, File, Line) );
            return Result;
        }

        void UnitTestGroup::TestNoThrow(const String& TestName,
                                        std::function<void ()> TestCallable,
                                        TestResult IfFalse, TestResult IfTrue,
                                        const String& FuncName, const String& File, Whole Line)
        {
            Boole Passed{false};
            try
                { TestCallable(); Passed = true;}
            catch (const std::exception& e)
            {
                if(EmitIntermediaryTestResults())
                    { TestLog << "Caught Unexpected Exception: " << e.what() << std::endl; }
            }
            catch (...)
            {
                if(EmitIntermediaryTestResults())
                    { TestLog << "Caught Unexpected Exception not derived from std::expection." << std::endl; }
            }
            Test(TestName, Passed, IfFalse, IfTrue, FuncName, File, Line);
        }

        void UnitTestGroup::TestTimed(const String& TestName,
                                      std::chrono::microseconds Expected,
                                      std::chrono::microseconds MaxVariance,
                                      std::function<void ()> TestCallable,
                                      TestResult IfFalse, TestResult IfTrue,
                                      const String& FuncName, const String& File, Whole Line)
        {
           TestTimer TestDuration;
           TestCallable();
           std::chrono::microseconds TimeTaken{std::chrono::duration_cast<microseconds>(TestDuration.GetLength())};
           Boole Passed{TimeTaken-MaxVariance<Expected && Expected<TimeTaken+MaxVariance};
           TestResult Result{Test(TestName, Passed, IfFalse, IfTrue, FuncName, File, Line)};
           if(EmitIntermediaryTestResults() && Mezzanine::Testing::TestResult::Success != Result)
           {
               TestLog << "Expected test to take " << Expected.count()
                       << "µs with a variance of " << MaxVariance.count()
                       << "µs, but it actually took " << TimeTaken.count() << "µs."<< std::endl;
           }
        }

        void UnitTestGroup::TestTimedUnder(const String& TestName,
                                           std::chrono::microseconds MaxAcceptable,
                                           std::function<void ()> TestCallable,
                                           TestResult IfFalse, TestResult IfTrue,
                                           const String& FuncName, const String& File, Whole Line)
        {
           TestTimer TestDuration;
           TestCallable();
           microseconds TimeTaken{std::chrono::duration_cast<microseconds>(TestDuration.GetLength())};
           TestResult Result{Test(TestName, TimeTaken < MaxAcceptable, IfFalse, IfTrue, FuncName, File, Line)};
           if(EmitIntermediaryTestResults() && Mezzanine::Testing::TestResult::Success != Result)
           {
               TestLog << "Expected test to take under " << MaxAcceptable.count()
                       << "µs, but it actually took " << TimeTaken.count() << "µs."<< std::endl;
           }
        }

        TestResult GetWorstResults(const UnitTestGroup::TestDataStorageType& ToSearch)
        {
            TestResult Highest = TestResult::Success;
            for(const TestData& OneTestData : ToSearch)
            {
                if(OneTestData.Results > Highest)
                    { Highest = OneTestData.Results; }
            }
            return Highest;
        }



    }// Testing
}// Mezzanine

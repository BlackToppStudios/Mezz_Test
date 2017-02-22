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
    {/*
        int PrintList(CoreTestGroup& TestGroups)
        {
            for(CoreTestGroup::iterator Iter=TestGroups.begin(); Iter!=TestGroups.end(); ++Iter)
                { cout << Iter->first << std::endl; }
            return ExitSuccess;
        }


        UnitTestGroup::UnitTestGroup()
            :   TestDataStorage(),
                LongestNameLength(0),
                DoSubProcessTest(false),
                DoAutomaticTest(false),
                DoInteractiveTest(false)
        {}

        UnitTestGroup::UnitTestGroup(const UnitTestGroup& OtherGroup)
            :   TestDataStorage(),
                LongestNameLength(0),
                DoSubProcessTest(OtherGroup.DoSubProcessTest),
                DoAutomaticTest(OtherGroup.DoAutomaticTest),
                DoInteractiveTest(OtherGroup.DoInteractiveTest)
        { insert(OtherGroup.begin(),OtherGroup.end()); }

        void UnitTestGroup::RunTests()
        {
            if(DoSubProcessTest)
                { RunSubprocessTest(GetSubSubProcessArgument()); }
            else
            {
                OutputBufferGuard CoutGuard(std::cout);
                OutputBufferGuard CerrGuard(std::cerr);
                OutputBufferGuard ClogGuard(std::clog);
                LaunchAutomaticTest();
                LaunchInteractiveTest();
            }
        }

        void UnitTestGroup::LaunchAutomaticTest()
        {
            std::cout << std::endl << "<AutomaticTestOutput><![CDATA[" << std::endl;
            std::cerr << std::endl << "<AutomaticTestError><![CDATA[" << std::endl;
            try
            {
                if(DoAutomaticTest)
                    { RunAutomaticTests(); }
                else if(HasAutomaticTests())
                    { AddTestResult( TestData("AutomaticTests",Testing::TestResult::Skipped, "RunTests") );}
            }catch(exception& e){
                std::cerr << "Caught an unhandled exception while doing RunAutomaticTests()." << endl
                          << "Message: " << e.what() << endl;
                AddTestResult( TestData("UnhandledException", Testing::TestResult::Failed) );
            }
            std::cout << std::endl << "]]></AutomaticTestOutput>" << std::endl;
            std::cerr << std::endl << "]]></AutomaticTestError>" << std::endl;
        }

        void UnitTestGroup::LaunchInteractiveTest()
        {
            std::cout << std::endl << "<InteractiveTestOutput><![CDATA[" << std::endl;
            std::cerr << std::endl << "<InteractiveTestError><![CDATA[" << std::endl;
            try
            {
                if(DoInteractiveTest)
                    { RunInteractiveTests(); }
                else if(HasInteractiveTests())
                    { AddTestResult( TestData("InteractiveTests",Testing::TestResult::Skipped, "RunTests") );}
            }catch(exception& e){
                std::cerr << "Caught an unhandled exception while doing RunInteractiveTests()." << endl
                          << "Message: " << e.what() << endl;
                AddTestResult( TestData("UnhandledException", Testing::TestResult::Failed) );
            }
            std::cout << std::endl << "]]></InteractiveTestOutput>" << std::endl;
            std::cerr << std::endl << "]]></InteractiveTestError>" << std::endl;
        }
*/

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Static and Thread local variables for the UnitTestGroup.

        SAVE_WARNING_STATE
        SUPPRESS_CLANG_WARNING("-Wexit-time-destructors")
        SUPPRESS_CLANG_WARNING("-Wglobal-constructors")
        thread_local std::stringstream UnitTestGroup::TestLog;
        RESTORE_WARNING_STATE

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Policy class methods, Test policy classes will implement these. 90% of tests classes ignore these.

        Boole UnitTestGroup::EmitIntermediaryTestResults() const
            { return true; }

        Boole UnitTestGroup::RequiresSubProcess() const
            { return false; }

        Boole UnitTestGroup::IsMultiThreadSafe() const
            { return true; }

        Boole UnitTestGroup::IsMultiProcessSafe() const
            { return true; }

        Boole UnitTestGroup::ShouldRunAutomatically() const
            { return true; }

        //////////////////////////////////////////////////////
        // MetaPolicy methods, don't override these, they use the policy methods.

        Boole UnitTestGroup::MustBeSerialized() const
            { return !IsMultiThreadSafe() && !IsMultiThreadSafe(); }

        Boole UnitTestGroup::CanBeParrale() const
            { return IsMultiThreadSafe() || IsMultiThreadSafe(); }

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

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Other useful stuff.

        TestResult UnitTestGroup::GetWorstResults() const
            { return Mezzanine::Testing::GetWorstResults(TestDataStorage); }

        String UnitTestGroup::GetTestLog() const
            { return TestLog.str(); }

        void UnitTestGroup::AddTestResult(TestData&& CurrentTest)
        {
            CurrentTest.TestName = Name() + "::" + CurrentTest.TestName;
            std::sort(TestDataStorage.begin(), TestDataStorage.end());
            if(std::binary_search(TestDataStorage.begin(), TestDataStorage.end(), CurrentTest))
            {
                throw std::runtime_error("Multiple tests have the same name, but cannot: " + CurrentTest.TestName);
            } else {
                if(EmitIntermediaryTestResults()) { TestLog << CurrentTest; }
                TestDataStorage.emplace_back(CurrentTest);
            }
        }
/*
        void UnitTestGroup::AddTestResult(const Mezzanine::String Fresh, TestResult Meat)
        {
            std::cout << "Noting result of " << this->Name() + "::" + Fresh << " as "
                      << TestResultToString(Meat) << std::endl;
            AddTestResult(TestData(Fresh,Meat));
        }

        const UnitTestGroup& UnitTestGroup::operator+=(const UnitTestGroup& rhs)
        {
            if(rhs.LongestNameLength>LongestNameLength)
                { LongestNameLength=rhs.LongestNameLength; }

            insert(rhs.begin(),rhs.end());
            //std::cout << rhs.TestOutput.str();
            //std::cerr << rhs.TestError.str();
            return *this;
        }

        void UnitTestGroup::AddTestsFromXML(pugi::xml_node Node)
        {
            if(!Node) //Basic Sanity Check
            {
                throw std::invalid_argument( String("UnitTestGroup::AddTestsFromXML can only handle XML but was passed "
                                                    "an empty file. Expected results from '") + Node.name() +"'." );
            }

            if(String("UnitTestGroup")==Node.name())
            {
                for(pugi::xml_node::iterator Iter = Node.begin(); Iter!=Node.end(); Iter++)
                {
                    String CurrentName(Iter->name());
                    if(String("TestData")==CurrentName)
                        { this->AddTestResult(TestData(*Iter)); }
                    else if(String("UnitTestOutput")==CurrentName)
                    {
                        std::cout << std::endl;
                        //Iter->print(TestOutput);
                        std::cout << std::endl;
                    }
                    else if(String("UnitTestError")==CurrentName)
                    {
                        String Text(Iter->text().as_string());
                        if(Text.size()>0)
                           { std::cerr << std::endl << Text << std::endl; }
                    }
                    else
                    {
                        throw std::invalid_argument(
                                String("UnitTestGroup::AddTestsFromXML Invalid subelement passed from ")
                                + Node.name()
                            );
                    }
                }
            }
            else
            {
                throw std::invalid_argument( String("UnitTestGroup::AddTestsFromXML can only handle XML with "
                                "\"UnitTestGroup\" as root element.  Expected results from ") + Node.name() );
            }
        }

        String UnitTestGroup::GetAsXML() const
        {
            String Results("\n<UnitTestGroup>");
            for (iterator Iter=this->begin(); Iter!=this->end(); Iter++)
                { Results += "\n  " + Iter->GetAsXML(); }
            Results += "\n<UnitTestOutput>";
            //Results += this->TestOutput.str();
            Results += "\n</UnitTestOutput>";
            Results += "\n<UnitTestError>";
            //Results += this->TestError.str();
            Results += "\n</UnitTestError>";
            Results += "\n</UnitTestGroup>";
            return Results;
        }

        void UnitTestGroup::DisplayResults(std::ostream& Output,
                                           std::ostream& Error,
                                           bool Summary,
                                           bool FullOutput,
                                           bool HeaderOutput)
        {
            std::vector<unsigned int> TestCounts; // This will store the counts of the Sucesses, failures, etc...
            //Fill with the exact amount of 0s
            TestCounts.insert(TestCounts.end(), 1+TestResultToUnsignedInt(TestResult::NotApplicable), 0);

            if(FullOutput && HeaderOutput) // No point in displaying the header without the other content.
            {
                Mezzanine::String TestName("Test Name");
                Output << std::endl << " " << TestName << MakePadding(TestName, LongestNameLength)
                       << "Result" << std::endl;
            }

            //if(FullOutput)
            //    { Output << TestOutput.str(); }

            for (TestDataStorage::iterator Iter=this->begin(); Iter!=this->end(); Iter++)
            {
                if(FullOutput)
                {
                    Output << Iter->TestName << MakePadding(Iter->TestName, LongestNameLength+1)
                           << TestResultToString(Iter->Results);
                    if(Iter->Results != TestResult::Success)
                    {
                        Output << "\t";
                        if(Iter->FileName.length())
                            { Output << " File: " << Iter->FileName; }
                        if(Iter->FunctionName.length())
                            { Output << " Function: " << Iter->FunctionName; }
                        if(Iter->LineNumber)
                            { Output << " Line: " << Iter->LineNumber; }
                        if(Iter->FileName.length()==0 && Iter->FunctionName.length() == 0 && Iter->LineNumber==0)
                            { Output << " No Metadata available able issue, use TEST to capture"; }
                    }
                    Output << endl;
                    Output.flush();
                }

                if ( (Iter->Results != TestResult::Success) &&
                        Iter->FileName.length() && Iter->FunctionName.length() && Iter->LineNumber)
                {
                    Error << Iter->FileName << ":" << Iter->LineNumber
                          << " Test " << TestResultToString(Iter->Results)
                          << " in function " << Iter->FunctionName << endl;
                    Error.flush();
                }
                TestCounts.at(TestResultToUnsignedInt(Iter->Results))++; // Count this test result
            }

            if(Summary)
            {
                Output << std::endl << " Results Summary:" << std::endl;
                for(unsigned int c=0; c<TestCounts.size();++c)
                {
                    Mezzanine::String ResultName(TestResultToString(IntToTestResult(c)));
                    Output << "  " << ResultName << MakePadding(ResultName,16) << TestCounts.at(c) << std::endl;
                }
                Output << std::endl;
                Output.flush();
            }

            //if(FullOutput && TestError.str().size()>5 ) // Sometimes the copying put "0\r\n" in TestError
            //    { Error << "Errors: " << TestError.str(); }
        }
*/
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

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
#include <stdexcept>
#include <sstream>
#include <cassert>

using namespace Mezzanine;
using namespace std;

namespace Mezzanine
{
    namespace Testing
    {
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

        void UnitTestGroup::RunAutomaticTests()
            {}
        bool UnitTestGroup::HasAutomaticTests() const
            { return false; }
        void UnitTestGroup::ShouldRunAutomaticTests()
            { DoAutomaticTest = true; }

        void UnitTestGroup::RunInteractiveTests()
            {}
        bool UnitTestGroup::HasInteractiveTests() const
            { return false; }
        void UnitTestGroup::ShouldRunInteractiveTests()
            { DoInteractiveTest = true; }

        void UnitTestGroup::RunSubprocessTest(const String&)
            {}
        bool UnitTestGroup::HasSubprocessTest() const
            { return false; }
        void UnitTestGroup::ShouldRunSubProcessTests()
            { DoSubProcessTest = true; }

        Mezzanine::String UnitTestGroup::Name()
            { return ""; }

        void UnitTestGroup::AddTestResult(TestData CurrentTest)
        {
            if(CurrentTest.TestName.npos != CurrentTest.TestName.find(" "))
            {
                throw std::invalid_argument("Invalid Test Name, contains one or more space character ( ), TestName: \""
                                            + CurrentTest.TestName + "\"");
            }
            if(CurrentTest.TestName.npos != CurrentTest.TestName.find("\""))
            {
                throw std::invalid_argument("Invalid Test Name, contains one or more double quote (\") character(s), "
                                            "TestName: \"" + CurrentTest.TestName + "\"");
            }

            if(this->Name().length())
            {
                if(this->Name().npos != this->Name().find(" "))
                {
                    throw std::invalid_argument("Invalid UnitTestGroup Name, contains one or more space character ( ), "
                                                "name: \"" + this->Name() + "\""); }
                if(this->Name().npos != this->Name().find("\""))
                {
                    throw std::invalid_argument("Invalid UnitTestGroup Name, contains one or more double quote (\"), "
                                                "name: \"" + this->Name() + "\""); }
                CurrentTest.TestName = this->Name() + "::" + CurrentTest.TestName;
            }

            TestDataStorage::iterator PreExisting = this->find(CurrentTest.TestName);
            if(this->end()!=PreExisting)
            {
                throw std::runtime_error("Two tests have the exact same name: " +PreExisting->TestName);
            } else {
                this->insert(CurrentTest);
                if(CurrentTest.TestName.length() > LongestNameLength)
                    { LongestNameLength=CurrentTest.TestName.length(); }
            }
        }

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

        TestResult UnitTestGroup::Test(const String& TestName, bool TestCondition,
                                 TestResult IfFalse,
                                 TestResult IfTrue,
                                 const String& FuncName,
                                 const String& File,
                                 Whole Line )
        {
            try
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
            }catch(exception& e){
                std::cerr << "Caught an unhandled exception while adding results for " << TestName << endl
                          << "Message: " << e.what() << endl;
                AddTestResult( TestData("UnhandledException", Testing::TestResult::Failed, FuncName, File, Line) );
                return Testing::TestResult::Failed;
            }
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
                { std::cout << "Caught Unexpected Exception: " << e.what() << std::endl; }
            catch (...)
                { std::cout << "Caught Unexpected Exception not derived from std::expection." << std::endl; }
            Test(TestName, Passed, IfFalse, IfTrue, FuncName, File, Line);
        }

        void UnitTestGroup::TestTimed(const String& TestName,
                                      chrono::microseconds Expected,
                                      chrono::microseconds MaxVariance,
                                      std::function<void ()> TestCallable,
                                      TestResult IfFalse, TestResult IfTrue,
                                      const String& FuncName, const String& File, Whole Line)
        {
           TimedTest TestDuration;
           TestCallable();
           std::chrono::microseconds TimeTaken{TestDuration.GetLength()};
           Boole Passed{TimeTaken-MaxVariance<Expected && Expected<TimeTaken+MaxVariance};
           TestResult Result{Test(TestName, Passed, IfFalse, IfTrue, FuncName, File, Line)};
           if(Mezzanine::Testing::TestResult::Success != Result)
           {
               std::cout << "Expected test to take " << Expected.count()
                         << "µs with a variance of " << MaxVariance.count()
                         << "µs, but it actually took " << TimeTaken.count() << "µs."<< std::endl;
           }
        }

        void UnitTestGroup::TestTimedUnder(const String& TestName,
                                           chrono::microseconds MaxAcceptable,
                                           std::function<void ()> TestCallable,
                                           TestResult IfFalse, TestResult IfTrue,
                                           const String& FuncName, const String& File, Whole Line)
        {
           TimedTest TestDuration;
           TestCallable();
           std::chrono::microseconds TimeTaken{TestDuration.GetLength()};
           TestResult Result{Test(TestName, TimeTaken < MaxAcceptable, IfFalse, IfTrue, FuncName, File, Line)};
           if(Mezzanine::Testing::TestResult::Success != Result)
           {
               std::cout << "Expected test to take under " << MaxAcceptable.count()
                         << "µs, but it actually took " << TimeTaken.count() << "µs."<< std::endl;
           }
        }



    }// Testing
}// Mezzanine

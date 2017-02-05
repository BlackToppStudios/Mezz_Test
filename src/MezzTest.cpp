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

#include "MezzTest.h"
#include "TestData.h"

#include "DataTypes.h"

#include <cstdlib> // For system
#include <fstream>
#include <vector>
#include <stdexcept>

/// @file
/// @brief This file is the entry point for the unit test framework.
/// @details If you need to change the nature of the executable this is the
/// file to change. This is where the simple (but robust) sub process
/// mechanism is implemented. Very little of the rest of the code in the
/// unit test frame work makes calls to this file, and everything that does
/// does so through the UnitTestGroup class via polymorphism.
namespace Mezzanine
{
    namespace
    {
        /// @internal
        /// @brief The current process depth as interpretted by Main
        Testing::ProcessDepth Depth;

        SAVE_WARNING_STATE
        SUPPRESS_CLANG_WARNING("-Wexit-time-destructors")
        SUPPRESS_CLANG_WARNING("-Wglobal-constructors")
            /// @internal
            /// @brief This will store the name of the command that launched this executable at run time
            Mezzanine::String CommandName;

            /// @internal
            /// @brief A string intended for use by any subsubprocess test
            Mezzanine::String SubSubProcessArgument;
        RESTORE_WARNING_STATE
    }

    namespace Testing
    {
        ProcessDepth GetCurrentProcessDepth()
            { return Depth; }

        Mezzanine::String GetSubSubProcessArgument()
            { return SubSubProcessArgument; }

        void WriteTempFile(const Testing::UnitTestGroup &TestsToWrite)
        {
            std::ofstream File(TempFile.c_str());
            File << TestsToWrite.GetAsXML();
            File.close();
        }

        UnitTestGroup GetResultsFromTempFile()
        {
            UnitTestGroup Results;

            pugi::xml_document Doc;

            std::ifstream InputFile(TempFile.c_str());
            pugi::xml_parse_result LoadResults = Doc.load(InputFile);

            if(LoadResults)
            {
                Results.AddTestsFromXML(Doc.first_child());
            }else{
                std::stringstream FailStream;
                FailStream << "Failure loading tempfile from SubProcess: "
                           << LoadResults.description() << std::endl
                           << "At " << LoadResults.offset << " bytes into the file.";
                throw std::runtime_error(FailStream.str());
            }

            return Results;
        }

        void ClearTempFile()
        {
            std::ofstream FileToClear;
            FileToClear.open(TempFile.c_str(),std::ios_base::out|std::ios_base::trunc); // Clear the work file.
            FileToClear.close();
        }

        void DeleteTempFile()
            { std::remove(TempFile.c_str()); }

        SAVE_WARNING_STATE
        SUPPRESS_CLANG_WARNING("-Wweak-vtables") // We really don't care, because this is the only translation unit.
        /// @internal
        /// @brief This aggregates the results of all the other test groups.
        class AllUnitTestGroups : public UnitTestGroup
        {
            private:
                /// @internal
                /// @brief So no one uses it
                void operator=(AllUnitTestGroups&)
                    {}

                /// @internal
                /// @brief Should all tests be run.
                bool RunAll;

            public:

                /// @internal
                /// @brief This indicates we should run automatic tests rather than than launching a subprocess for that
                virtual void ShouldRunAutomaticTests()
                {
                    RunAll = false;
                    UnitTestGroup::ShouldRunAutomaticTests();
                }

                /// @internal
                /// @brief This indicates we should run interactive tests rather than than launching a subprocess.
                virtual void ShouldRunInteractiveTests()
                {
                    RunAll = false;
                    UnitTestGroup::ShouldRunInteractiveTests();
                }

                /// @internal
                /// @brief Tells this whether or not it should be used to run any tests that child tests might want to
                /// spawn in a subprocess.
                virtual void ShouldRunSubProcessTests()
                {
                    RunAll = false;
                    UnitTestGroup::ShouldRunSubProcessTests();
                }

                /// @internal
                /// @brief Used to signal
                virtual void ShouldRunAllTests()
                    { RunAll = true; }
                /// @internal
                /// @brief Used to signal that none or only a subset of tests should run
                virtual void DontRunAllTests()
                    { RunAll = false; }

                /// @internal
                /// @brief A collection of all the test groups
                CoreTestGroup& TestGroups;

                /// @internal
                /// @brief Constructor
                /// @param MainTestGroups The collection of tests that could be run.
                AllUnitTestGroups(CoreTestGroup& MainTestGroups)
                    : RunAll(true),
                      TestGroups(MainTestGroups)
                {}

                /// @internal
                /// @brief When determining what tests to run the name are aggregated here
                std::vector<Mezzanine::String> TestGroupsToRun;           //List of tests to run

            private:
                /// @internal
                /// @brief This is used when the passed flags at the command prompt or when a master process is
                /// executing a single tests.
                void ExecuteSubTest()
                {
                    for(std::vector<Mezzanine::String>::iterator CurrentTestName=TestGroupsToRun.begin();
                        CurrentTestName!=TestGroupsToRun.end();
                        ++CurrentTestName) // Actually run the tests
                    {
                        if(DoInteractiveTest)
                            { TestGroups[*CurrentTestName]->ShouldRunInteractiveTests(); }
                        if(DoAutomaticTest)
                            { TestGroups[*CurrentTestName]->ShouldRunAutomaticTests(); }
                        TestGroups[*CurrentTestName]->RunTests();
                        (*this) += *(TestGroups[*CurrentTestName]);
                    }
                }

                /// @internal
                /// @brief this is used when there are test to execute and we need to loop over them and run each in a
                /// child process.
                void IterateSubtests()
                {
                    setvbuf(stdout, NULL, _IONBF, 0);
                    for(std::vector<Mezzanine::String>::iterator CurrentTestName=TestGroupsToRun.begin();
                        CurrentTestName!=TestGroupsToRun.end();
                        ++CurrentTestName)
                    {
                        ClearTempFile();
                        String SubprocessInvocation(CommandName + " " + *CurrentTestName + " " + MemSpaceArg  +
                                                    Mezzanine::String(DoAutomaticTest?" automatic":"") +
                                                    Mezzanine::String(DoInteractiveTest?" interactive":""));
                        if(system(SubprocessInvocation.c_str()))   // Run a single unit test as another process
                        {
                            // Using printf because cout could be redirected here.
                            printf("%s", (SubprocessInvocation+String(" - Failure\n")).c_str() );
                            this->AddTestResult(String("Process::" + *CurrentTestName), Testing::TestResult::Failed);
                        }else {
                            // Using printf because cout could be redirected here.
                            printf("%s", (SubprocessInvocation+String(" - Success\n")).c_str() );
                            this->AddTestResult(String("Process::" + *CurrentTestName), Testing::TestResult::Success);
                        }

                        try
                        {
                            (*this) += GetResultsFromTempFile();
                        } catch (std::exception& e) {
                            TestError << e.what() << std::endl;
                        }

                    }
                    DeleteTempFile();
                }
            public:

                /// @internal
                /// @brief Determine which tests need to be run and run them
                virtual void RunTests()
                {
                    if (DoAutomaticTest==DoInteractiveTest && DoInteractiveTest==false)
                        { DoAutomaticTest=true; } // enforce running automatic tests if no type of test is specified
                    if(RunAll)
                    {
                        // if Runall is set it is presumed that no tests have been added to list of tests to run yet.
                        for(std::map<String,UnitTestGroup*>::iterator Iter=TestGroups.begin();
                            Iter!=TestGroups.end();
                            ++Iter)
                            { TestGroupsToRun.push_back(Iter->first); }
                    }

                    if(MainProcess == GetCurrentProcessDepth())
                    {
                        IterateSubtests();
                    }else{
                        ExecuteSubTest();
                    }
                } // \function

                /// @internal
                /// @brief Display the results either to the console or to the temp file for the main process to pick
                /// up later.
                /// @param Output Where to send the output, this only works for the main process. Defaults to std::cout.
                /// @param Error A stream to send all errors to, defailts to std::cerr
                /// @param Summary Passed to the UnitTests UnitTestGroup::DisplayResults if run from the main process.
                /// @param FullOutput Passed to the UnitTests UnitTestGroup::DisplayResults if run from the main
                /// process.
                /// @param HeaderOutput Passed to the UnitTests UnitTestGroup::DisplayResults if run from the main
                /// process.
                virtual void DisplayResults(std::ostream& Output=std::cout,
                                            std::ostream& Error=std::cerr,
                                            bool Summary = true,
                                            bool FullOutput = true,
                                            bool HeaderOutput = true)
                {
                    // Running a test in a seperate process, so we need to control the output for communcation purposes.
                    if(DoSubProcessTest)
                    {
                        WriteTempFile(*this);
                        UnitTestGroup::DisplayResults(Output, Error, Summary, FullOutput, HeaderOutput);
                    }else{
                        UnitTestGroup::DisplayResults(Output, Error, Summary, FullOutput, HeaderOutput);
                    }
                }
        };
        RESTORE_WARNING_STATE

        int MainImplementation(int argc, char** argv, CoreTestGroup& TestInstances)
        {
            Depth = MainProcess;

            bool WriteFile = true;

            // If this is not being run from a shell somehow, then using system() to run this task in a new process is
            // not really possible.
            if( !system( NULL ) )
            {
                std::cerr << "system() call not supported, missing command processor." << std::endl;
                return EXIT_FAILURE;
            }

            // Display everything, or just a Summary or neither? Should this process do the work, or should we spawn a
            // new process.
            bool FullDisplay = true, SummaryDisplay = true;

            if (argc > 0) //Not really sure how this would happen, but I would rather test and not have silent failures.
                { CommandName=argv[0]; }
            else
                { return Usage("UnitTestGroups", TestInstances); }

            AllUnitTestGroups Runner(TestInstances);

            for (int c=1; c<argc; ++c) // Check Command line for keywords and get all the test names
            {
                String ThisArg(AllLower(argv[c]));
                if(ThisArg=="help")
                    { return Usage(CommandName, TestInstances); }
                else if(ThisArg==MemSpaceArg)        // Check to see if we do the work now or later
                {
                    Runner.ShouldRunSubProcessTests();
                    Depth = TestSubSubProcess;
                }
                else if(ThisArg=="testlist")
                    { return PrintList(TestInstances); }
                else if(ThisArg=="interactive")
                    { Runner.ShouldRunInteractiveTests(); }
                else if(ThisArg=="automatic")
                    { Runner.ShouldRunAutomaticTests(); }
                else if(ThisArg=="all")
                    { Runner.ShouldRunAllTests(); }
                else if(ThisArg=="summary")
                    { FullDisplay = false, SummaryDisplay = true; }
                else if(ThisArg=="skipfile")
                    { WriteFile = false; }
                else  // Wasn't a command so it is either gibberish or a test group or debug test group
                {
                    if(ThisArg.size() > SubTestPrefix.size())
                    {
                        if( ThisArg.substr(0,SubTestPrefix.size()) == SubTestPrefix)
                        {
                            Depth = TestSubSubProcess;
                            ThisArg = ThisArg.substr(SubTestPrefix.size(), ThisArg.size()-SubTestPrefix.size());
                            CoreTestGroup::iterator SearchResult = TestInstances.find(ThisArg);
                            if(TestInstances.end()==SearchResult)
                            {
                                std::cerr << ThisArg
                                          << " appears to be a request to debug a sub-process that does not exist."
                                          << std::endl;
                                Usage(CommandName, TestInstances);
                                return ExitInvalidArguments;
                            }
                            if(!SearchResult->second->HasSubprocessTest())
                            {
                                std::cerr << ThisArg << " appears to be a request to debug a sub-process that does not"
                                          << " have a sub process." << std::endl;
                                Usage(CommandName, TestInstances);
                                return ExitInvalidArguments;
                            }
                            SearchResult->second->ShouldRunSubProcessTests();
                        }
                    }
                    CoreTestGroup::iterator Found = TestInstances.find(String(ThisArg.c_str()));
                    if(Found != TestInstances.end())
                    {
                        Runner.DontRunAllTests();
                        Runner.TestGroupsToRun.push_back(ThisArg.c_str());
                    }else{
                        if(GetCurrentProcessDepth() == TestSubSubProcess)
                        {
                            SubSubProcessArgument = String(ThisArg);
                        }else{
                            std::cerr << ThisArg << " is not a valid testgroup or parameter." << std::endl;
                            Usage(CommandName, TestInstances);
                            return ExitInvalidArguments;
                        }
                    }
                }
            }

            Runner.RunTests();

            if(WriteFile)
            {
                String FileName("TestResults.txt");
                std::ofstream OutFile(FileName.c_str());
                Runner.DisplayResults(OutFile, OutFile, SummaryDisplay, FullDisplay);
                OutFile.close();
            }
            if(MainProcess == GetCurrentProcessDepth())
                { Runner.DisplayResults(std::cout, std::cerr, SummaryDisplay, FullDisplay); }

            for(TestData Results : Runner)
            {
                if(TestResultToInt(Results.Results) > TestResultToInt(TestResult::Skipped) )
                    { return ExitFailure; }
            }
            return ExitSuccess;
        }

    }// Testing
}// Mezzanine

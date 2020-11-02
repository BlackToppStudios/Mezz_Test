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

#include "MezzTest.h"
#include "TestData.h"

#include "DataTypes.h"

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <thread>
#include <mutex>
#include <sstream>

namespace
{
    using namespace Mezzanine::Testing;

    /// @brief Create a type for delegating work to something dynamic based on a string based lookup.
    typedef std::map<Mezzanine::String, std::function<void()>> CallingTableType;

    Mezzanine::String SanitizeTestNameForJunit(const Mezzanine::String& ToSanitize)
    {
        Mezzanine::String Sanitized;
        Sanitized.reserve(ToSanitize.size());
        for( Mezzanine::String::value_type CurrChar : ToSanitize )
        {
            switch (CurrChar)
            {
                case '&':   Sanitized.append("&amp;"); break;
                case '<':   Sanitized.append("&lt;"); break;
                case '>':   Sanitized.append("&gt;"); break;
                case '"':   Sanitized.append("&quot;"); break;
                case '\'':  Sanitized.append("&apos;"); break;
                default:    Sanitized.append(1, CurrChar); break;
            }
        }
        return Sanitized;
    }

    CallingTableType CreateMainArgsCallingTable(const CoreTestGroup& TestInstances, ParsedCommandLineArgs& Results)
    {
        CallingTableType CallingTable;

        CallingTable[HelpToken] = [&Results](){ Results.ExitWithError = EXIT_FAILURE; };
        CallingTable[RunInThisProcessToken] = [&Results]{ Results.InSubProcess = true; };
        CallingTable[NoThreads] = [&Results]{ Results.ForceSingleThread = true; };
        CallingTable[JunitXMLAToken] = [&Results]{ Results.EmitJunitXml = true; };
        CallingTable[JunitXMLBToken] = [&Results]{ Results.EmitJunitXml = true; };

        // Debug does both Single thread and Single process.
        auto RunHere = [&Results]{ Results.InSubProcess = true; Results.ForceSingleThread = true; };
        CallingTable[DebugAToken] = RunHere;
        CallingTable[DebugBToken] = RunHere;

        CallingTable[AllToken] = [&Results, &TestInstances]()
        {
            Results.TestsToRun.clear();
            for(CoreTestGroup::value_type OneTest : TestInstances)
                { Results.TestsToRun.push_back(OneTest.second); }
        };

        CallingTable[AutomaticToken] = [&Results, &TestInstances]()
        {
            for(CoreTestGroup::value_type OneTest : TestInstances)
                { if(OneTest.second->ShouldRunAutomatically()) {Results.TestsToRun.push_back(OneTest.second);} }
        };

        CallingTable[InteractiveToken] = [&Results, &TestInstances]()
        {
            for(CoreTestGroup::value_type OneTest : TestInstances)
                { if(!OneTest.second->ShouldRunAutomatically()) {Results.TestsToRun.push_back(OneTest.second);} }
        };

        CallingTable[SkipSummaryToken] = [&Results](){ Results.SkipSummary = true; };
        CallingTable[SkipFileToken] = [&Results](){ Results.SkipFile = true; };

        return CallingTable;
    }
}

/// @file
/// @brief This file is the entry point for the unit test framework.
/// @details If you need to change the nature of the executable this is the
/// file to change. This is where the simple (but robust) sub process
/// mechanism is implemented. Very little of the rest of the code in the
/// unit test frame work makes calls to this file, and everything that does
/// does so through the UnitTestGroup class via polymorphism.
namespace Mezzanine
{
    namespace Testing
    {
        ParsedCommandLineArgs DealWithdCommandLineArgs(int argc, char** argv, const CoreTestGroup& TestInstances)
        {
            ParsedCommandLineArgs Results{ {}, "Mezz_Tester", EXIT_SUCCESS, false, false, false, false, false};

            if (argc > 0) //Not really sure how this would happen, but I would rather test and not have silent failures.
                { Results.CommandName = argv[0]; }
            else
                { Results.ExitWithError = EXIT_FAILURE; }

            // Construct a delegation table that can take a huge variety of actions based on strings.
            CallingTableType CallingTable = CreateMainArgsCallingTable(TestInstances, Results);

            // Loop over the argv and make a decision for each arg.
            for (int c=1; c<argc; ++c)
            {
                if(EXIT_SUCCESS != Results.ExitWithError) { break; } // Something bogus bail
                const Mezzanine::String ThisArg(AllLower(argv[c]));           // Insure case insensitivity
                if(CallingTable.count(ThisArg))                               // check for keywords that aren't tests.
                    { CallingTable[ThisArg](); }
                else if(TestInstances.count(ThisArg)) // Wasn't a keyword, could it be a test?
                    { Results.TestsToRun.push_back(TestInstances.at(ThisArg)); }
                else if(ThisArg.size()>SkipTestToken.size() &&
                        TestInstances.count(ThisArg.substr(SkipTestToken.size())))
                {
                    auto FilterTestByName =
                        [&](UnitTestGroup* T){ return ThisArg.substr(SkipTestToken.size()) == AllLower(T->Name()); };
                    Results.TestsToRun.erase(
                        std::remove_if(Results.TestsToRun.begin(), Results.TestsToRun.end(), FilterTestByName),
                        Results.TestsToRun.end()
                    );
                }
                else
                {
                    std::cerr << ThisArg.substr(SkipTestToken.size()) << std::endl
                              << "Argument '" << ThisArg << "' not valid." << std::endl; // bogus
                    Results.ExitWithError = EXIT_FAILURE;
                }
            }

            if(0==Results.TestsToRun.size())
                { CallingTable[AllToken](); }

            if(EXIT_SUCCESS != Results.ExitWithError) { Usage(Results.CommandName, TestInstances); }
            return Results;
        }

        TestResult RenderTestResultSummary(const UnitTestGroup::TestDataStorageType& AllResults,
                                           std::ostream& SummaryStream)
        {
            std::vector<Mezzanine::Int32> ResultsSummary(TestResultToUnsignedInt(TestResult::Highest)+1, 0);
            for(const TestData& OneResult : AllResults)
                { ResultsSummary[TestResultToUnsignedInt(OneResult.Results)]++; }

            SummaryStream << "    --= Summary =--\n";
            for(Mezzanine::UInt32 ResultInt = 0; IntToTestResult(ResultInt)<=TestResult::Highest; ResultInt++)
            {
                SummaryStream << std::right << std::setw(14) << IntToTestResult(ResultInt)
                              << std::left << " " << ResultsSummary[ResultInt] << '\n';
            }

            TestResult Worst = GetWorstResults(AllResults);
            SummaryStream << "\n  From " << AllResults.size() << " tests the worst result is: " << Worst << '\n';

            return Worst;
        }

        void RenderTimingsSummary(const std::vector<NamedDuration>& AllTimings, std::ostream& SummaryStream)
        {
            SummaryStream << std::right << std::setw(TimingNameColumnWidth) << "--= Name"
                          << std::left << ": Duration (ns) --- Human Readable =--\n";
            for(const NamedDuration& OneTiming: AllTimings)
            {
                SummaryStream << std::right << std::setw(TimingNameColumnWidth) << OneTiming.Name << ": "
                              << std::left << std::setw(TimingNsColumnWidth) << OneTiming.Duration.count()
                              << PrettyDurationString(OneTiming.Duration) << '\n';
            }
        }

        void RunSubProcessTest(const ParsedCommandLineArgs& Options,
                               UnitTestGroup& OneTestGroup)
        {
            if(Options.InSubProcess)
            {
                OneTestGroup(); // Run tests and discard results, the parent process will grab it.
            } else {
                String ProcessLog = RunCommand(
                    Options.CommandName + " " + OneTestGroup.Name() + " " +
                        RunInThisProcessToken + " " + SkipSummaryToken,
                    OneTestGroup.Name() + "_SubProcess.log"
                );
                std::istringstream LogStream(ProcessLog);
                Mezzanine::String OneLine;
                while( std::getline(LogStream, OneLine) )
                {
                    TestData PossibleResults( StringToTestData(OneLine) );
                    if(TestData{} != PossibleResults)
                    {
                        PossibleResults.TestName = "SubProcess::" + PossibleResults.TestName;
                        OneTestGroup.AddTestResultWithoutName(std::move(PossibleResults));
                    }
                }
            }
        }

        void RunParallelThreads(const ParsedCommandLineArgs& Options,
                                UnitTestGroup::TestDataStorageType& AllResults,
                                std::vector<NamedDuration>& TestTimings)
        {
            std::mutex ResultsMutex;
            std::vector<std::thread> TestThreads;

            for(UnitTestGroup* OneTestGroup : Options.TestsToRun)
            {
                UnitTestGroup& TestGroupForThread = *(OneTestGroup);

                // Skip the ones that cannot be run here. Run only the tests that love massive parallelism.
                if(TestGroupForThread.MustBeSerialized()) { continue; }

                // Store the test and any synchronization inside it.
                auto DoAndTimeThisTest = [&]()
                {
                    // Multithreaded part
                    TestTimer SingleThreadTimer;
                    if(TestGroupForThread.IsMultiThreadSafe())
                    {
                        TestGroupForThread.operator()();
                    } else {
                        RunSubProcessTest(Options, TestGroupForThread);
                    }

                    // Synchronize with single threaded part.
                    std::lock_guard<std::mutex> Lock(ResultsMutex);
                    AllResults.insert(AllResults.end(), TestGroupForThread.begin(), TestGroupForThread.end());
                    std::cout << TestGroupForThread.GetTestLog(); // Publish the Thread Specific TestLogs.
                    TestTimings.emplace_back (SingleThreadTimer.GetNameDuration(TestGroupForThread.Name() + "-T "));
                };

                // Run it here if forced or in a thread otherwise.
                if(Options.ForceSingleThread)
                    { DoAndTimeThisTest(); }
                else
                    { TestThreads.emplace_back(DoAndTimeThisTest); }


            }
            for(std::thread& OneTestThread : TestThreads) { OneTestThread.join(); }
        }

        void RunSerializedTests(const ParsedCommandLineArgs& Options,
                                UnitTestGroup::TestDataStorageType& AllResults,
                                std::vector<NamedDuration>& TestTimings)
        {
            for(UnitTestGroup* OneTestGroup : Options.TestsToRun)
            {
                UnitTestGroup& TestGroupForThread = *(OneTestGroup);

                // Skip the ones that cannot be run here because they can't stand parrellelism.
                if(TestGroupForThread.CanBeParallel()) { continue; }

                // Run all of the rest tests right here.
                TestTimer SingleThreadTimer;
                if(TestGroupForThread.IsMultiProcessSafe())
                {
                    RunSubProcessTest(Options, TestGroupForThread);
                } else {
                    TestGroupForThread.operator()();
                }

                // Synchronize with single threaded part.
                AllResults.insert(AllResults.end(), TestGroupForThread.begin(), TestGroupForThread.end());
                std::cout << TestGroupForThread.GetTestLog(); // Publish the Test Specific Logs.
                TestTimings.emplace_back (SingleThreadTimer.GetNameDuration(TestGroupForThread.Name() + "-S "));

            }
        }


        void EmitJunitResultsToFile(const UnitTestGroup::TestDataStorageType& AllResults)
        {
            std::ofstream JunitCompatibleXMLFile("Mezz_Test_Results.xml");
            EmitJunitResults(JunitCompatibleXMLFile, AllResults);
            JunitCompatibleXMLFile << std::endl;
        }

        void EmitJunitResults(std::ostream& Output, const UnitTestGroup::TestDataStorageType& AllResults)
        {
            //std::stringstream XmlContents;
            using Count = UnitTestGroup::TestDataStorageType::size_type;
            Count FailureCount{0};
            Count TestCount{AllResults.size()};
            Count SkippedCount{0};

            for(UnitTestGroup::TestDataStorageType::value_type OneResult : AllResults)
            {
                if(TestResult::Warning <= OneResult.Results)
                    { FailureCount++; }
                if(TestResult::Skipped == OneResult.Results)
                    { SkippedCount++; }
            }

            Output << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
                   << "  <testsuite "
                        << "tests=\"" << TestCount << "\" "
                        << "failures=\"" << FailureCount << "\" "
                        << "skipped=\"" << FailureCount << "\" "
                        << ">\n";
            for(UnitTestGroup::TestDataStorageType::value_type OneResult : AllResults)
            {
                String TestName{SanitizeTestNameForJunit(OneResult.TestName)};
                switch(OneResult.Results)
                {
                    case TestResult::Success:
                    case TestResult::NonPerformant:
                        Output << "    <testcase classname=\"" << OneResult.FileName
                                    << "\" name=\"" << TestName << "\" />\n";
                        break;

                    case TestResult::Skipped:
                        Output << "    <testcase classname=\"" << OneResult.FileName
                                    << "\" name=\""<< TestName << "\">\n"
                                << "        <skipped />\n"
                                << "    </testcase>\n";
                        break;

                    case TestResult::Cancelled:
                    case TestResult::Failed:
                    case TestResult::Inconclusive:
                    case TestResult::NotApplicable:
                    case TestResult::Unknown:
                    case TestResult::Warning:
                        Output << "    <testcase classname=\"" << OneResult.FileName
                                    << "\" name=\"" << TestName << "\">\n"
                                << "        <failure type=\"" << OneResult.Results << "\">\n"
                                << "            " << OneResult
                                << "        </failure>\n"
                                << "    </testcase>\n";

                }
            }
            Output << "  </testsuite>";
        }

        UnitTestGroup::TestDataStorageType RunTests(const ParsedCommandLineArgs& Options,
                                                    std::vector<NamedDuration>& TestTimings)
        {
            UnitTestGroup::TestDataStorageType AllResults;
            RunParallelThreads(Options, AllResults, TestTimings);
            RunSerializedTests(Options, AllResults, TestTimings);
            if(Options.EmitJunitXml)
                { EmitJunitResultsToFile(AllResults); }
            return AllResults;
        }

        ExitCode MainImplementation(int argc, char** argv, const CoreTestGroup& TestInstances)
        {
            try
            {
                // Start by timing everything, because somehow that is important.
                TestTimer TotalTimer;

                // If a shell is not supported, then using system() to run this task in a new process won't work.
                if( !system(nullptr) )
                {
                    std::cerr << "system() call not supported, missing command processor." << std::endl;
                    return EXIT_FAILURE;
                }

                // Handle the command line arguments.
                ParsedCommandLineArgs Options = DealWithdCommandLineArgs(argc, argv, TestInstances);
                if(EXIT_SUCCESS != Options.ExitWithError)
                    { return Options.ExitWithError; }

                // Reserve a fairly arbitrary amount of space for storing the timings of the work to be done, make sure
                // it is a power of two for maximum legitimacy.
                std::vector<NamedDuration> VariousTimings;
                VariousTimings.reserve(32);
                VariousTimings.push_back(TotalTimer.GetNameDuration("Initial Setup"));

                // Run the tests that need to be run.
                TestTimer TestExecutionTimer;
                UnitTestGroup::TestDataStorageType AllResults = RunTests(Options, VariousTimings);
                VariousTimings.emplace_back(TestExecutionTimer.GetNameDuration("Test Execution Time"));

                TestResult Worst;
                if(!Options.SkipSummary)
                {
                    // Handle formatting test results.
                    TestTimer SummaryTimer;
                    std::stringstream SummaryStream;
                    Worst = RenderTestResultSummary(AllResults, SummaryStream);
                    VariousTimings.push_back(SummaryTimer.GetNameDuration("Summary Reporting Time"));

                    // Handle Formatting Times.
                    TestTimer TimingsTimer;
                    std::stringstream TimingsStream;
                    RenderTimingsSummary(VariousTimings, TimingsStream);
                    NamedDuration TimeTime = TimingsTimer.GetNameDuration(" + Time Spent Reporting Time");

                    // Actually display it all.
                    NamedDuration TotalTime = TotalTimer.GetNameDuration(" ≈ Total Run Time");
                    std::cout << "\n\n" << TimingsStream.str() << TimeTime << "\n "
                            << Mezzanine::String(78, '_') << "\n  "
                            << TotalTime << "\n\n"
                            << SummaryStream.str() << std::endl;
                } else {
                    Worst = GetWorstResults(AllResults);
                }

                // Return Strongly typed success or error code, and let the autogenerated main deal with it.
                if(TestResult::Warning > Worst)
                    { return EXIT_SUCCESS; }
                return EXIT_FAILURE;

            }
            catch(const std::exception& e)
            {
                std::cout << "Uncaught exception of type std::exception made it to main. it says:\n"
                          << e.what() << std::endl;
                return EXIT_FAILURE;
            }
            catch (...)
            {
                std::cout << "Uncaught exception of unknown type made it to main." << std::endl;
                return EXIT_FAILURE;
            }

        }

    }// Testing
}// Mezzanine

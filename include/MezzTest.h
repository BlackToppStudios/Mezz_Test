// © Copyright 2010 - 2018 BlackTopp Studios Inc.
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
#ifndef Mezz_Test_MezzTest_h
#define Mezz_Test_MezzTest_h

/// @file
/// @brief The declaration of a number of tools to help with the main function implementation.

#include "AutomaticTestGroup.h"
#include "BenchmarkTestGroup.h"
#include "BenchmarkThreadTestGroup.h"
#include "ConsoleLogic.h"
#include "OutputBufferGuard.h"
#include "ProcessTools.h"
#include "StringManipulation.h"
#include "SilentTestGroup.h"
#include "TestData.h"
#include "TestMacros.h"
#include "TestEnumerations.h"
#include "TimingTools.h"
#include "UnitTestGroup.h"

#include <stdexcept> // Used to throw for TEST_THROW

namespace Mezzanine
{
    namespace Testing
    {
        SAVE_WARNING_STATE
        SUPPRESS_CLANG_WARNING("-Wpadded")
            /// @brief A struct storing everything that might be passed in on the command line.
            struct MEZZ_LIB ParsedCommandLineArgs
            {
                /// @brief The list of tests to run.
                std::vector<UnitTestGroup*> TestsToRun;

                /// @brief This will store the name of the command that launched this executable at run time.
                Mezzanine::String CommandName;

                /// @brief If not ExitCode::ExitSuccess then the caller should exit immediately.
                ExitCode ExitWithError;

                /// @brief The current process depth as interpreted by Main.
                Boole InSubProcess;

                /// @brief Force single threaded to help troubleshoot.
                Boole ForceSingleThread;

                /// @brief Skip writing the log file.
                Boole SkipFile;

                /// @brief Skip writing the summary at the end.
                Boole SkipSummary;
            };// ParsedCommandLineArgs
        RESTORE_WARNING_STATE

        /// @brief Deal with all the fine detail of dealing with command like arguments.
        /// @param argc Should be the argc passed in from the system.
        /// @param argv Should be the argv passed in from the system.
        /// @param TestInstances The complete set of tests.
        /// @return A complete ParsedCommandLineArgs detailing every arg that this handles.
        ParsedCommandLineArgs MEZZ_LIB DealWithdCommandLineArgs(int argc,
                                                                char** argv,
                                                                const CoreTestGroup& TestInstances);
        /// @brief Print a report of the tests to a stream.
        /// @param AllResults All of the results to appear in the summary.
        /// @param SummaryStream Place to print the results.
        /// @return The worst test result.
        TestResult MEZZ_LIB RenderTestResultSummary(const UnitTestGroup::TestDataStorageType& AllResults,
                                                    std::ostream& SummaryStream);

        /// @brief Print a report of the timings to a stream.
        /// @param AllTimings All of the timings to appear in the summary.
        /// @param SummaryStream Place to print the timings.
        void MEZZ_LIB RenderTimingsSummary(const std::vector<NamedDuration>& AllTimings,
                                           std::ostream& SummaryStream);

        /// @brief Run a single test that requires a subProcess.
        /// @param Options The parsed command line options.
        /// @param OneTestGroup The test group to execute.
        void MEZZ_LIB RunSubProcessTest(const ParsedCommandLineArgs& Options,
                                        UnitTestGroup& OneTestGroup);

        /// @brief Run all the tests that run in other threads.
        /// @param Options The options passed in by the user.
        /// @param AllResults The place to store test results.
        /// @param TestTimings The place to store all test timings.
        void MEZZ_LIB RunParallelThreads(const ParsedCommandLineArgs& Options,
                                         UnitTestGroup::TestDataStorageType& AllResults,
                                         std::vector<NamedDuration>& TestTimings);

        /// @brief Run all the tests that DON'T run in other threads.
        /// @param Options The options passed in by the user.
        /// @param AllResults The place to store test results.
        /// @param TestTimings The place to store all test timings.
        void MEZZ_LIB RunSerializedTests(const ParsedCommandLineArgs& Options,
                                         UnitTestGroup::TestDataStorageType& AllResults,
                                         std::vector<NamedDuration>& TestTimings);

        /// @brief Run all the tests per their normal execution policies.
        /// @param Options The options about what tests to run.
        /// @param TestTimings A collection of timings this will add to.
        /// @return A collections of all the results.
        UnitTestGroup::TestDataStorageType MEZZ_LIB RunTests(const ParsedCommandLineArgs& Options,
                                                             std::vector<NamedDuration>& TestTimings);

        /// @brief This is the entry point for the unit test executable.
        /// @details This will construct an AllUnitTestGroups with the listing of unit tests available from cmake
        /// generated source file. It will then interpret any command line arguments and direct the created
        /// AllUnitTestGroups about which tests to run and how to run them. In addition to sending the results to the
        /// standard output a copy of the test results will be written to TestResults.txt, if not configured not to.
        /// @n @n
        /// If no arguments are passed this will add all the tests to the AllUnitTestGroups
        /// and execute all tests that are not interactive. Print out a default report of them.
        /// @return This will return EXIT_SUCCESS if the tests ran, even if some or all failed,
        /// even if a child process segfaulted, but will return other statuses only if the main
        /// process fails. If the main process cannot create child processes it will return EXIT_FAILURE.
        /// @param argc Is interpreted as the amount of passed arguments.
        /// @param argv Is interpreted as the arguments passed in from the launching shell.
        /// @param TestInstances A group of tests to be executed.
        ExitCode MEZZ_LIB MainImplementation(int argc, char** argv, const CoreTestGroup& TestInstances);

        /// @brief When display timings with fixed width columns, this is how wide the name column is.
        static const Mezzanine::Whole TimingNameColumnWidth = 30;

        /// @brief When display timings with fixed width columns, this is how wide the nanosecond column is.
        static const Mezzanine::Whole TimingNsColumnWidth = 16;

        SAVE_WARNING_STATE
        SUPPRESS_CLANG_WARNING("-Wexit-time-destructors")
        SUPPRESS_CLANG_WARNING("-Wglobal-constructors")

        /// @brief A string that if passed on the command tells the tests not to launch sub processes.
        static const Mezzanine::String RunInThisProcessToken("thisprocess");
        /// @vopydoc RunInThisProcessToken
        static const Mezzanine::String DebugAToken("debug");
        /// @vopydoc RunInThisProcessToken
        static const Mezzanine::String DebugBToken("debugtests");

        /// @brief A string that if passed forces single threaded execution.
        static const Mezzanine::String NoThreads("nothreads");

        /// @brief A string that if passed on the command tells this to show the usage.
        static const Mezzanine::String HelpToken("help");

        /// @brief The token to pass on the command line to run all the tests.
        static const Mezzanine::String AllToken("all");

        /// @brief The token to pass on the command line to run all tests intended for unattended execution.
        static const Mezzanine::String AutomaticToken("automatic");

        /// @brief The token to pass on the command line to run all tests NOT intended for unattended execution.
        static const Mezzanine::String InteractiveToken("interactive");

        /// @brief The token to pass on the command line to skip the summary.
        static const Mezzanine::String SkipSummaryToken("skipsummary");

        /// @brief The token to pass on the command line to not emit a log file.
        static const Mezzanine::String SkipFileToken("skipfile");

        /// @brief The token to pass as a prefix to a test to skip it.
        static const Mezzanine::String SkipTestToken("skip-");

        RESTORE_WARNING_STATE
    }// Testing
}// Mezzanine

#endif

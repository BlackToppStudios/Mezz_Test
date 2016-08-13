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
#ifndef Mezz_Test_mezztest_h
#define Mezz_Test_mezztest_h

/// @file
/// @brief The declaration of a number of tools to help with the main function implementation.

#include "consolelogic.h"
#include "consolestringmanipulation.h"
#include "testdata.h"
#include "unittestgroup.h"
#include "testdatatools.h"
#include "testenumerations.h"

#include <stdexcept> // Used to throw for TEST_THROW

namespace Mezzanine
{
    namespace Testing
    {
        /// @brief Get the argument count as it was passed into Main.
        /// @return This is returned as an int
        int GetMainArgumentCount();

        /// @brief Get the argument vector as it was passed into Main.
        /// @return This is returned as pointer to a null terminated array of pointers to null terminated characters.
        char** GetMainArgumentVector();

        /// @brief Get the command/executable name used to invoke this at the command prompt.
        /// @return This is returned as a String.
        Mezzanine::String GetExecutableName();

        /// @brief Used to indicate the current 'depth' in the subprocess tree.
        /// @details Since the caller of the unit tests can pass arbitrary gibberish
        /// It is possible to get things that should only run in subsubprocesses in the
        /// main test process. This does not truly indicate how many processes have
        /// been spawned, but rather indicates the plans for what will be executed.
        enum ProcessDepth {
            MainProcess             = 0, ///< There is no plan to execute tests in this process subprocesses will be
                                         /// launched for any tests.
            TestSubprocess          = 1, ///< One or more Tests will be executed in this process, but no subprocess
                                         /// requested by tests will be run.
            TestSubSubProcess       = 2  ///< Tests requested to run in subrocesses by individual tests will run in this
                                         /// process.
        };

        /// @brief Indicates what this test might attempt to do, based on what it thinks the process depth is.
        /// @return a ProcessDepth enum member
        ProcessDepth GetCurrentProcessDepth();

        /// @brief If a test needs to pass a string to a subsubprocess it will get stored here
        /// @return A string that a test group intends on passing into a subsubprocess test
        Mezzanine::String GetSubSubProcessArgument();

        /// @internal
        /// @brief Write the passed UnitTestGroup to an XML temp file
        /// @param TestsToWrite Teh group of tests to write.
        void WriteTempFile(const Testing::UnitTestGroup &TestsToWrite);

        /// @internal
        /// @brief This will open then parse the contents of the file specified by TempFile and interpret any test
        /// results located
        /// @throw This can throw any exception that the C++ filestream classes can throw.
        /// @return This "reads" the temp file and interprets it. It tries to extract the name of the test as the whole
        /// of a line minus the last word. The last word is then converted into a @ref TestResult using
        /// @ref StringToTestResult. Any Whitespace between between the end of the last word and the end of the test
        /// name is dropped. All lines are interpretted this way and returned as a single @ref UnitTestGroup.
        UnitTestGroup GetResultsFromTempFile();

        /// @internal
        /// @brief Empty the file specified by TempFile
        /// @warning This doesn't ask for permission and can't easily be cancelled or recovered
        /// from. This will open, then erase the contents of the file.
        /// @throw This can throw any exception that the C++ filestream classes can throw.
        void ClearTempFile();

        /// @internal
        /// @brief Attempts to delete TempFile. Silently fails if not possible.
        void DeleteTempFile();

        /// @brief This is the entry point for the unit test executable.
        /// @details This will contruct an AllUnitTestGroups with the listing of unit tests available from cmake
        /// generated source file. It will then interpret any command line arguments and direct the created
        /// AllUnitTestGroups about which tests to run and how to run them. In addition to sending the results to the
        /// standard output a copy of the test results will be written to TestResults.txt, if not configure not to.
        /// @n @n
        /// If no arguments are passed this will add all the tests to the AllUnitTestGroups
        /// and execute all tests that are not interactive. Print out a default report of them.
        /// @return This will return EXIT_SUCCESS if the tests ran, even if some or all failed,
        /// even if a child process segfaulted, but will return other statuses only if the main
        /// process fails. If the main process cannot create child processes it will return EXIT_FAILURE.
        /// @param argc Is interpretted as the amount of passed arguments
        /// @param argv Is interpretted as the arguments passed in from the launching shell.
        int MainImplementation(int argc, char** argv, CoreTestGroup& TestInstances);

        /// @internal
        /// @brief If this is passed to the command line the test is executed without launching a separate processs.
        /// @details In most cases each test is launched as a separate process and this is passed to it.
        static const Mezzanine::String MemSpaceArg("debugtests");

        /// @internal
        /// @brief If this is passed to the command line prepended to a testname that tests subprocess will be executed
        /// instead of that test
        static const Mezzanine::String SubTestPrefix("debug");

        /// @internal
        /// @brief This is the name of the file used to communicate results from child processes
        /// @warning This variable is used to create temporary files in a percieved insecure way
        /// Everything will be fine as long as nothing else writes to this this file during or
        /// between Tests. If something does, then you probably have big enough problems you
        /// shouldn't be developing software until that is fixed.
        static const Mezzanine::String TempFile("UnitTestWork.txt");

    }// Testing
}// Mezzanine


#endif

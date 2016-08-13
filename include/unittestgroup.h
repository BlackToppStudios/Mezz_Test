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
#ifndef Mezz_Test_unittestgroup_h
#define Mezz_Test_unittestgroup_h

/// @file
/// @brief UnitTestGroup class definitions.


#include "testdata.h"

#include "testenumerations.h"
#include "pugixml.h"

#include <set>
#include <map>
#include <iostream>
#include <sstream>

namespace Mezzanine
{
    namespace Testing
    {
        ///////////////////////////////////////////////////////////////////////////////////////////
        /// @brief A single group of tests, suitable for being all the tests of a small subsystem or single class.
        class UnitTestGroup : public TestDataStorage
        {
            friend class OutputCaptureManager;

            protected:
                /// @brief A destination for all normal ouput in the tests.
                std::stringstream TestOutput;
                /// @brief A destination for errors
                std::stringstream TestError;

                /// @brief Used to store the buffer connected to the stdout while it is being redirected.
                std::streambuf* CoutStreamBuf;
                /// @brief Used to store the buffer connected to the stderr while it is being redirected.
                std::streambuf* CerrStreamBuf;

                /// @brief This will direct any output that would have gone to an external process via cout to
                /// TestOutput Instead
                void CaptureOutputBuffers();
                /// @brief This will direct any error messages that would have gone to an external process via cerr to
                /// TestOutput Instead
                void RestoreOutputBuffers();

                /// @brief Some basic variable for tracking simple statistics
                unsigned int LongestNameLength;

                /// @brief Set to false if subprocess tests should not be executed. True if they should
                bool DoSubProcessTest;
                /// @brief Set the flag to run automatic tests
                bool DoAutomaticTest;
                /// @brief Sets the flag to run interactive tests
                bool DoInteractiveTest;

                /// @brief Used while running a test to see if
                Int32 Completed;

            public:
                /// @brief Default constructor
                UnitTestGroup();

                /// @brief Copy constructor
                /// @param OtherGroup A UnitTestGroup to copy into this one. The contents of any log streams are copied and the streams themselves.
                UnitTestGroup(const UnitTestGroup& OtherGroup);

                /// @brief This will call RunAutomaticTests based on the values passed.
                /// @details All test results should be inserted using AddTestResult to allow the returning of results.
                /// @n @n This can be overloaded to enable better detection of skipped tests. This niavely reports only
                /// "TestName::Interactive" and "TestName::Automatic" as skipped, and even then only if HasAutomaticTests
                /// or HasInteractiveTests return true.
                virtual void RunTests();

            protected:
                /// @brief This launches all the automated tests on the derived class if the flag is
                /// set to run them otherwise it checks if tests exist via HasAutomaticTests() and
                /// marks them as skipped if they do.
                void LaunchAutomaticTest();
                /// @brief This launches all the interactice tests on the derived class if the flag
                /// is set to run them otherwise it checks if tests exist via HasAutomaticTests()
                /// and marksthem as skipped if they do.
                void LaunchInteractiveTest();

                /// @brief Tests should use this to launch things that need sheltering from
                /// segfaults and similar faults
                /// @param Argument A string combined with the command to be run, this must be
                /// sanitized before being called, do not pass untrusted data.
                /// @return The output to stdout from the subprocess.
                String LaunchSubProcessTest(const String& Argument = String(""));

            public:
                /// @brief This should be overloaded to run all tests that do require not user interaction
                virtual void RunAutomaticTests();
                /// @brief Used only to report skipped tests.
                /// @return Defaults to returning false, but should be overloaded to return true if RunAutomaticTests() implements any tests.
                virtual bool HasAutomaticTests() const;
                /// @brief Sets a flag that indicatesz that is the process that should run this subprocess.
                virtual void ShouldRunAutomaticTests();

                /// @brief This should be overloaded to run all tests require user interaction
                virtual void RunInteractiveTests();
                /// @brief Used only to report skipped tests.
                /// @return Defaults to returning false, but should be overloaded to return true if RunInteractiveTests() implements any tests.
                virtual bool HasInteractiveTests() const;
                /// @brief Sets a flag that indicatesz that is the process that should run this subprocess.
                virtual void ShouldRunInteractiveTests();

                /// @brief Does nothing by default, tests which need to run code in a subprocess should override this.
                /// @details This will be executed in a subprocess before HasAutomaticTests() and RunInteractiveTests();
                /// @param Arg An argument from the calling test.
                /// @return Whatever was sent to stdout via C++ streams will be captured and sent here instead.
                virtual void RunSubprocessTest(const Mezzanine::String& Arg);
                /// @brief If this returns false then the test suite treats it like any other test, if true then it enables some features for launching subprocess tests
                /// @details This will cause an extra command line option to be created (as "debug" + testname). The function SubprocessTest() will be executed in the
                /// process that the new option is passed into. This allows for subprocess debugging. This will automatically be passed to the test process that will
                /// executed the sub-process tests.
                /// @return This returns false by default, any test which wants to execute a subtest will need to implement this to return true.
                virtual bool HasSubprocessTest() const;
                /// @brief Sets a flag that indicatesz that is the process that should run this subprocess.
                virtual void ShouldRunSubProcessTests();


                /// @brief Get Name of this UnitTestGroup
                /// @return The string that must be type at the command line to run this testgroup, should be all lowercase.
                /// @note One of two methods that must be implmented on a UnitTestGroup
                virtual Mezzanine::String Name();

                /// @brief Its expected that tests will be inserted using this
                /// @details This will automate tracking of the most and least successful tests
                /// @param CurrentTest The New test results and name
                /// @param Behavior An OverWriteResults that defines the overwrite behavior of this function, defaults to OverWriteIfLessSuccessful
                void AddTestResult( TestData CurrentTest,
                                    OverWriteResults Behavior=OverWriteIfLessSuccessful);

                /// @brief Add a test results without having to to construct a TestData first
                /// @details This prepends the name of this UnitTestGroup and "::" to the
                /// @warning The name of the test can have no spaces in it. An exception will be thrown if found.
                /// @param TestName The name of the Test
                /// @param TResult The actual TestResult
                /// @param Behavior An OverWriteResults that defines the overwirte behavior of this function, defaults to OverWriteIfLessSuccessful
                void AddTestResult(const Mezzanine::String TestName, TestResult TResult, OverWriteResults Behavior=OverWriteIfLessSuccessful);

                /// @brief Add all the items in another UnitTestGroup to this one
                /// @param rhs The item on the right hand side of the +=.
                /// @return A reference to this is returned allowiong operator chaining.
                const UnitTestGroup& operator+=(const UnitTestGroup& rhs);

                /// @brief Create and add all the tests in a given piece of parsed xml
                /// @param Node A pugi::xml_node referencing a UnitTestGroup
                void AddTestsFromXML(pugi::xml_node Node);

                /// @brief Get the Whole UnitTestGroup as a valid XML document
                /// @details The root element of the XMl document is named UnitTestGroup
                /// and it will contain the XML from each TestData this contains
                /// @return A String containing some XML
                String GetAsXML() const;

                /// @brief Print the results or save them to a file.
                /// @param Output the stream to send the results to.
                /// @param Error A stream to send errors to.
                /// @param Summary Print Statistics at the end, not needed when sending results between processes. Defaults to true/enabled.
                /// @param FullOutput Sometimes the user does not want to see each test results and just wants a little blurb. Defaults to true/enabled.
                /// @param HeaderOutput Makes the output a little more understandable it is short or needs to be copied into a spreadsheet. Defaults to true/enabled.
                virtual void DisplayResults(std::ostream& Output=std::cout, std::ostream& Error=std::cerr, bool Summary = true, bool FullOutput = true, bool HeaderOutput = true);

                /// @brief Interpret Boolean value as a test result. Also Prepends the name of the current test, as returned by Name() + "::", to ease test scoping.
                /// @warning IfFalse comes first in the argument list, This is because the common test cases have IfTrue = Testing::Success while IfFalse makes sense as other things
                /// @param TestCondition The test itself or the results of it.
                /// @param TestName The comple name of the test, used to track results.
                /// @param IfFalse Defaults to Testing::Failed but can be whatever Testing::TestResult you want if a false passed as the TestCondition.
                /// @param IfTrue Defaults to Testing::Success but can be whatever Testing::TestResult you want if a true passed as the TestCondition.
                /// @param FuncName The function the test was called from, if blank
                /// @param File To make tracking down failures easier the file name of the test can
                /// be passed in, if not set an empty string is used
                /// @param Line To make tracking down failures easier the line number of the test
                /// can  be passed in, if not set an empty string is used
                virtual void Test(  bool TestCondition,
                                    const String& TestName,
                                    TestResult IfFalse = Testing::Failed,
                                    TestResult IfTrue = Testing::Success,
                                    const String& FuncName = "",
                                    const String& File = "",
                                    Mezzanine::Whole Line = 0);
        };

        /// @internal
        /// @brief Used to aplly RAII to Stdout and STDERR buffers/streams
        class OutputCaptureManager
        {
            private:
                /// @brief The Target to work with
                UnitTestGroup* Target;
            public:
                /// @brief Captures Output buffers and configures test outputs on creation
                OutputCaptureManager(UnitTestGroup* RAIITarget) : Target(RAIITarget)
                    { Target->CaptureOutputBuffers(); }
                /// @brief Restores original output  buffers on creation
                ~OutputCaptureManager()
                    { Target->RestoreOutputBuffers(); }

        };



    }// Testing
}// Mezzanine

#endif

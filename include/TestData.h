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
#ifndef Mezz_Test_testdata_h
#define Mezz_Test_testdata_h

/// @file
/// @brief TestData and TestDataStorage class definitions.

#include "DataTypes.h"

#include "TestEnumerations.h"

SAVE_WARNING_STATE
SUPPRESS_CLANG_WARNING("-Wdeprecated")
SUPPRESS_CLANG_WARNING("-Wweak-vtables")
SUPPRESS_CLANG_WARNING("-Wpadded")
    #include "pugixml.h"
RESTORE_WARNING_STATE

#include <set>
#include <map>
#include <iostream>
#include <sstream>

namespace Mezzanine
{
    /// @namespace Mezzanine::Testing
    /// @brief This contains all the items (except the tests themselves) that make the unit tests work.
    namespace Testing
    {
        SAVE_WARNING_STATE
        SUPPRESS_CLANG_WARNING("-Wpadded")
        ///////////////////////////////////////////////////////////////////////////////////////////
        /// @brief The information about a test and how to easily find it in the filesystem
        struct TestData
        {
            /// @brief The name of a given test
            Mezzanine::String TestName;
            /// @brief The function the test was called from
            Mezzanine::String FunctionName;
            /// @brief The File The test happened in
            Mezzanine::String FileName;
            /// @brief What line in the file this test occurred when the test was compiled
            Mezzanine::Whole LineNumber;
            /// @brief How did the test turn out
            TestResult Results;

            /// @brief Create a TestData
            /// @param Name the name of the test, defaults to "".
            /// @param Result A TestResult, defaults to Testing::Success.
            /// @param FuncName The name of the function this test was called from, Defaults to "".
            /// @param File The name of the file in which the test exists, Defaults to "".
            /// @param Line The line in the file in which the test exists, Defaults to 0.
            TestData(const String& Name = "",
                     TestResult Result = Testing::TestResult::Success,
                     const String& FuncName = "",
                     const String& File = "",
                     Mezzanine::Whole Line = 0);

            /// @brief Create Test data from xml.
            /// @param Node The XMl node to create this from
            TestData(pugi::xml_node Node);

            /// @brief Used to sort TestData in std::std and other sorted containers, by TestName.
            /// @param Rhs the right hand operand when using the less than comparison operator.
            /// @return A bool with the same value as this->TestName < Rhs.TestName.
            bool operator<(const TestData& Rhs) const;

            /// @brief Return a snippet of xml describing this TestData
            /// @return A String with a single XML element with an attribute for each of the TestName, Results,
            /// FunctionName, FileName, LineNumber
            String GetAsXML() const;
        };
        RESTORE_WARNING_STATE

        /// @brief Just a map to store the content of TestData, incidentally it will lexographically sort the list of
        /// tests.
        typedef std::set<TestData> TestDataStorage;

        // Forward declarations.
        class UnitTestGroup;
        class OutputCaptureManager;

        /// @brief A group of testnames and the Actual class that implements those test(s).
        typedef std::map<Mezzanine::String, UnitTestGroup*> CoreTestGroup;

        /// @brief Print all the groups that exist in a given CoreTestGroup
        /// @param TestGroups The group whose constents names with be printed
        /// @return ExitSuccess on success.
        int PrintList(CoreTestGroup &TestGroups);

        /// @brief Trim the whitespace from a line of text and try to interpret the remains as TestResults and a
        /// testname.
        /// @param Line A line of Test that starts with whitespace, then a TestResult String, then has a whitesapce
        /// delimiter and a ends with the name of test.
        /// @return A parsed TestData.
        TestData StringToTestData(Mezzanine::String Line);
    }// Testing
}// Mezzanine

#endif

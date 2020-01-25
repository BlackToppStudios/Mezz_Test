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
#ifndef Mezz_Test_TestData_h
#define Mezz_Test_TestData_h

/// @file
/// @brief TestData and TestDataStorage class definitions.

#include "DataTypes.h"

#include "TestEnumerations.h"

#include <iostream>

namespace Mezzanine
{
    /// @namespace Mezzanine::Testing
    /// @brief This contains all the items (except the tests themselves) that make the unit tests work.
    namespace Testing
    {
        SAVE_WARNING_STATE
        SUPPRESS_CLANG_WARNING("-Wpadded")
        ///////////////////////////////////////////////////////////////////////////////////////////
        /// @brief The information about a test and how to easily find it in the filesystem.
        struct MEZZ_LIB TestData
        {
            /// @brief The name of a given test.
            Mezzanine::String TestName;
            /// @brief The function the test was called from.
            Mezzanine::String FunctionName;
            /// @brief The File The test happened in.
            Mezzanine::String FileName;
            /// @brief What line in the file this test occurred when the test was compiled.
            Mezzanine::Whole LineNumber;
            /// @brief How did the test turn out.
            TestResult Results;

            /// @brief Create a TestData.
            /// @param Name the name of the test, defaults to "".
            /// @param Result A TestResult, defaults to Testing::Success.
            /// @param FuncName The name of the function this test was called from, Defaults to "".
            /// @param File The name of the file in which the test exists, Defaults to "".
            /// @param Line The line in the file in which the test exists, Defaults to 0.
            explicit TestData(const String& Name = "",
                              TestResult Result = Testing::TestResult::Success,
                              const String& FuncName = "",
                              const String& File = "",
                              Mezzanine::Whole Line = 0);

            /// @brief Default copy constructible.
            /// @param ToCopy Other TestDataToCopy.
            TestData(const TestData& ToCopy) = default;

            /// @brief Default move constructible.
            /// @param ToMove Other TestDataToCopy.
            TestData(TestData&& ToMove) = default;

            /// @brief Default copy constructible.
            /// @param ToCopy Other TestDataToCopy.
            /// @return A reference to the assigned TestData.
            TestData& operator=(const TestData& ToCopy) = default;

            /// @brief Default move assignable.
            /// @param ToMove Other TestDataToCopy.
            /// @return A reference to the assigned TestData.
            TestData& operator=(TestData&& ToMove) = default;

            /// @brief Used to sort TestData in std::std and other sorted containers, by TestName.
            /// @param Rhs the right hand operand when using the less than comparison operator.
            /// @return A bool with the same value as this->TestName < Rhs.TestName.
            Boole operator<(const TestData& Rhs) const;

            /// @brief Used to compare two TestData mostly for testing purposes.
            /// @param Rhs the right hand operand when using the == operator.
            /// @return True if every member matches, false otherwise.
            Boole operator==(const TestData& Rhs) const;

            /// @brief Used to compare two TestData for inequality.
            /// @param Rhs the right hand operand when using the ~= operator.
            /// @return True if any members differ, false otherwise.
            Boole operator!=(const TestData& Rhs) const;
        };// TestData
        RESTORE_WARNING_STATE

        /// @brief Trim the whitespace from a line of text and try to interpret the remains as TestResults and a
        /// test name.
        /// @param Line A line of Test that starts with whitespace, then a TestResult String, then has a whitesapce
        /// delimiter and a ends with the name of test.
        /// @return A parsed TestData.
        TestData MEZZ_LIB StringToTestData(Mezzanine::String Line);

        /// @brief Test names to to fit on a single line to be read from files, This insures they fit on one line.
        /// @param RawName A name of test that needs to be made safe for writing to a file.
        /// @return A String with all backslashes and carriage returns escaped with more backslashes.
        Mezzanine::String MEZZ_LIB EscapeTestNameString(const Mezzanine::String& RawName);

        /// @brief Test names in files are not suitable for reading directly, this fixes that.
        /// @param MungedName A name of test that needs to be made safe for human viewing.
        /// @return A String with all backslashes and carriage returns put back how they belong.
        Mezzanine::String MEZZ_LIB UnescapeTestNameString(const Mezzanine::String& MungedName);

        /// @brief Send a TestData down a stream.
        /// @param Stream the stream to write to.
        /// @param ToStream The TestData to Write.
        /// @return the stream after modification.
        std::ostream& MEZZ_LIB operator<< (std::ostream& Stream, const TestData& ToStream);
    }// Testing
}// Mezzanine

#endif

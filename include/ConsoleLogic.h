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
#ifndef Mezz_Test_ConsoleLogic_h
#define Mezz_Test_ConsoleLogic_h

/// @file
/// @brief The definition of a few command line tools.

#include "DataTypes.h"
#include "UnitTestGroup.h"

namespace Mezzanine
{
    namespace Testing
    {
        /// @brief Print a message for the user on the standard output that briefly describes how to use this.
        /// @param ThisName The name of the current executable.
        /// @param TestGroups This is searched for the current list of valid test names.
        /// @details This just prints what GetUsageString returns.
        void MEZZ_LIB Usage(const Mezzanine::String& ThisName, const Mezzanine::Testing::CoreTestGroup& TestGroups);

        /// @param ThisName The name of the current executable.
        /// @param TestGroups This is searched for the current list of valid test names.
        /// @return A human readable string suitable for explaining how to use this executable.
        Mezzanine::String MEZZ_LIB GetUsageString(const Mezzanine::String& ThisName,
                                                  const Mezzanine::Testing::CoreTestGroup& TestGroups);

        /// @brief Print all the names of all the tests to stdout and try to keep the size under the passed width.
        /// @param TargetWidth The expected width of the console. If a test name is longer than this it will print past
        /// this width.
        /// @TestGroups The testgroup to print the names of.
        Mezzanine::String MEZZ_LIB GetPrintableTestList(const Mezzanine::String::size_type TargetWidth,
                                                        const CoreTestGroup& TestGroups);
/*
        // Most remarked code ought to be removed swiftly, This feature will be re-added as soon as a scheme for
        // testing it exists.

        /// @brief Asked the user a question on the std output and get a TestResult as an answer.
        /// @param Question The question to ask the user.
        /// Status otherwise they will return Warnings.
        /// @details The following strings provide the following results.
        ///     -  "True", "Yes" as Success.
        ///     -  "False", "No" as Failed or Warn.
        ///     -  "Cancel" as "Cancelled".
        ///     -  "Warn" as "Warning".
        ///     -  "Unsure", "Inconclusive" as Inconclusive.
        /// @return Depends on users input
        TestResult MEZZ_LIB GetTestAnswerFromStdin(Mezzanine::String Question);
*/
        /// @brief Convert one human readable character to a TestResults by inferring things like 'Y' is "yes" which is
        /// "success", 'F' is 'Fail' is a test "Failure" etc...
        /// @param Answer The answer that presumably a human provided.
        /// @return Any of 'T', 't', 'Y', 'y' return TestResult::Success. Any of 'F', 'f', 'N', 'n' return
        /// TestResult::Failed. Any of 'C', 'c' return TestResult::Cancelled.  Any of 'W', 'w' return
        /// TestResult::Warning. Any of 'U', 'u', 'I', 'i' return TestResult::Inconclusive. Anything else returns
        /// TestResult::Unknown.
        TestResult MEZZ_LIB Char8ToTestResults(Mezzanine::Char8 Answer);
    }// Testing
}// Mezzanine

#endif

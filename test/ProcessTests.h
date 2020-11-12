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
#ifndef Mezz_Test_ProcessTests_h
#define Mezz_Test_ProcessTests_h

/// @file
/// @brief Test tools related to launching processes.

#include "MezzTest.h"

#include <fstream>

/// @brief Tests for the class to store test data results.
AUTOMATIC_TEST_GROUP(ProcessTests, Process)
{
    using namespace Mezzanine;
    {//GetFileContents
        const String TestFilename("ProcessTestFile.txt");
        const String TestToken(
                    "I've seen things you people wouldn't believe. Attack ships on fire off the "
                    "shoulder of Orion. I watched C-beams glitter in the dark near the Tannhäuser "
                    "Gate. All those moments will be lost in time, like tears in rain. Time to die.");
        std::ofstream TestFile(TestFilename);
        TestFile << TestToken;
        TestFile.close();

        TEST_EQUAL("GetFileContents(const_StringView)", TestToken, Testing::GetFileContents(TestFilename))
    }//GetFileContents

    {//GetCommandOutput
        TEST_STRING_CONTAINS("GetCommandOutput(const_StringView)-foo",
                             String("foo"),
                             Testing::GetCommandOutput("cmake -E echo \"foo\""))
        TEST_THROW("GetCommandOutput(const_StringView)-Throw-BadSymbol",
                   std::runtime_error,
                   []{ (void)Testing::GetCommandOutput("echo foo > somefile.txt"); })
        TEST_THROW("GetCommandOutput(const_StringView)-Throw-BadExe",
                   std::runtime_error,
                   []{ (void)Testing::GetCommandOutput("NotARealFilePlzDontActuallyExist"); })
    }//GetCommandOutput

    {//GetCommandOutput w/ ExecutablePath
        // No good way to test this.
    }//GetCommandOutput w/ ExecutablePath

    {//OutputCommandToFile

    }//OutputCommandToFile

    {//OutputCommandToFile w/ ExecutablePath
        // No good way to test this.
    }//OutputCommandToFile w/ ExecutablePath

    {//RunCommand
        StringView Empty;
        Testing::CommandResult HelloResult = Testing::RunCommand(Empty,"cmake -E echo Hello");
        TEST_EQUAL("RunCommand(const_StringView,const_StringView)-Hello-ExitCode",
                   Integer(0),
                   HelloResult.ExitCode);
        TEST_STRING_CONTAINS("RunCommand(const_StringView,const_StringView)-Hello-Output",
                             String("Hello"),
                             HelloResult.ConsoleOutput);
        Testing::CommandResult FalseResult = Testing::RunCommand(Empty,"cmake -E false");
        TEST_EQUAL("RunCommand(const_StringView,const_StringView)-FalseCommand-ExitCode",
                   Integer(1),
                   FalseResult.ExitCode);
        TEST_EQUAL("RunCommand(const_StringView,const_StringView)-FalseCommand-Output",
                   true,
                   FalseResult.ConsoleOutput.empty());
        std::cout << "\nFalseResult output: " << FalseResult.ConsoleOutput << ".\n";
        Testing::CommandResult TrueResult = Testing::RunCommand(Empty,"cmake -E true");
        TEST_EQUAL("RunCommand(const_StringView,const_StringView)-TrueCommand-ExitCode",
                   Integer(0),
                   TrueResult.ExitCode);
        TEST_EQUAL("RunCommand(const_StringView,const_StringView)-TrueCommand-Output",
                   true,
                   TrueResult.ConsoleOutput.empty());
    }//RunCommand

    //Integer MEZZ_LIB OutputCommandToFile(const StringView Command, const StringView OutputFileName)
}

#endif

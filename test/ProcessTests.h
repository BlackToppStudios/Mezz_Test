// © Copyright 2010 - 2021 BlackTopp Studios Inc.
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
DEFAULT_TEST_GROUP(ProcessTests, Process)
{
    using namespace Mezzanine;

    {//RunCommand
        Testing::CommandResult HelloResult = Testing::RunCommand("cmake -E echo Hello");
        TEST_EQUAL("RunCommand(const_StringView)-Hello-ExitCode",
                   Integer(0),
                   HelloResult.ExitCode)
        TEST_STRING_CONTAINS("RunCommand(const_StringView)-Hello-Output",
                             String("Hello"),
                             HelloResult.ConsoleOutput)

        Testing::CommandResult FalseResult = Testing::RunCommand("git asdfg");
        TEST_EQUAL("RunCommand(const_StringView)-FalseCommand-ExitCode",
                   Integer(1),
                   FalseResult.ExitCode)
        TEST_EQUAL("RunCommand(const_StringView)-FalseCommand-Output",
                   true,
                   FalseResult.ConsoleOutput.empty())

        Testing::CommandResult TrueResult = Testing::RunCommand("git --help");
        TEST_EQUAL("RunCommand(const_StringView)-TrueCommand-ExitCode",
                   Integer(0),
                   TrueResult.ExitCode)
        TEST_EQUAL("RunCommand(const_StringView)-TrueCommand-Output",
                   false,
                   TrueResult.ConsoleOutput.empty())

        TEST_THROW("RunCommand(const_StringView)-Throw-BadSymbol",
                   std::runtime_error,
                   []{ (void)Testing::RunCommand("echo foo | somefile.txt"); })
    }//RunCommand

    {//RunCommand w/ ExecutablePath
        // No good way to test this.
    }//RunCommand w/ ExecutablePath
}

#endif

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
#ifndef Mezz_Test_ProcessTools_h
#define Mezz_Test_ProcessTools_h

/// @file
/// @brief Tools for running commands and getting their output.

#include "DataTypes.h"

namespace Mezzanine {
namespace Testing {
    /// @brief A simple struct for storing (some) output from a called process.
    struct MEZZ_LIB CommandResult
    {
        /// @brief The code returned when the called process exited.
        Integer ExitCode = EXIT_FAILURE;
        /// @brief The output to cout and cerr from the called process.
        String ConsoleOutput;
    };//CommandResult

    ///////////////////////////////////////////////////////////////////////////////
    // Output to String

    /// @brief Launches a different process on the system.
    /// @param ExecutablePath The path to the executable to be invoked.
    /// @param Command The command to attempt to run and direct its output.
    /// @return Returns the ExitCode and Cout/Cerr output of the command that was run.
    CommandResult RunCommand(const StringView ExecutablePath, const StringView Command);

    /// @brief Launches a different process on the system and return only it's console output as a String.
    /// @remarks This function ignores the exit code of the command. If you want/need the exit code, use
    /// @ref RunCommand instead.
    /// @param ExecutablePath The path to the executable to be invoked.
    /// @param Command The command to attempt to run and direct its output.
    /// @return Returns a String containing the output of the specified command.
    [[nodiscard]]
    String MEZZ_LIB GetCommandOutput(const StringView ExecutablePath, const StringView Command);
    /// @brief Launches a different process on the system and return only it's console output as a String.
    /// @remarks This function ignores the exit code of the command. If you want/need the exit code, use
    /// @ref RunCommand instead.
    /// @param Command The command to attempt to run and direct its output.
    /// @return Returns a String containing the output of the specified command.
    [[nodiscard]]
    String MEZZ_LIB GetCommandOutput(const StringView Command);

    ///////////////////////////////////////////////////////////////////////////////
    // Output to File

    /// @brief Run a process and direct its console output to a file.
    /// @remarks This function is meant to be used in conjunction with GetFileContents to view the output.
    /// @details This launches a process, redirects its output to a file and returns the command's exit code.
    /// This is a simple wrapper around std::system, concatenating output redirection to the command. Because of
    /// this "|", "<" and ">" are not allowed in commands passed to this.
    /// @param ExecutablePath The path to the executable to be invoked.
    /// @param Command The command to attempt to run and direct its output.
    /// @param OutputFileName The file to put all the console output of the command.
    /// @return Returns the exit code of the command.
    Integer MEZZ_LIB OutputCommandToFile(const StringView ExecutablePath,
                                         const StringView Command,
                                         const StringView OutputFileName);
    /// @brief Run a process and direct its console output to a file.
    /// @remarks This function is meant to be used in conjunction with GetFileContents to view the output.
    /// @details This launches a process, redirects its output to a file and returns the command's exit code.
    /// This is a simple wrapper around std::system, concatenating output redirection to the command. Because of
    /// this "|", "<" and ">" are not allowed in commands passed to this.
    /// @param Command The command to attempt to run and direct its output.
    /// @param OutputFileName The file to put all the console output of the command.
    /// @return Returns the exit code of the command.
    Integer MEZZ_LIB OutputCommandToFile(const StringView Command,
                                         const StringView OutputFileName);

    /// @brief Get all the text in a file.
    /// @param FileName The file to read.
    /// @return A string with the contents.
    [[nodiscard]]
    String MEZZ_LIB GetFileContents(const StringView FileName);
}// Testing
}// Mezzanine

#endif

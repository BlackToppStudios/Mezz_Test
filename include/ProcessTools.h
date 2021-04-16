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
#ifndef Mezz_Test_ProcessTools_h
#define Mezz_Test_ProcessTools_h

/// @file
/// @brief Tools for running commands and getting their output.

#include "DataTypes.h"

namespace Mezzanine {
namespace Testing {

SAVE_WARNING_STATE
SUPPRESS_CLANG_WARNING("-Wpadded")

    /// @brief A simple struct for storing (some) output from a called process.
    struct MEZZ_LIB CommandResult
    {
        /// @brief The output to cout from the called process.
        String ConsoleOutput;
        /// @brief The code returned when the called process exited.
        Integer ExitCode = EXIT_FAILURE;
    };//CommandResult

RESTORE_WARNING_STATE

    /// @brief Launches a different process on the system.
    /// @note ExecutableName cannot be empty on Posix systems or this function will fail.
    /// @param ExePathName The identifier for the executable to be launched. This can be an absolute path,
    /// relative path, or a just an executable that will be searched for in the system PATH.
    /// @param Command The command to attempt to run and direct its output.
    /// @return Returns the ExitCode and Cout output of the command that was run.
    [[nodiscard]]
    CommandResult MEZZ_LIB RunCommand(const StringView ExePathName, const StringView Command);
    /// @brief Launches a different process on the system.
    /// @remarks This function will interpret all of the text up until the first delimiter (space or tab)
    /// to be the path to the executable to be launched. If that executable has a space in it's path, use
    /// the two parameter version of this function instead. @n@n
    /// Like the two parameter version, the first part of the command that specifies the executable to be
    /// launched can be an absolute path, relative path, or just an executable to be searched for in the
    /// system PATH.
    /// @param Command The command to attempt to run and direct its output.
    /// @return Returns the ExitCode and Cout output of the command that was run.
    [[nodiscard]]
    CommandResult MEZZ_LIB RunCommand(const StringView Command);
}// Testing
}// Mezzanine

#endif

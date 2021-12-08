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
#ifndef Mezz_Test_StringManipulation_h
#define Mezz_Test_StringManipulation_h

/// @file
/// @brief Some string manipulation functions for use in the Unit Tests.

#include "DataTypes.h"

namespace Mezzanine
{
    namespace Testing
    {
        /// @brief Take the whitespace off the end of a String.
        /// @param Text The Text to clean up.
        /// @return If " asdf " is passed " asdf" is returned. Also removes carriage returns, newlines and tabs. Does
        /// not use the locale.
        Mezzanine::String MEZZ_LIB RightTrim(const Mezzanine::StringView Text);

        /// @copydoc RightTrim
        /// @details This is delegates its work to RightTrim, the name just exists to simplify.
        Mezzanine::String MEZZ_LIB rtrim(const Mezzanine::StringView Text);

        /// @brief Makes a c style strong all lowercase with respect to the current locale.
        /// @param StringToConvert This string is actually changed.
        /// @return for convenience purposes the string is also returned.
        Mezzanine::String MEZZ_LIB AllLower(const Mezzanine::StringView StringToConvert);

        /// @brief Checks to see if a string is safe to use as a file name.
        /// @param Filename The string to be checked.
        /// @return Returns true if the provided string contains invalid characters for a file name.
        Mezzanine::Boole MEZZ_LIB IsUnsafeForFileName(const Mezzanine::StringView Filename);

        /// @brief Get a copy of the passed filename with every unsafe character removed.
        /// @param OriginalFilename A string something like the filename you want.
        /// @return A string you can actually use as a file name.
        Mezzanine::String MEZZ_LIB SanitizeFileName(const Mezzanine::StringView OriginalFilename);

        /// @brief Checks to see if a string is safe to use as a process command.
        /// @param Process The strong to be checked.
        /// @return Returns true if the provided string contains invalid characters for a process command.
        Mezzanine::Boole MEZZ_LIB IsUnsafeForProcessCommand(const Mezzanine::StringView Process);

        /// @brief Get a copy of the passed possible process command with every unsafe character removed.
        /// @param OriginalProcess A string something like the process you want to launch.
        /// @return A string you can actually use as a file name.
        Mezzanine::String MEZZ_LIB SanitizeProcessCommand(const Mezzanine::StringView OriginalProcess);
    }// Testing
}// Mezzanine

#endif

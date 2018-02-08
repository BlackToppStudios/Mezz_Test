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

/// @file
/// @brief The implementation of items important a commandline tool to work correctly without need to be available to
/// test developers.

#include "ProcessTools.h"
#include "StringManipulation.h"
#include "SuppressWarnings.h"

#include <exception>
#include <cstdlib>

#include <iostream>
#include <limits>

namespace Mezzanine
{
    namespace Testing
    {
        SAVE_WARNING_STATE
        SUPPRESS_GCC_WARNING("-Wunused-result") // The result from std::system is useless here.
        Mezzanine::String RunCommand(const Mezzanine::String& Command, const Mezzanine::String& TempFileName)
        {
            const Mezzanine::String SafeCommand(SanitizeProcessCommand(Command));
            const Mezzanine::String SafeTempFileName(SanitizeProcessCommand(TempFileName));
            if(SafeCommand != Command)
                { throw std::runtime_error("Command name included unsafe characters, it would not run correctly."); }
            const Mezzanine::String CommandToRun(SafeCommand + " 2>&1 > " + SafeTempFileName);
            int ResultThatIsIgnored = std::system(CommandToRun.c_str());

            // The return of std::system is only defined in certain situations, and not this one, so this result is
            // useless, but we cannot ignore because of hard to suppress warnings.
            if(ResultThatIsIgnored > std::numeric_limits<int>::max()) { std::cout << ""; }

            return GetFileContents(SafeTempFileName);
        }
        RESTORE_WARNING_STATE

        SAVE_WARNING_STATE
        SUPPRESS_CLANG_WARNING("-Wsign-conversion") // std::streamoff are signed with the string constructor takes
        // size_type which is unsigned. So this is only good for files with fewer than 2^31 bytes.
        SUPPRESS_GCC_WARNING("-Wconversion") // The same issue but an extra warning GCC raises.
        SUPPRESS_VC_WARNING(4244) // Same conversion issue, why is there not a better way in std to handle this.
        SUPPRESS_VC_WARNING(4365) // Why does the same code throw multiple warnings
        Mezzanine::String GetFileContents(const Mezzanine::String& Filename)
        {
            std::ifstream ResultReader(Filename, std::ios::binary | std::ios::ate);
            std::streamoff FileSize{ResultReader.tellg()};
            ResultReader.seekg(std::ios::beg);
            String FileContents(FileSize,0);
            ResultReader.read(&FileContents[0], FileSize);
            return FileContents;
        }
        RESTORE_WARNING_STATE

    }// Testing
}// Mezzanine

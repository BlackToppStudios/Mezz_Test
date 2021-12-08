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

/// @file
/// @brief The implementation of the string manipulation functions the unit tests use

#include "TestEnumerations.h"
#include "StringManipulation.h"

#include "Trace.h"

#include <algorithm>

namespace {
    /// @brief Checks to see if a character is invalid to use in a file name.
    /// @tparam CharType The type of character to be checked.
    /// @param ToCheck The character to be checked.
    /// @return Returns true if the provided character is unsafe for use in a file name, false otherwise.
    template<typename CharType>
    constexpr Mezzanine::Boole IsInvalidFileNameChar(const CharType ToCheck)
    {
        switch(ToCheck)
        {
            case '|': case '>': case '<': case '?': case ':': case '%': case '"': case '=':
            case '+': case ',': case '!': case '@': case '[': case ']': case '*': case ';':
                return true;
                break;
            default:
                break;
        }
        return false;
    }

    /// @brief Checks to see if a character is invalid to use in a process command.
    /// @tparam CharType The type of character to be checked.
    /// @param ToCheck The character to be checked.
    /// @return Returns true if the provided character is unsafe for use in a process command, false otherwise.
    template<typename CharType>
    constexpr Mezzanine::Boole IsInvalidProcessCommandChar(const CharType ToCheck)
    {
        switch(ToCheck)
        {
            case '|': case '>': case '<':
                return true;
                break;
            default:
                break;
        }
        return false;
    }
}

namespace Mezzanine
{
    namespace Testing
    {
        Mezzanine::String RightTrim(const Mezzanine::StringView Text)
        {
            Mezzanine::String HayStack(Text);
            Mezzanine::StringView::size_type found{HayStack.find_last_not_of(" \n\r\t")};
            if (found != String::npos)
                { HayStack.erase(found+1); }
            else
                { HayStack.clear(); } // HayStack is all whitespace
            return HayStack;
        }

        Mezzanine::String rtrim(const Mezzanine::StringView Text)
            { return RightTrim(Text); }

        Mezzanine::String AllLower(const Mezzanine::StringView StringToConvert)
        {
            using StringChar = Mezzanine::String::value_type;

            Mezzanine::String Results;
            Results.reserve(StringToConvert.size());

            std::for_each(
                StringToConvert.begin(), StringToConvert.end(),
                [&Results](StringChar Letter)
                    { Results.push_back( static_cast<StringChar>(tolower(Letter)) ); }
            );

            return Results;
        }

        Mezzanine::Boole IsUnsafeForFileName(const Mezzanine::StringView Filename)
        {
            for(const Mezzanine::StringView::value_type& OneChar : Filename)
            {
                if( IsInvalidFileNameChar(OneChar) ) {
                    return true;
                }
            }
            return false;
        }

        Mezzanine::String SanitizeFileName(const Mezzanine::StringView OriginalFilename)
        {
            Mezzanine::String Results(OriginalFilename);
            for(Mezzanine::StringView::value_type& OneChar : Results)
            {
                if( IsInvalidFileNameChar(OneChar) ) {
                    OneChar = '_';
                }
            }
            return Results;
        }

        Mezzanine::Boole IsUnsafeForProcessCommand(const Mezzanine::StringView Process)
        {
            for(const Mezzanine::StringView::value_type& OneChar : Process)
            {
                if( IsInvalidProcessCommandChar(OneChar) ) {
                    return true;
                }
            }
            return false;
        }

        Mezzanine::String SanitizeProcessCommand(const Mezzanine::StringView OriginalProcess)
        {
            Mezzanine::String Results(OriginalProcess);
            for(Mezzanine::StringView::value_type& OneChar : Results)
            {
                if( IsInvalidProcessCommandChar(OneChar) ) {
                    OneChar = '_';
                }
            }
            return Results;
        }

    }// Testing
}// Mezzanine

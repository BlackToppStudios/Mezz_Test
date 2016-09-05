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
#ifndef Mezz_Test_unittestenumerations_cpp
#include "supresswarnings.h"
SAVE_WARNING_STATE
SUPPRESS_CLANG_WARNING("-Wunused-macros")
#define Mezz_Test_unittestenumerations_cpp
RESTORE_WARNING_STATE

/// @file
/// @brief The implmentation of a few functions used with the enumuration in the unit tests

#include "datatypes.h"

#include "testenumerations.h"

#include <stdexcept>

namespace Mezzanine
{
    namespace Testing
    {
        SAVE_WARNING_STATE
        SUPPRESS_GCC_WARNING("-Wreturn-type") // Control cannot reach the end of this function, because the
                                                    // warning for incomplete case statements will stop compilation.
        Mezzanine::String TestResultToString(TestResult Convertable)
        {
            switch(Convertable)
            {
                case Success:
                    return SuccessString;
                case Warning:
                    return WarningString;
                case Skipped:
                    return SkippedString;
                case Cancelled:
                    return CancelledString;
                case Inconclusive:
                    return InconclusiveString;
                case Failed:
                    return FailedString;
                case Unknown:
                    return UnknownString;
                case NotApplicable:
                    return NotApplicableString;
            }
        }
        RESTORE_WARNING_STATE

        TestResult StringToTestResult(Mezzanine::String Text)
        {
            if(Text.size()==0)
                { throw std::invalid_argument("Cannot convert to TestResult from empty String"); }

            switch(Text.at(0))
            {
                case 'S':
                    if ( SuccessString == Text )
                        { return Success; }
                    else if ( SkippedString == Text )
                        { return Skipped; }
                    else
                        { throw std::invalid_argument("Cannot convert to TestResult from text(S) " + Text); }
                case 'W':
                    if ( WarningString == Text )
                        { return Warning;}
                    else
                        { throw std::invalid_argument("Cannot convert to TestResult from text(W) " + Text); }
                case 'C':
                    if ( CancelledString == Text )
                        { return Cancelled;}
                    else
                        { throw std::invalid_argument("Cannot convert to TestResult from text(C) " + Text); }
                case 'I':
                    if ( InconclusiveString == Text )
                        { return Inconclusive;}
                    else
                        { throw std::invalid_argument("Cannot convert to TestResult from text(I) " + Text); }
                case 'U':
                    if ( UnknownString == Text )
                        { return Unknown;}
                    else
                        { throw std::invalid_argument("Cannot convert to TestResult from text(U) " + Text); }
                case 'F':
                    if ( FailedString == Text )
                        { return Failed;}
                    else
                        { throw std::invalid_argument("Cannot convert to TestResult from text(F) " + Text); }
                case 'N':
                    if ( NotApplicableString == Text )
                        { return NotApplicable;}
                    else
                        { throw std::invalid_argument("Cannot convert to TestResult from text(F) " + Text); }
                default:
                    { throw std::invalid_argument("Cannot convert to TestResult from text() " + Text); }
            }
        }

        Mezzanine::Int32 TestResultToInt(TestResult Convertable)
            { return Mezzanine::Int32(Convertable); }

        Mezzanine::UInt32 TestResultToUnsignedInt(TestResult Convertable)
            { return Mezzanine::UInt32(TestResultToInt(Convertable)); }

        TestResult IntToTestResult(Mezzanine::Int32 Convertable)
            { return TestResult(Convertable); }

        TestResult IntToTestResult(Mezzanine::UInt32 Convertable)
            { return TestResult(Convertable); }


    }// Testing
}// Mezzanine

#endif

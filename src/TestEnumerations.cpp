// © Copyright 2010 - 2019 BlackTopp Studios Inc.
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
/// @brief The implmentation of a few functions used with the enumuration in the unit tests.

#include "DataTypes.h"

#include "TestEnumerations.h"

SAVE_WARNING_STATE
SUPPRESS_VC_WARNING(4668)

    #include <stdexcept>

RESTORE_WARNING_STATE

namespace Mezzanine
{
    namespace Testing
    {
        Mezzanine::String TestResultToString(TestResult Convertable)
        {
            switch(Convertable)
            {
                case TestResult::Success:
                    return SuccessString;
                case TestResult::Warning:
                    return WarningString;
                case TestResult::Skipped:
                    return SkippedString;
                case TestResult::Cancelled:
                    return CancelledString;
                case TestResult::Inconclusive:
                    return InconclusiveString;
                case TestResult::Failed:
                    return FailedString;
                case TestResult::Unknown:
                    return UnknownString;
                case TestResult::NotApplicable:
                    return NotApplicableString;
            }
            return NotApplicableString;
        }

        String TestResultToFixedBoxString(TestResult Convertable)
        {
            switch(Convertable)
            {
                case TestResult::Success:
                    return "[    " + SuccessString + "    ]";
                case TestResult::Warning:
                    return "[    " + WarningString + "    ]";
                case TestResult::Skipped:
                    return "[    " + SkippedString + "    ]";
                case TestResult::Cancelled:
                    return "[   " + CancelledString + "   ]";
                case TestResult::Inconclusive:
                    return "[ " + InconclusiveString + "  ]";
                case TestResult::Failed:
                    return "[    " + FailedString + "     ]";
                case TestResult::Unknown:
                    return "[    " + UnknownString + "    ]";
                case TestResult::NotApplicable:
                    return "[ " + NotApplicableString + " ]";
            }
            return NotApplicableString;
        }

        Mezzanine::Int32 TestResultToInt(TestResult Convertable)
            { return Mezzanine::Int32(Convertable); }

        Mezzanine::UInt32 TestResultToUnsignedInt(TestResult Convertable)
            { return Mezzanine::UInt32(TestResultToInt(Convertable)); }

        std::ostream& operator<<(std::ostream& Stream, TestResult Output)
            { return Stream << TestResultToString(Output); }

        TestResult StringToTestResult(Mezzanine::String Text)
        {
            if(Text.size()==0)
                { throw std::invalid_argument("Cannot convert to TestResult from empty String"); }

            switch(Text.at(0))
            {
                case 'S':
                    if ( SuccessString == Text )
                        { return TestResult::Success; }
                    else if ( SkippedString == Text )
                        { return TestResult::Skipped; }
                    else
                        { throw std::invalid_argument("Cannot convert to TestResult from text(S) " + Text); }
                case 'W':
                    if ( WarningString == Text )
                        { return TestResult::Warning;}
                    else
                        { throw std::invalid_argument("Cannot convert to TestResult from text(W) " + Text); }
                case 'C':
                    if ( CancelledString == Text )
                        { return TestResult::Cancelled;}
                    else
                        { throw std::invalid_argument("Cannot convert to TestResult from text(C) " + Text); }
                case 'I':
                    if ( InconclusiveString == Text )
                        { return TestResult::Inconclusive;}
                    else
                        { throw std::invalid_argument("Cannot convert to TestResult from text(I) " + Text); }
                case 'U':
                    if ( UnknownString == Text )
                        { return TestResult::Unknown;}
                    else
                        { throw std::invalid_argument("Cannot convert to TestResult from text(U) " + Text); }
                case 'F':
                    if ( FailedString == Text )
                        { return TestResult::Failed;}
                    else
                        { throw std::invalid_argument("Cannot convert to TestResult from text(F) " + Text); }
                case 'N':
                    if ( NotApplicableString == Text )
                        { return TestResult::NotApplicable;}
                    else
                        { throw std::invalid_argument("Cannot convert to TestResult from text(F) " + Text); }
                default:
                    { throw std::invalid_argument("Cannot convert to TestResult from text() " + Text); }
            }
        }

    }// Testing
}// Mezzanine

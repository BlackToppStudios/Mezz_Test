// © Copyright 2010 - 2017 BlackTopp Studios Inc.
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
/// @brief The definition of the string manipulation functions the unit tests use

#include "MezzTest.h"

namespace Mezzanine
{
    namespace Testing
    {

        TestData::TestData(const String& Name,
                 TestResult Result,
                 const String& FuncName,
                 const String& File,
                 Mezzanine::Whole Line)
            : TestName(Name), FunctionName(FuncName), FileName(File), LineNumber(Line), Results(Result)
        {}

        bool TestData::operator<(const TestData& Rhs) const
            { return this->TestName < Rhs.TestName; }

        TestData StringToTestData(Mezzanine::String Line)
        {
            TestData Results;
            size_t LastSpace=Line.rfind(' ');
            Results.Results = StringToTestResult(Line.substr(LastSpace+1));
            Results.TestName=rtrim(Line.substr(0,LastSpace));
            return Results;
        }

        String EscapeTestNameString(const Mezzanine::String& RawName)
        {
            Mezzanine::String Results;
            Results.reserve(RawName.size() + RawName.size()/10); // A little extra space in case we need to escape stuff
            for(const char& OneLetter : RawName)
            {
                switch (OneLetter)
                {
                    case '\n':
                        Results += "\\n";
                        break;
                    case '\\':
                        Results += "\\\\";
                        break;
                    default:
                        Results.push_back(OneLetter);
                }
            }
            return Results;
        }

        String UnescapeTestNameString(const Mezzanine::String& MungedName)
        {
            Mezzanine::String Results;
            Results.reserve(MungedName.size());
            for(Mezzanine::String::const_iterator IterOneLetter = MungedName.cbegin();
                MungedName.cend() != IterOneLetter;
                IterOneLetter++)
            {
                if('\\' == *IterOneLetter)
                {
                    IterOneLetter++;
                    switch(*IterOneLetter)
                    {
                        case 'n':
                            Results.push_back('\n');
                            break;
                        case '\\':
                            Results.push_back('\\');
                            break;
                        default:
                            Results.push_back('\\');
                            Results.push_back(*IterOneLetter);
                    }

                } else {
                    Results.push_back(*IterOneLetter);
                }
            }
            return Results;
        }

        std::ostream& operator<<(std::ostream& Stream, const TestData& ToStream)
        {
            Stream << ' ' << TestResultToFixedBoxString(ToStream.Results) << "  "
                   <<  EscapeTestNameString(ToStream.TestName) << '\n';
            if(TestResult::Success != ToStream.Results)
            {
                Stream << "                      in function '" << ToStream.FunctionName
                       << "' at " << ToStream.FileName
                       << ":" << ToStream.LineNumber << ".\n";
            }
            return Stream;
        }

    }// Testing
}// Mezzanine

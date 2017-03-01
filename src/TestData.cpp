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

#include <algorithm>
#include <iostream>

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

        Boole TestData::operator<(const TestData& Rhs) const
            { return this->TestName < Rhs.TestName; }

        Boole TestData::operator==(const TestData& Rhs) const
        {
            return Rhs.LineNumber == this->LineNumber   &&  Rhs.Results == this->Results    &&
                   Rhs.TestName == this->TestName       &&  Rhs.FileName == this->FileName  &&
                   Rhs.FunctionName == this->FunctionName;
        }

        Boole TestData::operator!=(const TestData& Rhs) const
        {
            return Rhs.LineNumber != this->LineNumber   ||  Rhs.Results != this->Results    ||
                   Rhs.TestName != this->TestName       ||  Rhs.FileName != this->FileName  ||
                   Rhs.FunctionName != this->FunctionName;
        }

        SAVE_WARNING_STATE
        SUPPRESS_CLANG_WARNING("-Wexit-time-destructors")
        SUPPRESS_GCC_WARNING("-Wsign-conversion")

        TestData StringToTestData(Mezzanine::String Line)
        {
            using Iterator = Mezzanine::String::const_iterator;
            using Character = Mezzanine::String::value_type;

            const auto NotSpace = [](const Character& L) { return (L!=' ' && L!='\t' && L!='\r' && L!='\n'); };
            const String FunctionToken(" in function ");
            const String FileToken(" at ");

            // Skip whatever to get to '['
            Iterator OpenBracket = std::find(Line.cbegin(), Line.cend(), '[');
            if(Line.cend() == OpenBracket) { return TestData{}; }

            // Skip whitespace to Beginning of Result and Pull out the result
            Iterator ResultsBegin = std::find_if(OpenBracket+1, Line.cend(), NotSpace);
            if(Line.cend() == ResultsBegin) { return TestData{}; }
            Iterator ResultsEnd = std::find(ResultsBegin+1, Line.cend(), ' ');
            if(Line.cend() == ResultsEnd) { return TestData{}; }
            TestResult Result = StringToTestResult(String(ResultsBegin, ResultsEnd));

            // Skip whatever to '['
            Iterator CloseBracket = std::find(ResultsEnd+1, Line.cend(), ']');
            if(Line.cend() == CloseBracket) { return TestData{}; }

            // Read the Name
            Iterator NameBegin = std::find_if(CloseBracket+1, Line.cend(), NotSpace);
            if(Line.cend() == NameBegin) { return TestData{}; }
            Iterator NameEnd;
            if(TestResult::Success == Result)
                { NameEnd = Line.cend(); }
            else
            {
                String::size_type NameEndIndex = Line.find(FunctionToken, std::distance(Line.cbegin(), NameBegin));
                if(String::npos == NameEndIndex) { return TestData{}; }
                NameEnd = Line.cbegin() + NameEndIndex;
            }
            String Name(NameBegin, NameEnd);

            // If successful, we are done, If not find the function name
            if(TestResult::Success == Result) { return TestData{Name, Result}; }
            Iterator FunctionBegin = std::find(NameEnd+FunctionToken.size(), Line.cend(), '\'');
            if(Line.cend() == FunctionBegin) { return TestData{Name, Result, "Bad Function Name, No Start Quote"}; }
            Iterator FunctionEnd = std::find(FunctionBegin+1, Line.cend(), '\'');
            if(Line.cend() == FunctionEnd) { return TestData{Name, Result, "Bad Function Name, No End Quote"}; }
            String FunctionName(FunctionBegin+1, FunctionEnd);

            // Find the start of the file
            String::size_type FileStartHint = Line.find(FileToken, std::distance(Line.cbegin(), FunctionEnd));
            if(String::npos == FileStartHint) { return TestData{Name, Result, FunctionName, "Bad Filename no 'at'"}; }
            Iterator FileStart = Line.cbegin() + FileStartHint + FileToken.size();
            Iterator FileEnd = std::find(FileStart + 1, Line.cend(), ':');
            if(Line.cend() == FileEnd) { return TestData{Name, Result, FunctionName, "Bad Filename no ':'"}; }
            String FileName(FileStart, FileEnd);

            // Get Line number as a Whole
            String RestOfLine(Line.substr(std::distance(Line.cbegin(), FileEnd)+1));
            std::stringstream LineNumberConverter( RestOfLine );
            Whole LineNumber{0};
            LineNumberConverter >> LineNumber;

            return TestData{Name, Result, FunctionName, FileName, LineNumber};
        }
        RESTORE_WARNING_STATE

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
                   <<  EscapeTestNameString(ToStream.TestName);
            if(TestResult::Success != ToStream.Results)
            {
                Stream << " in function '" << ToStream.FunctionName
                       << "' at " << ToStream.FileName
                       << ":" << ToStream.LineNumber << '.';
            }
            Stream << '\n';
            return Stream;
        }

    }// Testing
}// Mezzanine

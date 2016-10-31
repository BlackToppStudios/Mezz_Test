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

/// @file
/// @brief The definition of the string manipulation functions the unit tests use

#include "MezzTest.h"

#include <vector>
#include <stdexcept>
#include <sstream>
#include <cassert>

using namespace Mezzanine;
using namespace std;

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

        TestData::TestData(pugi::xml_node Node)
            : TestName( Node.attribute("TestName").as_string() ),
              FunctionName( Node.attribute("FunctionName").as_string() ),
              FileName( Node.attribute("FileName").as_string() ),
              LineNumber( Node.attribute("LineNumber").as_uint() ),
              Results( StringToTestResult(Node.attribute ("Results").as_string()) )
        {}

        bool TestData::operator<(const TestData& Rhs) const
            { return this->TestName < Rhs.TestName; }

        String TestData::GetAsXML() const
        {
            std::stringstream Snippet;
            Snippet << "<TestData "
                    << "TestName=\"" << TestName << "\" "
                    << "Results=\"" << TestResultToString(Results) << "\" "
                    << "FunctionName=\"" << FunctionName << "\" "
                    << "FileName=\"" << FileName << "\" "
                    << "LineNumber=\"" << LineNumber << "\" "
                    << "/>";
            return Snippet.str();
        }

        int PrintList(CoreTestGroup& TestGroups)
        {
            for(CoreTestGroup::iterator Iter=TestGroups.begin(); Iter!=TestGroups.end(); ++Iter)
                { cout << Iter->first << std::endl; }
            return ExitSuccess;
        }

        TestData StringToTestData(Mezzanine::String Line)
        {
            TestData Results;
            size_t LastSpace=Line.rfind(' ');
            Results.Results = StringToTestResult(Line.substr(LastSpace+1));
            Results.TestName=rtrim(Line.substr(0,LastSpace));
            return Results;
        }
    }// Testing
}// Mezzanine

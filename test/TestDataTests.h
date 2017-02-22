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
#ifndef Mezz_Test_TestDataTests_h
#define Mezz_Test_TestDataTests_h

/// @file
/// @brief A Test header suitable for copying as a starting point for tests.

// Add other headers you need here
#include "MezzTest.h"

#include <vector>
#include <algorithm>
#include <sstream>

using Mezzanine::Testing::TestData;
using Mezzanine::Testing::EscapeTestNameString;
using Mezzanine::Testing::UnescapeTestNameString;
using Mezzanine::Testing::TestResult;
using Mezzanine::String;

/// @brief Tests for the class to store test data results.
AUTOMATIC_TEST_GROUP(TestDataTests, TestData)
{
    // Simple construction tests
    TestData DefaultConstructed;
    TEST_EQUAL("TestDataDefaultConstruction.TestName",      String(""),             DefaultConstructed.TestName);
    TEST_EQUAL("TestDataDefaultConstruction.Results",       TestResult::Success,    DefaultConstructed.Results);
    TEST_EQUAL("TestDataDefaultConstruction.FunctionName",  String(""),             DefaultConstructed.FunctionName);
    TEST_EQUAL("TestDataDefaultConstruction.FileName",      String(""),             DefaultConstructed.FileName);
    TEST_EQUAL("TestDataDefaultConstruction.LineNumber",    Mezzanine::Whole{0},    DefaultConstructed.LineNumber);

    TestData Constructed("Name", TestResult::Failed, "Func", "file.cpp", 42);
    TEST_EQUAL("TestDataConstruction.TestName",     String("Name"),         Constructed.TestName);
    TEST_EQUAL("TestDataConstruction.Results",      TestResult::Failed,     Constructed.Results);
    TEST_EQUAL("TestDataConstruction.FunctionName", String("Func"),         Constructed.FunctionName);
    TEST_EQUAL("TestDataConstruction.FileName",     String("file.cpp"),     Constructed.FileName);
    TEST_EQUAL("TestDataConstruction.LineNumber",   Mezzanine::Whole{42},   Constructed.LineNumber);


    // Sorting
    std::vector<TestData> Sorted = {TestData("Ocelot"), TestData("Aardvark"), TestData("Zebra")};
    std::sort(Sorted.begin(), Sorted.end());
    TEST_EQUAL("TestDataSorting.A", String("Aardvark"), Sorted[0].TestName);
    TEST_EQUAL("TestDataSorting.B", String("Ocelot"), Sorted[1].TestName);
    TEST_EQUAL("TestDataSorting.C", String("Zebra"), Sorted[2].TestName);

    // Escaping tests
    TEST_EQUAL("EscapeTestNameString-PassThrough",      String("Foo"),          EscapeTestNameString("Foo"));
    TEST_EQUAL("EscapeTestNameString-Slashes",          String("Foo\\\\Bar"),   EscapeTestNameString("Foo\\Bar"));
    TEST_EQUAL("EscapeTestNameString-Newline",          String("Foo\\nBar"),    EscapeTestNameString("Foo\nBar"));
    TEST_EQUAL("EscapeTestNameString-IgnoredTab",       String("Foo\tBar"),     EscapeTestNameString("Foo\tBar"));
    TEST_EQUAL("EscapeTestNameString-SlashT",           String("Foo\\\\tBar"),  EscapeTestNameString("Foo\\tBar"));

    TEST_EQUAL("UnescapeTestNameString-PassThrough",    String("Foo"),          UnescapeTestNameString("Foo"));
    TEST_EQUAL("UnescapeTestNameString-Slashes",        String("Foo\\Bar"),     UnescapeTestNameString("Foo\\\\Bar"));
    TEST_EQUAL("UnescapeTestNameString-Newline",        String("Foo\nBar"),     UnescapeTestNameString("Foo\\nBar"));
    TEST_EQUAL("UnescapeTestNameString-IgnoredTab",     String("Foo\\tBar"),    UnescapeTestNameString("Foo\\tBar"));
    TEST_EQUAL("UnescapeTestNameString-IgnoredSlashT",  String("Foo\\tBar"),    UnescapeTestNameString("Foo\\tBar"));
    TEST_EQUAL("UnescapeTestNameString-EscapedSlashT",  String("Foo\\tBar"),    UnescapeTestNameString("Foo\\\\tBar"));

    {
        TestData Streamable("StreamedTestNameWarning", TestResult::Warning, "FunctionWithProblem", "file.cpp", 123);
        std::stringstream SampleStream;
        SampleStream << Streamable;
        TEST_EQUAL("StreamSimpleWarning",
                   String(" [    Warning    ]  StreamedTestNameWarning\n"
                          "                      in function 'FunctionWithProblem' at file.cpp:123.\n"),
                   SampleStream.str())
    }

    {
        TestData Streamable("StreamedTestNameSuccess", TestResult::Success, "FunctionWithNoProblem", "file2.cpp", 456);
        std::stringstream SampleStream;
        SampleStream << Streamable;
        TEST_EQUAL("StreamSimpleSuccess",
                   String(" [    Success    ]  StreamedTestNameSuccess\n"),
                   SampleStream.str())
    }

    {
        TestData Streamable("Streamed\nTest\nName\nNewline", TestResult::Success, "FunctionNewlines", "file3.cpp", 789);
        std::stringstream SampleStream;
        SampleStream << Streamable;
        TEST_EQUAL("StreamNewline",
                   String(" [    Success    ]  Streamed\\nTest\\nName\\nNewline\n"),
                   SampleStream.str())
    }
}

#endif

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
using Mezzanine::Testing::StringToTestData;
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

    // Equality
    TEST_EQUAL("DefaultEqual", TestData(),  TestData());
    TEST_EQUAL("InequalName", false,
               TestData("foob", TestResult::Skipped) == TestData("foo", TestResult::Skipped));
    TEST_EQUAL("InequalResult", false,
               TestData("foo", TestResult::Warning) == TestData("foo", TestResult::Skipped));
    TEST_EQUAL("InequalFunc", false,
               TestData("foo", TestResult::Warning, "bar") == TestData("foo", TestResult::Warning, "baz"));
    TEST_EQUAL("InequalFile", false,
               TestData("foo", TestResult::Warning, "baz", "a.c") ==
               TestData("foo", TestResult::Warning, "baz", "a.h"));
    TEST_EQUAL("InequalLine", false,
               TestData("foo", TestResult::Warning, "baz", "a.h", 12) ==
               TestData("foo", TestResult::Warning, "baz", "a.h", 13));
    TEST_EQUAL("EqualFull", false,
               TestData("foo", TestResult::Warning, "bar", "a.h", 12) ==
               TestData("foo", TestResult::Warning, "baz", "a.h", 12));

    // Inequality
    TEST_EQUAL("NotEqualName", true,
               TestData("bar", TestResult::Warning, "lorem", "a.h", 12) !=
               TestData("baz", TestResult::Warning, "lorem", "a.h", 12));
    TEST_EQUAL("NotEqualResult", true,
               TestData("bar", TestResult::Unknown, "lorem", "a.h", 12) !=
               TestData("bar", TestResult::Warning, "lorem", "a.h", 12));
    TEST_EQUAL("NotEqualFunction", true,
               TestData("bar", TestResult::Warning, "lorem", "a.h", 12) !=
               TestData("bar", TestResult::Warning, "ipsum", "a.h", 12));
    TEST_EQUAL("NotEqualFile", true,
               TestData("bar", TestResult::Warning, "lorem", "b.h", 12) !=
               TestData("bar", TestResult::Warning, "lorem", "a.h", 12));
    TEST_EQUAL("NotEqualLine", true,
               TestData("bar", TestResult::Warning, "lorem", "a.h", 12) !=
               TestData("bar", TestResult::Warning, "lorem", "a.h", 11));

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

    // Serialization
    {
        TestData Streamable("StreamedTestNameWarning", TestResult::Warning, "FunctionWithProblem", "file.cpp", 123);
        std::stringstream SampleStream;
        SampleStream << Streamable;
        TEST_EQUAL("StreamSimpleWarning",
            String(" [    Warning    ]  StreamedTestNameWarning in function 'FunctionWithProblem' at file.cpp:123.\n"),
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

    // Deserialization
    TestData FromLine{ StringToTestData(" [    Success    ]  Fred") };
    TEST_EQUAL("StringToTestData-Success-Name",         "Fred",                 FromLine.TestName);
    TEST_EQUAL("StringToTestData-Success-Results",      TestResult::Success,    FromLine.Results);
    TEST_EQUAL("StringToTestData-Success-FunctionName", String(""),             FromLine.FunctionName);
    TEST_EQUAL("StringToTestData-Success-LineNumber",   Mezzanine::Whole{0},    FromLine.LineNumber);
    TEST_EQUAL("StringToTestData-Success-FileName",     String(""),             FromLine.FileName);

    TestData FailedLine{ StringToTestData("[    Failed     ]  Scooby in function 'snack' at MysteryMachine.h:147.") };
    TEST_EQUAL("StringToTestData-Failed-Name",          "Scooby",                   FailedLine.TestName);
    TEST_EQUAL("StringToTestData-Failed-Results",       TestResult::Failed,         FailedLine.Results);
    TEST_EQUAL("StringToTestData-Failed-FunctionName",  String("snack"),            FailedLine.FunctionName);
    TEST_EQUAL("StringToTestData-Failed-LineNumber",    Mezzanine::Whole{147},      FailedLine.LineNumber);
    TEST_EQUAL("StringToTestData-Failed-FileName",      String("MysteryMachine.h"), FailedLine.FileName);

    TEST_STRING_CONTAINS("TestResultToString-Warning",
                         String("Warning"),
                         TestResultToString(TestResult::Warning));
    TEST_STRING_CONTAINS("TestResultToString-Cancel",
                         String("Cancel"),
                         TestResultToString(TestResult::Cancelled));
    TEST_STRING_CONTAINS("TestResultToString-Inconclusive",
                         String("Inconclusive"),
                         TestResultToString(TestResult::Inconclusive));
    TEST_STRING_CONTAINS("TestResultToString-Failed",
                         String("Failed"),
                         TestResultToString(TestResult::Failed));
    TEST_STRING_CONTAINS("TestResultToString-Unknown",
                         String("Unknown"),
                         TestResultToString(TestResult::Unknown));
    TEST_STRING_CONTAINS("TestResultToString-NotApplicable",
                         String("NotApplicable"),
                         TestResultToString(TestResult::NotApplicable));

    TEST_STRING_CONTAINS("TestResultToFixedBoxString-Warning",
                         String("Warning"),
                         TestResultToFixedBoxString(TestResult::Warning));
    TEST_STRING_CONTAINS("TestResultToFixedBoxString-Cancel",
                         String("Cancel"),
                         TestResultToFixedBoxString(TestResult::Cancelled));
    TEST_STRING_CONTAINS("TestResultToFixedBoxString-Inconclusive",
                         String("Inconclusive"),
                         TestResultToFixedBoxString(TestResult::Inconclusive));
    TEST_STRING_CONTAINS("TestResultToFixedBoxString-Failed",
                         String("Failed"),
                         TestResultToFixedBoxString(TestResult::Failed));
    TEST_STRING_CONTAINS("TestResultToFixedBoxString-Unknown",
                         String("Unknown"),
                         TestResultToFixedBoxString(TestResult::Unknown));
    TEST_STRING_CONTAINS("TestResultToFixedBoxString-NotApplicable",
                         String("NotApplicable"),
                         TestResultToFixedBoxString(TestResult::NotApplicable));

    TestData BogusLine{ StringToTestData(" Shaggy is probably a stoner, but there was never any on screen.") };
    TEST_EQUAL("StringToTestData-CompletelyBogus", TestData{}, BogusLine);


    TEST_THROW("StringToTestData-BadStatusThrows",
               std::invalid_argument,
               []{ StringToTestData(" [    Daphne    ]  Fred"); });

    // Assignment and move tests
    TestData DestAssign("DestinationAssign", TestResult::Unknown);
    TestData DestMove("DestinationMove", TestResult::Unknown);
    TestData Source("Source", TestResult::Success);

    TEST_EQUAL("PreCopyTargetDestAssign",   TestData("DestinationAssign", TestResult::Unknown), DestAssign);
    TEST_EQUAL("PreCopyTargetDestMove",     TestData("DestinationMove", TestResult::Unknown),   DestMove);
    TEST_EQUAL("PreCopyTargetSource",       TestData("Source", TestResult::Success),            Source);

    DestAssign = Source;
    DestMove = std::move(Source);

    TEST_EQUAL("PostCopyTargetDestAssign",  TestData("Source", TestResult::Success),    DestAssign);
    TEST_EQUAL("PostCopyTargetDestMove",    TestData("Source", TestResult::Success),    DestMove);

    // Add throw tests
}

#endif

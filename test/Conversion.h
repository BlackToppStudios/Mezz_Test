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
#ifndef Mezz_Test_Conversion_h
#define Mezz_Test_Conversion_h

/// @file
/// @brief Tests for conversions used internally and presented as part of the Mezz Test API

#include "MezzTest.h"

SAVE_WARNING_STATE
SUPPRESS_CLANG_WARNING("-Wweak-vtables") // We really don't care, because this will this will be recompiled each test.
SUPPRESS_CLANG_WARNING("-Wpadded") // Test classes will likely need to be padded and we don't care.

class ConversionTests : public Mezzanine::Testing::UnitTestGroup
{
    public:
        /// @brief Default Constructor
        ConversionTests() = default;

        /// @brief Default Deconstructor
        virtual ~ConversionTests() = default;

        // This is used as the name of the test on the command prompt
        virtual Mezzanine::String Name() override
            { return "conversion"; }

        void RunAutomaticTests()
        {
            TEST_THROW("EmptyStringsAreNotTestResults",
                       std::invalid_argument,
                       []{ Mezzanine::Testing::StringToTestResult(""); }
                       );

            TEST_EQUAL("SuccessStringConvertsToResult",
                       Mezzanine::Testing::TestResult::Success,
                       Mezzanine::Testing::StringToTestResult("Success")
                       );
            TEST_EQUAL("SkippedStringConvertsToResult",
                       Mezzanine::Testing::TestResult::Skipped,
                       Mezzanine::Testing::StringToTestResult("Skipped")
                       );
            TEST_THROW("BadSStringTestResults",
                       std::invalid_argument,
                       []{ Mezzanine::Testing::StringToTestResult("SuplexTheTestResults"); }
                       );

            TEST_EQUAL("WarningStringConvertsToResult",
                       Mezzanine::Testing::TestResult::Warning,
                       Mezzanine::Testing::StringToTestResult("Warning")
                       );
            TEST_THROW("BadWStringTestResults",
                       std::invalid_argument,
                       []{ Mezzanine::Testing::StringToTestResult("Wrong(YesThisIs)"); }
                       );

            TEST_EQUAL("CancelledStringConvertsToResult",
                       Mezzanine::Testing::TestResult::Cancelled,
                       Mezzanine::Testing::StringToTestResult("Cancelled")
                       );
            TEST_THROW("BadCStringTestResults",
                       std::invalid_argument,
                       []{ Mezzanine::Testing::StringToTestResult("ClearlyWrong(YesThisIs)"); }
                       );

            TEST_EQUAL("InconclusiveStringConvertsToResult",
                       Mezzanine::Testing::TestResult::Inconclusive,
                       Mezzanine::Testing::StringToTestResult("Inconclusive")
                       );
            TEST_THROW("BadIStringTestResults",
                       std::invalid_argument,
                       []{ Mezzanine::Testing::StringToTestResult("Indecipherable"); }
                       );

            TEST_EQUAL("InconclusiveStringConvertsToResult",
                       Mezzanine::Testing::TestResult::Inconclusive,
                       Mezzanine::Testing::StringToTestResult("Inconclusive")
                       );
            TEST_THROW("BadIStringTestResults",
                       std::invalid_argument,
                       []{ Mezzanine::Testing::StringToTestResult("Indecipherable"); }
                       );

            TEST_EQUAL("UnknownStringConvertsToResult",
                       Mezzanine::Testing::TestResult::Unknown,
                       Mezzanine::Testing::StringToTestResult("Unknown")
                       );
            TEST_THROW("BadUStringTestResults",
                       std::invalid_argument,
                       []{ Mezzanine::Testing::StringToTestResult("Unknowable"); }
                       );

            TEST_EQUAL("FailedStringConvertsToResult",
                       Mezzanine::Testing::TestResult::Failed,
                       Mezzanine::Testing::StringToTestResult("Failed")
                       );
            TEST_THROW("BadFStringTestResults",
                       std::invalid_argument,
                       []{ Mezzanine::Testing::StringToTestResult("FThatIsYouGradeNow"); }
                       );

            TEST_EQUAL("NotApplicableStringConvertsToResult",
                       Mezzanine::Testing::TestResult::NotApplicable,
                       Mezzanine::Testing::StringToTestResult("NotApplicable")
                       );
            TEST_THROW("BadNStringTestResults",
                       std::invalid_argument,
                       []{ Mezzanine::Testing::StringToTestResult("NeverApplicable"); }
                       );



            Mezzanine::Testing::TestData FromLine{ Mezzanine::Testing::StringToTestData("SampleTestName    Success") };
            TEST_EQUAL("LineToTestDataName", "SampleTestName", FromLine.TestName);
            TEST_EQUAL("LineToTestDataResults", Mezzanine::Testing::TestResult::Success, FromLine.Results);
        }
        bool HasAutomaticTests() const
            { return true; }


        //void RunSubprocessTest(const Mezzanine::String& Arg) // Add this parameter back in if you need it.
        void RunSubprocessTest(const Mezzanine::String&)
        {
            // Tests with a high risk of crashing can go here

            // If you ever need to debug stuff in here be sure to pass "debugtests" to test executable

            // To enable this test group have HasSubprocessTest() return true
        }
        bool HasSubprocessTest() const
            { return false; }


        void RunInteractiveTests()
        {
            // Tests that require the mushy and weak flesh of human (Until our machine overlords rise up).

            // To enable this test group have HasInteractiveTests() return true
        }
        bool HasInteractiveTests() const
            { return false; }


};

RESTORE_WARNING_STATE
#endif

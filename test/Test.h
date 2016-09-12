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
#ifndef Mezz_Test_TheActualTests_h
#define Mezz_Test_TheActualTests_h

/// @file
/// @brief Header for unit tests for the testing framework.

// Add other headers you need here
#include "mezztest.h"

SAVE_WARNING_STATE
SUPPRESS_CLANG_WARNING("-Wweak-vtables") // We really don't care, because this will this will be recompiled each test.
SUPPRESS_CLANG_WARNING("-Wpadded") // Test classes will likely need to be padded and we don't care.

// This class is not called directly by the Unit Test framework and is just used by
// TestTests to verify that Failing works correctly.
class NegativeTestTests : public Mezzanine::Testing::UnitTestGroup
{
     public:
         NegativeTestTests() = default;
         virtual ~NegativeTestTests() = default;

         void RunAutomaticTests()
         {
             TEST("DefaultTestFailing", false);
             TEST_EQUAL("EqualityTestFailing", 1, 2);
             TEST_EQUAL_EPSILON("EqualEpsilonFailing",0.1,0.2);
             //TEST_EQUAL_MULTI_EPSILON("EqualEpsilonFailing",0.1,0.1,2);
             // TEST_EQUAL_EPSILON
             // TEST_EQUAL_MULTI_EPSILON
             // TEST_WARN
             // TEST_RESULT
             // TEST_THROW
             // TEST_NO_THROW
             // TEST_TIMED
         }
         bool HasAutomaticTests() const
             { return true; }
};

// This class is not called directly by the Unit Test framework and is just used by
// TestTests to verify that Warnings works correctly.
class WarningTestTests : public Mezzanine::Testing::UnitTestGroup
{
    public:
        WarningTestTests() = default;
        virtual ~WarningTestTests() = default;

        void RunAutomaticTests()
        {
            TEST_WARN("WarningTestFailing", false);
        }
        bool HasAutomaticTests() const
            { return true; }
};

class TestTests : public Mezzanine::Testing::UnitTestGroup
{
    public:
        TestTests() = default;
        virtual ~TestTests() = default;

        void RunAutomaticTests()
        {
            // Positive tests
            TEST("DefaultTestPassing", true);
            TEST_EQUAL("EqualityTestPassing", 1, 1);
            TEST_WARN("WarningTestPassing", true);
            TEST_EQUAL_EPSILON("EqualEpsilonPassing",0.1,0.1);
            //TEST_EQUAL_MULTI_EPSILON("EqualEpsilonPassing",0.1,0.1,2);

            // TEST_RESULT
            // TEST_THROW
            // TEST_NO_THROW
            // TEST_TIMED

            // Negative Tests
            class NegativeTestTests Negation;
            Negation.RunAutomaticTests();
            for(const Mezzanine::Testing::TestData& SingleResult : Negation)
                { TEST_EQUAL(SingleResult.TestName, Mezzanine::Testing::Failed, SingleResult.Results); }

            // Warning Tests
            class WarningTestTests Warnifier;
            Warnifier.RunAutomaticTests();
            for(const Mezzanine::Testing::TestData& SingleResult : Warnifier)
                { TEST_EQUAL(SingleResult.TestName, Mezzanine::Testing::Warning, SingleResult.Results); }

        }
        bool HasAutomaticTests() const
            { return true; }


        //void RunSubprocessTest(const Mezzanine::String& Arg) // Add this parameter back in if you need it.
        void RunSubprocessTest(const Mezzanine::String&)
            {}
        bool HasSubprocessTest() const
            { return false; }


        void RunInteractiveTests()
            {}
        bool HasInteractiveTests() const
            { return false; }
};

RESTORE_WARNING_STATE
#endif

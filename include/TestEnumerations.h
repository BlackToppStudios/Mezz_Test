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
#ifndef Mezz_Test_UnitTestEnumerations_h
#define Mezz_Test_UnitTestEnumerations_h

/// @file
/// @brief Enumerations and constant values associated with the Unit tests.

#include "DataTypes.h"
#include "SuppressWarnings.h"

namespace Mezzanine
{
    namespace Testing
    {

        /// @brief Return values from tests.
        /// @details These are loosely order from best to worse, in terms of
        /// what we want to see from our tests. This takes the mindset that
        /// knowing about a failure is better than not knowing about it.
        enum class TestResult
        {
            Success         = 0,        ///< Test was ran and appeared to work.
            Skipped         = 1,        ///< Test was simply not ran and this was intentional and does not mean failure.
            Cancelled       = 2,        ///< Was canceled by user, so success is unknown, but any user knows test was
                                        /// cancelled and the test is not failed
            Warning         = 3,        ///< Technically the test passed but there is something that is not quite right.
            Inconclusive    = 4,        ///< If a user answers that with "don't know" in a test that involved
                                        /// interaction, The user knows there is a potential issue.
            Failed          = 5,        ///< Known failure.
            Unknown         = 6,        ///< Since we don't know what happened this is the worst kind of failure.
            NotApplicable   = 7,        ///< This is not even a kind of failure, This is used to when referencing a
                                        /// test, so if this winds up coming out of a test, then something has failed.
            Highest = TestResult::NotApplicable  ///< Highest will always match the highest value of the class enum,
                                                 /// to make it easier to inspect.
        };

        // These global constructors are certainly safe. These reference nothing external and that is the reason
        // for the warning.
        SAVE_WARNING_STATE
        SUPPRESS_CLANG_WARNING("-Wexit-time-destructors")
        SUPPRESS_CLANG_WARNING("-Wglobal-constructors")
        /// @brief Corresponds to TestResult::Success.
        const Mezzanine::StringView SuccessString           ("Success");
        /// @brief Corresponds to TestResult::Skipped.
        const Mezzanine::StringView SkippedString           ("Skipped");
        /// @brief Corresponds to TestResult::Cancelled.
        const Mezzanine::StringView CancelledString         ("Cancelled");
        /// @brief Corresponds to TestResult::Warning.
        const Mezzanine::StringView WarningString           ("Warning");
        /// @brief Corresponds to TestResult::Inconclusive.
        const Mezzanine::StringView InconclusiveString      ("Inconclusive");
        /// @brief Corresponds to TestResult::Failed.
        const Mezzanine::StringView FailedString            ("Failed");
        /// @brief Corresponds to TestResult::Unknown.
        const Mezzanine::StringView UnknownString           ("Unknown");
        /// @brief Corresponds to TestResult::NotApplicable.
        const Mezzanine::StringView NotApplicableString     ("NotApplicable");

        /// @brief Corresponds to TestResult::Success, but in a box. Must contain the non-box version.
        const Mezzanine::StringView SuccessStringBox        ("[    Success    ]");
        /// @brief Corresponds to TestResult::Skipped, but in a box. Must contain the non-box version.
        const Mezzanine::StringView SkippedStringBox        ("[    Skipped    ]");
        /// @brief Corresponds to TestResult::Cancelled, but in a box. Must contain the non-box version.
        const Mezzanine::StringView CancelledStringBox      ("[   Cancelled   ]");
        /// @brief Corresponds to TestResult::Warning, but in a box. Must contain the non-box version.
        const Mezzanine::StringView WarningStringBox        ("[    Warning    ]");
        /// @brief Corresponds to TestResult::Inconclusive, but in a box. Must contain the non-box version.
        const Mezzanine::StringView InconclusiveStringBox   ("[ Inconclusive  ]");
        /// @brief Corresponds to TestResult::Failed, but in a box. Must contain the non-box version.
        const Mezzanine::StringView FailedStringBox         ("[    Failed     ]");
        /// @brief Corresponds to TestResult::Unknown, but in a box. Must contain the non-box version.
        const Mezzanine::StringView UnknownStringBox        ("[    Unknown    ]");
        /// @brief Corresponds to TestResult::NotApplicable, but in a box. Must contain the non-box version.
        const Mezzanine::StringView NotApplicableStringBox  ("[ NotApplicable ]");

        RESTORE_WARNING_STATE

        /// @brief This converts A test result enum value into a String matching the identifier name.
        /// @param Convertable A TestResult inclusively between Success and NotApplicable.
        /// @return  A string like "Success" or "Inconclusive" or similar.
        Mezzanine::StringView TestResultToString(TestResult Convertable);

        /// @brief This converts A test result into a String matching the in a fixed width box.
        /// @param Convertable A TestResult inclusively between Success and NotApplicable.
        /// @return  A string like "[    Success    ]" or "[    Warning    ]" or similar.
        Mezzanine::StringView MEZZ_LIB TestResultToFixedBoxString(TestResult Convertable);

        /// @brief Convert a TestResult to a number for other processing.
        /// @param Convertable The valid TestResults to be converted.
        /// @return A signed integer for representing Convertable.
        Mezzanine::Int32 MEZZ_LIB TestResultToInt(TestResult Convertable);

        /// @brief Convert a TestResult to a number for other processing.
        /// @param Convertable The valid TestResults to be converted.
        /// @return An unsigned integer for representing Convertable.
        Mezzanine::UInt32 MEZZ_LIB TestResultToUnsignedInt(TestResult Convertable);

        /// @brief Convert a number to a TestResult.
        /// @param Convertable The valid number representing a TestResult.
        /// @return A TestResults with the same internal representation as Convertable.
        /// @tparam T hopefully an integer type.
        template<typename T>
        TestResult IntToTestResult(const T& Convertable)
            { return TestResult(Convertable); }

        /// @brief Calls TestResultToString and sends that to the output stream
        /// @param Output The TestResult to emit.
        /// @param Stream The output stream to send the converted TestResult out.
        std::ostream& MEZZ_LIB operator<< (std::ostream& Stream, TestResult Output);

        /// @brief Roughly convert a String to a TestResult.
        /// @param Text If this matches a word like "Success", "Skipped", "Cancelled", "Inconclusive", "Failed",
        /// "Unknown" or "N/A" exactly as returned by @ref TestResultToString then a valid result can be returned.
        /// @return The @ref TestResult corresponding to the String passed in.
        /// @throw std::invalid_argument If the string passed in does not match a valid TestResult then this will throw
        /// an std::invalid_argument with a message describing why and some data about string parsing.
        TestResult MEZZ_LIB StringToTestResult(Mezzanine::String Text);
    }// Testing
}// Mezzanine

#endif

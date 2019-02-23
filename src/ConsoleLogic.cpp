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
/// @brief The implementation of items important for a commandline tool to work correctly without the need to be
/// available to test developers.

#include "ConsoleLogic.h"
#include "StringManipulation.h"
#include "UnitTestGroup.h"

SAVE_WARNING_STATE
SUPPRESS_VC_WARNING(4668) // A Preprocessor macro was used and never defined because the VS Headers are poorly written.

#include <vector>
#include <sstream>

#ifdef MEZZ_Windows
    #ifndef WIN32_LEAN_AND_MEAN
        #define WIN32_LEAN_AND_MEAN
        #define __UNDEF_LEAN_AND_MEAN
    #endif
    #include <windows.h>
    #ifdef __UNDEF_LEAN_AND_MEAN
        #undef WIN32_LEAN_AND_MEAN
        #undef __UNDEF_LEAN_AND_MEAN
    #endif
#else
    #include <unistd.h>
#endif

RESTORE_WARNING_STATE

namespace Mezzanine
{
    namespace Testing
    {
        void Usage(const String& ThisName, const Mezzanine::Testing::CoreTestGroup& TestGroups)
            { std::cerr << GetUsageString(ThisName, TestGroups) << std::endl; }

        Mezzanine::String GetUsageString(const Mezzanine::String& ThisName,
                                         const Mezzanine::Testing::CoreTestGroup& TestGroups)
        {
            return Mezzanine::String("\nUsage: ") + ThisName + (
                    " [help] [summary] [testlist] [interactive|automatic] [all]\n\t[skipfile] <Test Names>...\n\n"
                    "<Test Name>      Add this test to the list of tests to run.\n"
                    "Skip-<Test Name> Remove this from the list of tests to run.\n\n"
                    "All:             All test groups will be run.\n"
                    "Interactive:     Only interactive tests will be performed on specified test groups.\n"
                    "Automatic:       Only automated tests will be performed on specified test groups.\n"
                    "Summary:         Display a count of failures and successes.\n"
                    "SkipFile:        Do not store a copy of the results in TestResults.txt.\n"
                    "DebugTests:      Run tests in the current process in single thread. Skips crash protection,\n"
                    "                 but eases test debugging.\n"
                    "NoThreads:       Half of Debugtests, forces single threaded, but allows subprocesses"
                    "Help:            Display this message.\n\n"
                    "If only test group names are entered, then all tests in those groups are run.\n"
                    "This command is not case sensitive.\n\n"
                    "Current Test Groups:\n"
            ) + GetPrintableTestList(80, TestGroups); // This presumes the console is 80 chars wide.
        }

        Mezzanine::String GetPrintableTestList(const Mezzanine::String::size_type TargetWidth,
                                               const CoreTestGroup& TestGroups)
        {
            std::stringstream Assembler;

            // Replace with std::max_element once we get C++17.
            Mezzanine::String::size_type LongestName = 0;
            for(const std::pair<Mezzanine::String,UnitTestGroup*>& Entry: TestGroups)
            {
                if(Entry.first.size()>LongestName)
                    { LongestName=Entry.first.size(); }
            }

            Mezzanine::String::size_type ColumnWidth = LongestName+1;
            Mezzanine::String::size_type Column = 0;
            Mezzanine::String::size_type CurrentWidth=0;
            for(const std::pair<Mezzanine::String,UnitTestGroup*>& Entry: TestGroups)
            {
                if(0==Column)
                {
                    Assembler << "  ";
                    CurrentWidth = 2;
                } else {
                    Assembler << " ";
                    CurrentWidth++;
                }

                Assembler << Entry.second->Name() << " ";
                CurrentWidth += Entry.first.size() + 1;


                // Written this way because std::clamp would solve it in a simple formula but not until C++17.
                for(Mezzanine::String::size_type SpaceD=Entry.first.size()+1; SpaceD<=ColumnWidth; SpaceD++)
                {
                    Assembler << " ";
                    CurrentWidth++;
                }

                if(CurrentWidth>TargetWidth-LongestName-1)
                {
                    Assembler << '\n';
                    Column = 0;
                    CurrentWidth = 0;
                }
            }
            Assembler << '\n';
            return Assembler.str();
        }

        TestResult Char8ToTestResults(Mezzanine::Char8 Answer)
        {
            switch(static_cast<Mezzanine::Char8>(tolower(Answer)))
            {
                case 't': case 'y':
                    return Testing::TestResult::Success;
                case 'f': case 'n':
                    return Testing::TestResult::Failed;
                case 'w':
                    return Testing::TestResult::Warning;
                case 'c':
                    return Testing::TestResult::Cancelled;
                case 'u': case 'i':
                    return Testing::TestResult::Inconclusive;
                default:
                    return Testing::TestResult::Unknown;
            }
        }

    }// Testing
}// Mezzanine

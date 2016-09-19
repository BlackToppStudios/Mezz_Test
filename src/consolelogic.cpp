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
/// @brief The implementation of items important a commandline tool to work correctly without need to be available to
/// test developers.

#include "consolelogic.h"
#include "consolestringmanipulation.h"
#include "unittestgroup.h"

SAVE_WARNING_STATE
SUPPRESS_VC_WARNING(4668)

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

using namespace Mezzanine;
using namespace std;

namespace Mezzanine
{
    namespace Testing
    {
        int Usage(Mezzanine::String ThisName, CoreTestGroup& TestGroups)
        {
            cout    << "\nUsage: " << ThisName
                    << " [help] [summary] [testlist] [interactive|automatic] [all]\n\t[skipfile] Test Group Names ..."
                        << std::endl << std::endl
                    << "All:         All test groups will be run." << std::endl
                    << "Interactive: Only interactive tests will be performed on specified test groups." << std::endl
                    << "Automatic:   Only automated tests will be performed on specified test groups." << std::endl
                    << "Summary:     Display a count of failures and successes." << std::endl
                    << "testlist:    Output a list of all tests, one per line." << std::endl
                    << "skipfile:    Do not store a copy of the results in TestResults.txt." << std::endl
                    << "debugtests:  Run tests in the current process. Skips crash protection,"
                       << " but eases test debugging." << std::endl
                    << "Help:        Display this message." << std::endl << std::endl
                    << "If only test group names are entered, then all tests in those groups are run." << std::endl
                    << "This command is not case sensitive." << std::endl << std::endl
                    << "Current Test Groups: " << std::endl;

            Mezzanine::Whole LongestName = 0;
            for(std::map<Mezzanine::String,UnitTestGroup*>::iterator Iter=TestGroups.begin();
                Iter!=TestGroups.end();
                ++Iter)
            {
                if(Iter->first.size()>LongestName)
                    { LongestName=Iter->first.size();}
            }

            // This presumes the console it 80 chars wide
            Mezzanine::Whole TargetWidth=80;
            Mezzanine::Whole ColumnWidth = LongestName+1;
            Mezzanine::Whole Column = 0;
            Mezzanine::Whole CurrentWidth=0;
            for(std::map<Mezzanine::String,UnitTestGroup*>::iterator Iter=TestGroups.begin();
                Iter!=TestGroups.end();
                ++Iter)
            {
                if(0==Column)
                {
                    cout << "  ";
                    CurrentWidth = 2;
                } else {
                    cout << " ";
                    CurrentWidth ++;
                }

                cout << Iter->second->Name() << " ";
                CurrentWidth += Iter->first.size() + 1;
                Column++;
                for(Mezzanine::Whole SpaceD=Iter->first.size()+1; SpaceD<=ColumnWidth; SpaceD++)
                {
                    cout << " ";
                    CurrentWidth++;
                }

                if(CurrentWidth>TargetWidth-LongestName-1)
                {
                    cout << std::endl;
                    Column = 0;
                    CurrentWidth = 0;
                }

            }
            cout << std::endl;

            return ExitInvalidArguments;
        }

        TestResult GetTestAnswerFromStdin(Mezzanine::String Question, Boole FailStatusOnIncorrect)
        {
            Mezzanine::String Input;
            char Answer;

            while(true)
            {
                cout << Question;
                getline(std::cin, Input);
                std::stringstream InputStream(Input);
                if (InputStream >> Answer)
                {
                    Answer=static_cast<char>(tolower(Answer));
                    if (Answer=='t' || Answer=='y' || Answer=='f' || Answer=='n' ||
                        Answer=='c' || Answer=='u' || Answer=='i')
                        { break; }
                }

                cout << std::endl << "Expected (T)rue/(Y)es for Success, (F)alse/(N)o for Failure," << std::endl
                     << " (C)anceled to cancel this test, or (U)nsure/(I)nconclusive if you don't know." << std::endl
                     << std::endl;
            }

            switch(Answer)
            {
                case 't': case 'y':
                    return Testing::TestResult::Success;
                case 'f': case 'n':
                    if(FailStatusOnIncorrect)
                        { return Testing::TestResult::Failed; }
                    else
                        { return Testing::TestResult::Warning; }
                case 'c':
                    return Testing::TestResult::Cancelled;
                case 'u': case 'i':
                    return Testing::TestResult::Inconclusive;
                default:
                    return Testing::TestResult::Unknown;
            }
        }

        String GetCommandResults(String Command)
        {
            Command += " > CommandResults.txt";
            system(Command.c_str());
            std::ifstream File("CommandResults.txt");
            return String( std::istreambuf_iterator<char>(File), std::istreambuf_iterator<char>());
        }

        void sleep_for(UInt32 MicroSeconds)
        {
            #ifdef MEZZ_Windows
                Sleep(MicroSeconds/1000);
            #else
                usleep(MicroSeconds);
            #endif
        }

    }// Testing
}// Mezzanine

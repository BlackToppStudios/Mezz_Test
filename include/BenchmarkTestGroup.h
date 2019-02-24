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
#ifndef Mezz_Test_BenchmarkTestGroup_h
#define Mezz_Test_BenchmarkTestGroup_h

/// @file
/// @brief The declaration of a group of tests that is performance sensitive.

#include "UnitTestGroup.h"

namespace Mezzanine
{
    namespace Testing
    {
        SAVE_WARNING_STATE
        //SUPPRESS_CLANG_WARNING("-Wpadded") // Temporary
        //SUPPRESS_CLANG_WARNING("-Wweak-vtables") // Temporary
        SUPPRESS_VC_WARNING(4625) // BS about implicit copy constructors, despite explicit deletion in parent class.

        ///////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Benchmarks are performance sensitive, and require special attention.
        /// @details Because the smallest load can affect performance, nothing else should be running while a
        /// benchmark runs. Since many benchmarks are sensitive to what was just run (pre-filled caches, extra
        /// allocated memory, etc) each test inheriting from this test group will get several safeguards. Inheritors'
        /// tests will be run in their own process to guarantee isolation. No other threads or processes will be run
        /// while this test group runs, but the main process will still be alive in a waiting state until this test
        /// group finishes.
        class MEZZ_LIB BenchmarkTestGroup : public Mezzanine::Testing::UnitTestGroup
        {
        public:
            /// @brief Default virtual deconstructor to allow for polymorphism.
            virtual ~BenchmarkTestGroup() override = default;

            Boole IsMultiThreadSafe() const override;
            Boole IsMultiProcessSafe() const override;
        };
        RESTORE_WARNING_STATE
    }// Testing
}// Mezzanine

#endif

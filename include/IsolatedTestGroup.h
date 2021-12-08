// © Copyright 2010 - 2021 BlackTopp Studios Inc.
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
#ifndef Mezz_Test_IsolatedTestGroup_h
#define Mezz_Test_IsolatedTestGroup_h

/// @file
/// @brief The declaration of the a group of tests that is performance sensitive and process isoloated.

#include "UnitTestGroup.h"

namespace Mezzanine
{
    namespace Testing
    {
        SAVE_WARNING_STATE
        SUPPRESS_VC_WARNING(4625) // BS about implicit copy constructors, despite explicit deletion in parent class.

        ///////////////////////////////////////////////////////////////////////////////////////////
        /// @brief Useful for any time sensitive test that requires special attention.
        /// @details Because the smallest load can affect performance, nothing else should running while a
        /// isolated test runs. Many benchmarks are care about the amount of time a thing takes or need to know that the
        /// system scheduler isn't being strained (simulating race conditions).  Each test inheriting from this test
        /// group will get several safeguards. Inheritoring tests will be run in their own process to guarantee
        /// isolation. No other threads or processes will be run by the Mezz_Test suite while this test group runs. This
        /// cannot handle every situation, other processes on the system will continue running normally and the main
        /// Mezz_Test process will still be alive, but in a waiting state until this test group finishes.
        class MEZZ_LIB IsolatedTestGroup : public Mezzanine::Testing::UnitTestGroup
        {
        public:
            /// @brief Default virtual deconstructor to allow for polymorphism.
            virtual ~IsolatedTestGroup() override = default;

            Boole IsMultiThreadSafe() const override;
            Boole IsMultiProcessSafe() const override;

        };
        RESTORE_WARNING_STATE
    }// Testing
}// Mezzanine

#endif

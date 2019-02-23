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
#ifndef Mezz_Test_InteractiveTestGroup_h
#define Mezz_Test_InteractiveTestGroup_h

/// @file
/// @brief The declaration of a group of tests that need human discretion and judgment.

#include "UnitTestGroup.h"

namespace Mezzanine
{
    namespace Testing
    {
        SAVE_WARNING_STATE
        SUPPRESS_VC_WARNING(4625) // BS about implicit copy constructors, despite explicit deletion in parent class.

        ///////////////////////////////////////////////////////////////////////////////////////////
        /// @brief A single group of tests that all run entirely interactively.
        /// @details These tests are not run automatically, because they require human judgment. Test groups inheriting
        /// from this might seek to answer questions like: Does this texture look like wood grain? Does this image look
        /// like a robot? Is the text emitted legible? In principle these are all questions that could be answered some
        /// other way, but subtle behaviors might change the way processes generating these results work. A procedural
        /// general algorithm might have hard to predict output, a GPU might take shortcuts and not produce pixel
        /// perfect rendering, and system encoding might corrupt text in unexpected ways. Simply showing it to a human
        /// and asking the question is much easier and more cost effective than automating every possible failure mode.
        /// @n All This test group does is remove itself and its inheritors from the list of tests to run automatically.
        class MEZZ_LIB InteractiveTestGroup : public UnitTestGroup
        {
        public:
            /// @brief Default virtual deconstructor to allow for polymorphism.
            virtual ~InteractiveTestGroup() override = default;

            virtual Boole ShouldRunAutomatically() const override;
        };
        RESTORE_WARNING_STATE
    }// Testing
}// Mezzanine

#endif

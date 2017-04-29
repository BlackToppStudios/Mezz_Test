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
#ifndef Mezz_Test_TimingTools_h
#define Mezz_Test_TimingTools_h

/// @file
/// @brief TestData, TestDataStorage and UnitTestGroup class definitions.

#include "DataTypes.h"
#include "SuppressWarnings.h"

#include <chrono>


namespace Mezzanine
{
    namespace Testing
    {
        SAVE_WARNING_STATE
        SUPPRESS_CLANG_WARNING("-Wpadded") // Emscripten complains here.
            /// @brief A simple piece of data to represent the length of a named period of time.
            struct MEZZ_LIB NamedDuration
            {
                /// @brief What was it called?
                Mezzanine::String Name;

                /// @brief How long did it take.
                std::chrono::nanoseconds Duration;
            };
        RESTORE_WARNING_STATE

        /// @brief An easy way to get the time something took to execute.
        class MEZZ_LIB TestTimer
        {
        private:
            /// @brief The time this was constructed.
            std::chrono::high_resolution_clock::time_point BeginTimer;

        public:
            /// @brief Simply Creating this starts the timer.
            TestTimer()
                : BeginTimer(std::chrono::high_resolution_clock::now())
                {}

            /// @brief How long since this started.
            /// @return An std::chrono::duration in nanoseconds containing the difference between now and when
            /// timing was started.
            std::chrono::nanoseconds GetLength();

            /// @brief How long since this started and give it a name for added meaning.
            /// @param Name The name of the time period that just elapsed.
            NamedDuration GetNameDuration(const Mezzanine::String& Name);
        };// TestTimer

        /// @brief Get a human readable string, annotated with minutes, seconds, etc...
        /// @param Duration Some amount of nano seconds that is probably preposterous for humans to grok.
        Mezzanine::String MEZZ_LIB PrettyDurationString(std::chrono::nanoseconds Duration);

        /// @brief Pretty print a NamedDuration.
        /// @param Stream the stream, likely cout to send it.
        /// @param TimingToStream A single NameDuration.
        /// @return The modified stream.
        std::ostream& MEZZ_LIB operator<<(std::ostream& Stream, const NamedDuration& TimingToStream);
    }// Testing
}// Mezzanine

#endif

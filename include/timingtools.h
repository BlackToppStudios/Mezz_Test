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
#ifndef Mezz_Test_timingtools_h
#define Mezz_Test_timingtools_h

/// @file
/// @brief TestData, TestDataStorage and UnitTestGroup class definitions.

#include "datatypes.h"

#include <limits>

namespace Mezzanine
{
    namespace Testing
    {
        /// @brief Get a timestamp, in microseconds.
        /// @details This will generally be some multiple of the GetTimeStampResolution return value.
        /// @warning On some platforms this requires a static initialization, an can cause undefined behavior if called
        /// before static initializations are complete, so don't use this in static contructors at all or it will break
        /// when you move code to a new platform.
        /// @return The largest size integer containing a timestamp that can be compared to other timestamps, but has
        /// no guarantees for external value.
        MaxInt Now();

        /// @brief Get the resolution of the timestamp in microseconds. This is the smallest amount of time that the
        /// function @ref GetTimeStamp can accurately track.
        /// @return A Whole which returns in millionths of a second the smallest unit of time that GetTimeStamp can
        /// measure.
        Whole NowResolution();

        /// @brief An easy way to get the time something took to execute.
        class TimedTest
        {
            /// @brief The time this was constructed.
            MaxInt BeginTimer;
            public:
                /// @brief Simply Creating this starts the timer
                TimedTest()
                    : BeginTimer(Mezzanine::Testing::Now())
                    {}

                /// @brief How long since this started.
                /// @return A MaxInt containing the Now - BeginTimer.
                MaxInt GetLength()
                    { return Mezzanine::Testing::Now() - BeginTimer; }
        };
    }// Testing
}// Mezzanine

#endif

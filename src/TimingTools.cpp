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
/// @brief The implementation of stuff that must be run in the context of TestData.

#include "TimingTools.h"
#include "MezzTest.h"

#include <iomanip>

using std::chrono::minutes;
using std::chrono::seconds;
using std::chrono::milliseconds;
using std::chrono::microseconds;
using std::chrono::nanoseconds;

using std::chrono::duration_cast;

namespace
{
    /// @internal
    /// @brief Turn part of a useless count of nanoseconds into a meaningful string of minutes, seconds and subseconds.
    /// @tparam UnitType A std::chrono::duration to convert the nanoseconds.
    /// @param Duration The nanoseconds to break into larger units.
    /// @param Suffix A string to decribe the unit.
    /// @param Stream The place to send the newly created useful string.
    /// @return The new Duration with all of the whole instances of the specified unit removed.
    template <class UnitType>
    nanoseconds TruncateUnit(nanoseconds Duration, const Mezzanine::String& Suffix, std::ostream& Stream)
    {
        UnitType TruncateAmount = duration_cast<UnitType>(Duration);
        Stream << std::setw(3) << TruncateAmount.count() << Suffix << ' ';
        Duration -= TruncateAmount;
        return Duration;
    }
}

namespace Mezzanine
{
    namespace Testing
    {

        std::chrono::nanoseconds TestTimer::GetLength()
            { return duration_cast<nanoseconds> (std::chrono::high_resolution_clock::now() - BeginTimer); }

        NamedDuration TestTimer::GetNameDuration(const String& Name)
            { return NamedDuration{Name, GetLength()}; }

        std::ostream& operator<<(std::ostream& Stream, const NamedDuration& TimingToStream)
        {
            return Stream << std::right << std::setw(TimingNameColumnWidth) << TimingToStream.Name << ": "
                          << std::left << std::setw(TimingNsColumnWidth) << TimingToStream.Duration.count()
                          << PrettyDurationString(TimingToStream.Duration);
        }

        Mezzanine::String PrettyDurationString(nanoseconds Duration)
        {
            std::stringstream PrettyTimeAssembler;

            Duration = TruncateUnit<minutes>(Duration, "min", PrettyTimeAssembler);
            Duration = TruncateUnit<seconds>(Duration, "s", PrettyTimeAssembler);
            Duration = TruncateUnit<milliseconds>(Duration, "ms", PrettyTimeAssembler);
            Duration = TruncateUnit<microseconds>(Duration, "μs", PrettyTimeAssembler);
            Duration = TruncateUnit<nanoseconds>(Duration, "ns", PrettyTimeAssembler);

            return PrettyTimeAssembler.str();
        }

    }// Testing
}// Mezzanine

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
#ifndef Mezz_Test_OutputBufferGuard_h
#define Mezz_Test_OutputBufferGuard_h

/// @file
/// @brief Definition of a class for intercepting and returning the rdbuf to and from OutputStreams.

#include "DataTypes.h"

namespace Mezzanine
{
    namespace Testing
    {
        /// @internal
        /// @brief Used to apply RAII to STDOUT and STDERR buffers/streams.
        class MEZZ_LIB OutputBufferGuard
        {
        private:
            /// @brief A place to store output that would go to the original stream.
            std::stringstream StreamOuputSink;

            /// @brief Stores a reference to the original stream so we can restore it in the end.
            std::ostream& StreamToGaurd;

            /// @brief Storage to hold a non-owning pointer to the original read buffer.
            std::streambuf* OriginalStreamBuf;
        public:

            /// @brief Copying this kind of guard is insanity so the copy constructor is deleted.
            OutputBufferGuard(const OutputBufferGuard&) = delete;
            /// @brief Assignment of this kind of guard is insanity so the copy constructor is deleted.
            OutputBufferGuard& operator=(const OutputBufferGuard&) = delete;

            /// @brief Moving one of these guards is probably just fine, but it has been deleted until needed.
            OutputBufferGuard(OutputBufferGuard&&) = delete;
            /// @brief Move assigning one of these guards might be just fine, but it has been deleted until needed.
            OutputBufferGuard& operator=(OutputBufferGuard&&) = delete;

            /// @brief Captures Output buffers and configures test outputs on creation.
            /// @param StreamToSilence The stream to silence and guard from outputting anything until the destructor
            /// is called.
            explicit OutputBufferGuard(std::ostream& StreamToSilence);

            /// @brief Captures anything sent to the guarded stream.
            /// @return A String with the whole of the contents sunk into the original Stream.
            Mezzanine::String GetSunkOutput() const;

            /// @brief Restores original output buffers on creation.
            virtual ~OutputBufferGuard();
        };// OutputBufferGuard
    }// Testing
}// Mezzanine

#endif

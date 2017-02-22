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
#ifndef Mezz_Test_ConsoleStringManipulationTests_h
#define Mezz_Test_ConsoleStringManipulationTests_h

/// @file
/// @brief A few simple tests for string process tools that might be useful for command line applications.

#include "MezzTest.h"

using Mezzanine::String;
using Mezzanine::Testing::rtrim;
using Mezzanine::Testing::RightTrim;
using Mezzanine::Testing::AllLower;
using Mezzanine::Testing::SanitizeFileName;
using Mezzanine::Testing::SanitizeProcessCommand;

AUTOMATIC_TEST_GROUP(StringManipulationTests, StringManipulation)
{
    TEST_EQUAL("rtrim-none",        String("lorem"),    rtrim("lorem"));
    TEST_EQUAL("rtrim-simple",      String("ipsum"),    rtrim("ipsum  "));
    TEST_EQUAL("rtrim-tabs",        String("sit"),      rtrim("sit \t "));
    TEST_EQUAL("rtrim-newlines",    String("dolor"),    rtrim("dolor \n\r "));

    TEST_EQUAL("RightTrim-none",        String("lorem"),    RightTrim("lorem"));
    TEST_EQUAL("RightTrim-simple",      String("ipsum"),    RightTrim("ipsum  "));
    TEST_EQUAL("RightTrim-tabs",        String("sit"),      RightTrim("sit \t "));
    TEST_EQUAL("RightTrim-newlines",    String("dolor"),    RightTrim("dolor \n\r "));

    TEST_EQUAL("AllLower-none",     String("lorem"),    AllLower("lorem"));
    TEST_EQUAL("AllLower-simple",   String("ipsum"),    AllLower("Ipsum"));
    TEST_EQUAL("AllLower-all",      String("sit"),      AllLower("SIT"));
    TEST_EQUAL("AllLower-back",     String("dolor"),    AllLower("dolOR"));
    TEST_EQUAL("AllLower-extra",    String("amet  "),   AllLower("ameT  "));

    TEST_EQUAL("SanitizeFileName-none",     String("logfile.txt"),          SanitizeFileName("logfile.txt"));
    TEST_EQUAL("SanitizeFileName-folders",  String("logs/A\\logfile.txt"),  SanitizeFileName("logs/A\\logfile.txt"));
    TEST_EQUAL("SanitizeFileName-pipe",     String("echo foo _ wc _ bar"),  SanitizeFileName("echo foo | wc > bar"));
    TEST_EQUAL("SanitizeFileName-allbad",   String("________________"),     SanitizeFileName("|><?:%\"=+,!@[]*;"));

    TEST_EQUAL("SanitizeProcessCommand-none",     String("echo 'foo'"),     SanitizeProcessCommand("echo 'foo'"));
    TEST_EQUAL("SanitizeProcessCommand-pipe",     String("ls _ wc"),        SanitizeProcessCommand("ls | wc"));
    TEST_EQUAL("SanitizeProcessCommand-allbad",   String("___"),            SanitizeProcessCommand("|><"));
}

#endif

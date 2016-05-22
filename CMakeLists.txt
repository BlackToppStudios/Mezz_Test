# © Copyright 2010 - 2016 BlackTopp Studios Inc.
# This file is part of The Mezzanine Engine.
#
#    The Mezzanine Engine is free software: you can redistribute it and/or modify
#    it under the terms of the GNU General Public License as published by
#    the Free Software Foundation, either version 3 of the License, or
#    (at your option) any later version.
#
#    The Mezzanine Engine is distributed in the hope that it will be useful,
#    but WITHOUT ANY WARRANTY; without even the implied warranty of
#    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#    GNU General Public License for more details.
#
#    You should have received a copy of the GNU General Public License
#    along with The Mezzanine Engine.  If not, see <http://www.gnu.org/licenses/>.
#
#   The original authors have included a copy of the license specified above in the
#   'Docs' folder. See 'gpl.txt'
#
#   We welcome the use of the Mezzanine engine to anyone, including companies who wish to
#   Build professional software and charge for their product.
#
#   However there are some practical restrictions, so if your project involves
#   any of the following you should contact us and we will try to work something
#   out:
#    - DRM or Copy Protection of any kind(except Copyrights)
#    - Software Patents You Do Not Wish to Freely License
#    - Any Kind of Linking to Non-GPL licensed Works
#    - Are Currently In Violation of Another Copyright Holder's GPL License
#    - If You want to change our code and not add a few hundred MB of stuff to
#        your distribution
#
#   These and other limitations could cause serious legal problems if you ignore
#   them, so it is best to simply contact us or the Free Software Foundation, if
#   you have any questions.
#
#   Joseph Toppi - toppij@gmail.com
#   John Blackwood - makoenergy02@gmail.com

####################################################################################################
cmake_minimum_required(VERSION 3.0)
project(Test)
message(STATUS "${PROJECT_NAME} - Starting Configuration.")

##########################################################################|#########################
#set(JagatiChecksum "da401b29af84a4b3c119a23b955e45534c4128d78baa14bf63a58d\
#8b4c11c10c1b85464dbaea68d180d2fd5f3ea43a4a2a960c7dfc3dfe0890696ee92480a280")

#file(DOWNLOAD
#    "https://raw.githubusercontent.com/BlackToppStudios/Jagati/master/Jagati.cmake"
#    "${${PROJECT_NAME}_BINARY_DIR}/Jagati.cmake"
#    EXPECTED_HASH SHA512=${JagatiChecksum}
#)
include("${${PROJECT_NAME}_BINARY_DIR}/Jagati.cmake")

StandardJagatiSetup()

IncludeJagatiPackage("StaticFoundation")

####################################################################################################
message(STATUS "Determining Source Files.")

set(HeaderFiles
    "${TestIncludeDir}consolelogic.h"
    "${TestIncludeDir}consolestringmanipulation.h"
    "${TestIncludeDir}dagframescheduler.h"
    "${TestIncludeDir}mezztest.h"
    "${TestIncludeDir}pugiconfig.h"
    "${TestIncludeDir}pugixml.h"
    "${TestIncludeDir}testdata.h"
    "${TestIncludeDir}testdatatools.h"
    "${TestIncludeDir}testenumerations.h"
)
ShowList("Header Files:" "\t" "${HeaderFiles}")
AddJagatiDoxInput("${HeaderFiles}")

set(SourceFiles
    "${TestSourceDir}consolelogic.cpp"
    "${TestSourceDir}consolestringmanipulation.cpp"
    "${TestSourceDir}mezztest.cpp"
    "${TestSourceDir}pugixml.cpp"
    "${TestSourceDir}testdata.cpp"
    "${TestSourceDir}testdatatools.cpp"
    "${TestSourceDir}testenumerations.cpp"
)
ShowList("Source Files:" "\t" "${SourceFiles}")

set(TestSourceFiles
    "${TestTestDir}testtests.h"
    "${TestTestDir}testtests.cpp"
)
ShowList("Test Source Files:" "\t" "${TestSourceFiles}")

set(DoxFiles
    "${TestDoxDir}dox.h"
)
ShowList("Doxygen Input Files:" "\t" "${DoxFiles}")
AddJagatiDoxInput("${DoxFiles}")

####################################################################################################
# Set up Include Directories

message("StaticFoundationIncludeDir: ${StaticFoundationIncludeDir}")
message("TestIncludeDir: ${TestIncludeDir}")
include_directories(
    ${StaticFoundationIncludeDir}
    ${StaticFoundationGenHeadersDir}
    ${${PROJECT_NAME}IncludeDir}
)

####################################################################################################
# Set up Build targets

AddJagatiLibrary("Test")
add_library(${TestLib} ${MEZZ_LibraryBuildType} ${HeaderFiles} ${SourceFiles})
target_link_libraries(${TestLib} ${StaticFoundationLib})

add_executable(Test_Tester ${HeaderFiles} ${TestSourceFiles})
target_link_libraries(Test_Tester ${StaticFoundationLib} ${TestLib})

message(STATUS "${PROJECT_NAME} - Ending Configuration.")

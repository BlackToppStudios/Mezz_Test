# © Copyright 2010 - 2017 BlackTopp Studios Inc.
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

########################################################################################################################
cmake_minimum_required(VERSION 3.0)
project(Test)
message(STATUS "${PROJECT_NAME} - Starting Configuration.")

##########################################################################|#############################################
# Download, Verify and setup the Jagati
set(JagatiChecksum "35100defd5b0355c10f0ffff5ba4f979a2757d7f8e395afdf8dd57\
c84c3556f4c74569e5c1f65bb727e3676ff3ad3695e1edb59bc73de96274e8fdcd5ed1a019")
file(DOWNLOAD
    "https://raw.githubusercontent.com/BlackToppStudios/Jagati/0.15.1/Jagati.cmake"
    "${${PROJECT_NAME}_BINARY_DIR}/Jagati.cmake"
    EXPECTED_HASH SHA512=${JagatiChecksum}
)
include("${${PROJECT_NAME}_BINARY_DIR}/Jagati.cmake")

StandardJagatiSetup()
IncludeJagatiPackage("StaticFoundation")

########################################################################################################################
# Add Sources
message(STATUS "Determining Source Files.")
set(TestHeaderFiles
    "${TestIncludeDir}AutomaticTestGroup.h"
    "${TestIncludeDir}BenchmarkTestGroup.h"
    "${TestIncludeDir}ConsoleLogic.h"
    "${TestIncludeDir}InteractiveTestGroup.h"
    "${TestIncludeDir}MezzTest.h"
    "${TestIncludeDir}OutputBufferGuard.h"
    "${TestIncludeDir}ProcessTools.h"
    "${TestIncludeDir}SilentTestGroup.h"
    "${TestIncludeDir}StringManipulation.h"
    "${TestIncludeDir}TestData.h"
    "${TestIncludeDir}TestEnumerations.h"
    "${TestIncludeDir}TestMacros.h"
    "${TestIncludeDir}TimingTools.h"
    "${TestIncludeDir}UnitTestGroup.h"
)
ShowList("Source Files:" "\t" "${TestHeaderFiles}")

set(TestSourceFiles
    "${TestSourceDir}AutomaticTestGroup.cpp"
    "${TestSourceDir}BenchmarkTestGroup.cpp"
    "${TestSourceDir}ConsoleLogic.cpp"
    "${TestSourceDir}InteractiveTestGroup.cpp"
    "${TestSourceDir}MezzTest.cpp"
    "${TestSourceDir}OutputBufferGuard.cpp"
    "${TestSourceDir}ProcessTools.cpp"
    "${TestSourceDir}SilentTestGroup.cpp"
    "${TestSourceDir}StringManipulation.cpp"
    "${TestSourceDir}TestData.cpp"
    "${TestSourceDir}TestEnumerations.cpp"
    "${TestSourceDir}TestMacros.cpp"
    "${TestSourceDir}TimingTools.cpp"
    "${TestSourceDir}UnitTestGroup.cpp"
)
ShowList("Source Files:" "\t" "${TestSourceFiles}")

set(TestSwigFiles
    ""
)
ShowList("Swig Input Files:" "\t" "${TestSwigFiles}")

set(TestDoxFiles
    "${TestDoxDir}Dox.h"
)
ShowList("Doxygen Input Files:" "\t" "${TestDoxFiles}")
AddJagatiDoxInput("${TestDoxFiles}")

########################################################################################################################
# Build targets

# Make a library with our sources.
AddJagatiLibrary()
CreateCoverageTarget(${TestLib} "${TestSourceFiles}")

# This has only three tests and they could  be added as follows.
#AddTestClass("Boilerplate") # Not a real test but we want our example to build
#AddTestClass("Conversion")
#AddTestClass("Test")

# Instead we can also add all the tests in the test directory with the following:
AddTestDirectory(${${PROJECT_NAME}TestDir})

# Spit out the C++ file
EmitTestCode()
AddTestTarget()

# Some extra creating of targets for other development related tasks
AddIDEVisibility("")
SetCodeCoverage()

message(STATUS "${PROJECT_NAME} - Ending Configuration.")

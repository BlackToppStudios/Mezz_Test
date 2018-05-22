# © Copyright 2010 - 2018 BlackTopp Studios Inc.
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

############################################################################|###########################################
# Download, Verify and Setup the Jagati
if(NOT JAGATI_File)
    set(JAGATI_File "${${PROJECT_NAME}_BINARY_DIR}/Jagati.cmake" CACHE FILEPATH
        "The file to load the Jagati from and potentially to download it to.")
endif(NOT JAGATI_File)
if(NOT JAGATI_Download)
    option(JAGATI_Download "Should the Jagati be downloaded automatically" ON)
endif(NOT JAGATI_Download)
if(JAGATI_Download)
    set(JAGATI_Checksum "5417802bf9150088c086df70453e091b060b6d03edf005f90dd\
a39b504906f76bffa0dbcc02248597e536beceed7dc90c8c582aae9f4a98447efdf8d609b23c0"
        CACHE STRING "Check that when the Jagati is downloaded the right one is used (for consistency and security).")
    set(JAGATI_Url "https://raw.githubusercontent.com/BlackToppStudios/Jagati/0.25.1/Jagati.cmake"
        CACHE STRING "Where to download the Jagati from.")
    file(DOWNLOAD "${JAGATI_Url}" "${JAGATI_File}" EXPECTED_HASH SHA512=${JAGATI_Checksum})
endif(JAGATI_Download)
include("${JAGATI_File}")

########################################################################################################################
# The rest of the Jagati setup

StandardJagatiSetup()
IncludeJagatiPackage("StaticFoundation")

########################################################################################################################
# Add Sources
message(STATUS "Determining Source Files.")

AddHeaderFile("AutomaticTestGroup.h")
AddHeaderFile("BenchmarkTestGroup.h")
AddHeaderFile("BenchmarkThreadTestGroup.h")
AddHeaderFile("ConsoleLogic.h")
AddHeaderFile("InteractiveTestGroup.h")
AddHeaderFile("MezzTest.h")
AddHeaderFile("OutputBufferGuard.h")
AddHeaderFile("ProcessTools.h")
AddHeaderFile("SilentTestGroup.h")
AddHeaderFile("StringManipulation.h")
AddHeaderFile("TestData.h")
AddHeaderFile("TestEnumerations.h")
AddHeaderFile("TestMacros.h")
AddHeaderFile("TimingTools.h")
AddHeaderFile("UnitTestGroup.h")
ShowList("Source Files:" "\t" "${TestHeaderFiles}")

AddSourceFile("BenchmarkTestGroup.cpp")
AddSourceFile("BenchmarkThreadTestGroup.cpp")
AddSourceFile("ConsoleLogic.cpp")
AddSourceFile("InteractiveTestGroup.cpp")
AddSourceFile("MezzTest.cpp")
AddSourceFile("OutputBufferGuard.cpp")
AddSourceFile("ProcessTools.cpp")
AddSourceFile("SilentTestGroup.cpp")
AddSourceFile("StringManipulation.cpp")
AddSourceFile("TestData.cpp")
AddSourceFile("TestEnumerations.cpp")
AddSourceFile("TimingTools.cpp")
AddSourceFile("UnitTestGroup.cpp")
ShowList("Source Files:" "\t" "${TestSourceFiles}")

#AddSwigEntryPoint("NotYet.h")
ShowList("Swig Input Files:" "\t" "${TestSwigFiles}")

AddJagatiDoxInput("Dox.h")
ShowList("Doxygen Input Files:" "\t" "${TestDoxFiles}")

########################################################################################################################
# Build targets

# Make a library with our sources.
AddJagatiLibrary()
CreateCoverageTarget(${TestLib} "${TestSourceFiles}")

# This has only three tests and they could  be added as follows.
# AddTestFile("Boilerplate.h") # Not a real test but we want our example to build
# AddTestFile("Conversion.h")
# AddTestFile("Test.h")

# Instead we can also add all the tests in the test directory with the following:
AddTestDirectory(${${PROJECT_NAME}TestDir})

# Spit out the C++ file
EmitTestCode()
AddTestTarget()

# Some extra creating of targets for other development related tasks
AddIDEVisibility("Jenkinsfile")
SetCodeCoverage()

message(STATUS "${PROJECT_NAME} - Ending Configuration.")

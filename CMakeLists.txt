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

########################################################################################################################
cmake_minimum_required(VERSION 3.0)
project(Test)
message(STATUS "${PROJECT_NAME} - Starting Configuration.")

##########################################################################|#############################################
set(JagatiChecksum "14b39df7e3cebef5e1e78a1b974e52fac737f062e432666ead8155\
414ce0591f569b108c047c4f90799c13a8574b28c4b05dadac22c4375b8f1ecc05b798bf39")

file(DOWNLOAD
    "https://raw.githubusercontent.com/BlackToppStudios/Jagati/0.11.2/Jagati.cmake"
    "${${PROJECT_NAME}_BINARY_DIR}/Jagati.cmake"
    EXPECTED_HASH SHA512=${JagatiChecksum}
)

include("${${PROJECT_NAME}_BINARY_DIR}/Jagati.cmake")

StandardJagatiSetup()

IncludeJagatiPackage("StaticFoundation")

########################################################################################################################
message(STATUS "Determining Source Files.")
set(TesterHeaderFiles
    "${TestIncludeDir}ConsoleLogic.h"
    "${TestIncludeDir}ConsoleStringManipulation.h"
    "${TestIncludeDir}MezzTest.h"
    "${TestIncludeDir}pugixml.h"
    "${TestIncludeDir}TestData.h"
    "${TestIncludeDir}TestDataTools.h"
    "${TestIncludeDir}TestEnumerations.h"
    "${TestIncludeDir}TimingTools.h"
    "${TestIncludeDir}UnitTestGroup.h"

    # External library included here for simplicity
    "${TestIncludeDir}pugiconfig.h"
)

set(TesterSourceFiles
    "${TestSourceDir}ConsoleLogic.cpp"
    "${TestSourceDir}ConsoleStringManipulation.cpp"
    "${TestSourceDir}MezzTest.cpp"
    "${TestSourceDir}TestData.cpp"
    "${TestSourceDir}TestDataTools.cpp"
    "${TestSourceDir}TestEnumerations.cpp"
    "${TestSourceDir}TimingTools.cpp"
    "${TestSourceDir}UnitTestGroup.cpp"

    # External Library, not part of the API don't use this externally.
    "${TestSourceDir}pugixml.cpp"
)
ShowList("Source Files:" "\t" "${TesterSourceFiles}")

set(DoxFiles
    "${TestDoxDir}Dox.h"
)
ShowList("Doxygen Input Files:" "\t" "${DoxFiles}")
AddJagatiDoxInput("${DoxFiles}")

########################################################################################################################
# This setup is unusual but could be useful.

include_directories(
    ${${PROJECT_NAME}IncludeDir}
    ${TestIncludeDir}
    ${StaticFoundationIncludeDir}
    ${StaticFoundationGenHeadersDir}
)

# A normal project would put declaration of targets here, a really good project would house the bulk of its
# functionality in a library so that it could easily be linked to the testing library and the final executable.
#add_library(${YourLibraryHere} ${MEZZ_LibraryBuildType} ${YourHeaders} ${YourSources})

AddJagatiLibrary("Test")
CreateCoverageTarget(${TestLib} "${TesterSourceFiles}")

# This has only one test and could be added as follows.
# AddTestClass("Test")
# AddTestClass("Conversions")

# Instead to prove it works we added all the tests in the test directory with the following:
AddTestDirectory(${${PROJECT_NAME}TestDir})

# Spit out the C++ file
EmitTestCode()
AddTestTarget("")

# You need to link the test library to your stuff to be tested.
#target_link_libraries(${TestLib} ${YourLibraryHere})



########################################################################################################################
# Set up Build targets



# A Project to do nothing but get All the Source files visible in IDEs
add_custom_target(
    ${PROJECT_NAME}_IDE_Visibility
    DEPENDS ${PROJECT_NAME}_Tester
            "${${PROJECT_NAME}BinaryDir}Guidelines.md"
            ExternalProjectDownload
    SOURCES ${HeaderFiles}
            ${SourceFiles}
            ${TestSourceFiles}
            ${SwigFiles}
            ${StaticFoundationConfigFilename}
            README.md
            COPYING.md
            .travis.yml
            appveyor.yml
            codecov.yml
            "${DoxFiles}"
)

SetCodeCoverage()

message(STATUS "${PROJECT_NAME} - Ending Configuration.")

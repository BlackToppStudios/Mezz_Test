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
#ifndef staticfoundation_dox_h
#define staticfoundation_dox_h

/// @file
/// @brief The main documentation header to include for this library

/// @page Mezz_StaticFoundation Jagati Library Static Foundation
/// This is library provides basic OS feature detection facilities for C++ code and SWIG bound
/// languages. This library can be used by adding it to your project as you would any other
/// @ref Jagati library and include @ref staticfoundation.h. The class @ref Mezzanine::RuntimeStatic
/// describes all options detected that can be used by SWIG bound libraries or queried at runtime by
/// C++ code. Here is a list of all Preprocessor definitions and what they do:
///
///     - @ref MEZZ_BuildDoxygen - @ref StaticFoundationConfig.h - Set if the doxygen build option
/// was set.
///     - @ref MEZZ_BuildStaticLibraries - @ref StaticFoundationConfig.h - Set if the build option
/// to build static libraries was set. This defaults to true. Dynamic libraries will be built
/// otherwise.
///     - @ref MEZZ_Linux  - @ref StaticFoundationConfig.h - Set if CMake dectects a Linux build
/// environment.
///     - @ref MEZZ_Windows  - @ref StaticFoundationConfig.h - Set if CMake dectects a Windows build
/// environment.
///     - @ref MEZZ_MacOSX  - @ref StaticFoundationConfig.h - Set if CMake dectects a Mac OS X build
/// environment.
///     - @ref MEZZ_CompilerIsGCC  - @ref StaticFoundationConfig.h - Set if the compiler is GCC,
/// Mingw or TDM-GCC.
///     - @ref MEZZ_CompilerIsClang  - @ref StaticFoundationConfig.h - Set if the compiler is Clang.
///     - @ref MEZZ_CompilerIsIntel  - @ref StaticFoundationConfig.h - Set if the compiler Intel.
///     - @ref MEZZ_CompilerIsMsvc  - @ref StaticFoundationConfig.h - Set only if the compiler is
/// MSVC.
///     - @ref MEZZ_CompilerDesignNix  - @ref StaticFoundationConfig.h - Set for pretty much all
/// compilers except MSVC.
///     - @ref MEZZ_CompilerDesignMS  - @ref StaticFoundationConfig.h - Set only when using MSVC

/// @file StaticFoundationConfig.h
/// @details This file is generated automatically by the CMake build process in the Static
/// Foundation Jagati Package.

/// @def MEZZ_BuildDoxygen
/// @brief A Preprocessor macro set in @ref StaticFoundationConfig.h. This is a CMake Option set in
/// the StaticFoundation. If set an extra build step is added that adds a build target which runs
/// doxygen against every Jagati package in the build.

/// @def MEZZ_BuildStaticLibraries
/// @brief A Preprocessor macro set in @ref StaticFoundationConfig.h. This is set by cmake option
/// and on supported platforms this will allow choosing between static libraries (.a/.lib) and
/// dynamix libraries (.so/.dll).

/// @def MEZZ_Linux
/// @brief A Preprocessor macro set in @ref StaticFoundationConfig.h. Set if Linux is the target
/// platform, unset otherwise.

/// @def MEZZ_Windows
/// @brief A Preprocessor macro set in @ref StaticFoundationConfig.h. Set if windows is the target
/// platform, unset otherwise.

/// @def MEZZ_MacOSX
/// @brief A Preprocessor macro set in @ref StaticFoundationConfig.h. Set if Mac OS X is the target
/// platform, unset otherwise.

/// @def MEZZ_CompilerIsGCC
/// @brief A Preprocessor macro set in @ref StaticFoundationConfig.h. Set if the current build is
/// being done with GCC, false otherwise.

/// @def MEZZ_CompilerIsClang
/// @brief A Preprocessor macro set in @ref StaticFoundationConfig.h. Set if the current build is
/// being done with Clang, false otherwise.

/// @def MEZZ_CompilerIsIntel
/// @brief A Preprocessor macro set in @ref StaticFoundationConfig.h. Set if the current build is
/// being done with the Intel compiler, false otherwise.

/// @def MEZZ_CompilerIsMsvc
/// @brief A Preprocessor macro set in @ref StaticFoundationConfig.h. Set if the current build is
/// being done with msvc, false otherwise.

/// @def MEZZ_CompilerDesignNix
/// @brief A Preprocessor macro set in @ref StaticFoundationConfig.h. Set if the compiler accepts
/// arguments and pragmas generally like Unix style compilers.

/// @def MEZZ_CompilerDesignMS
/// @brief A Preprocessor macro set in @ref StaticFoundationConfig.h. Set if the compiler accepts
/// arguments and pragmas generally like ms style compilers.





/// @page Jagati Jagati
/// @todo describe using Jagati

#endif

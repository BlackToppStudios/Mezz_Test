// © Copyright 2010 - 2020 BlackTopp Studios Inc.
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
/// @brief The implementation of items important a commandline tool to work correctly without need to be available to
/// test developers.

#include "ProcessTools.h"
#include "StringManipulation.h"
#include "SuppressWarnings.h"

SAVE_WARNING_STATE
SUPPRESS_VC_WARNING(4548) // This was added to suppress a warning in MSVC's implementation
                          // of malloc.h where they use a comma in an assert.
SUPPRESS_VC_WARNING(4668) // _WIN32_WINNT_WIN10_RS5 isn't defined causing a warning which
                          // is escalated into an error.
#ifdef MEZZ_Windows
    #include "windows.h"
#else // MEZZ_Windows
    #include "unistd.h"
    #include <string.h>
    #include <sys/types.h>
    #include <sys/wait.h>
#endif // MEZZ_Windows

RESTORE_WARNING_STATE

#ifdef max
#undef max
#endif // max

#ifdef min
#undef min
#endif // max

#include <exception>
#include <cstdlib>

#include <iostream>
#include <fstream>
#include <limits>

namespace {
    using namespace Mezzanine;
#ifdef MEZZ_Windows
    /// @brief A struct with basic info that can be returned from attempting to launch a process.
    struct MEZZ_LIB ProcessInfo
    {
        /// @brief A HANDLE belonging to the output end of the child processes cout pipe.
        HANDLE ChildPipe = 0;
        /// @brief A HANDLE belonging to the child process itself.
        HANDLE ChildProcess = 0;
        /// @brief The error code given if the process failed to launch.
        DWORD ErrorNum = 0;
        /// @brief The error string given if the process failed to launch.
        String ErrorStr;
    };//ProcessInfo

    /// @brief Convenience type because Windows likes fatty Strings.
    using WideString = std::wstring;

    /// @brief Demotes a size_t to an int, which is required by the Win API.
    /// @brief ToDemote The large integral type to be demoted.
    /// @return Returns an int of the same value if it can be stored, or asserts otherwise.
    [[nodiscard]]
    int IntDemote(const size_t ToDemote)
    {
        assert( ToDemote <= static_cast<size_t>( std::numeric_limits<int>::max() ) );
        return static_cast<int>(ToDemote);
    }
    /// @brief Converts a narrow (8-bit) string to a wide (16-bit) string.
    /// @param Thin The string to be converted.
    /// @return Returns a wide string with the converted contents.
    [[nodiscard]]
    WideString ConvertToWideString(const StringView Thin)
    {
        WideString Ret;
        size_t ThinSize = Thin.size();
        if( ThinSize > 0 ) {
            size_t WideLength = ::MultiByteToWideChar(CP_UTF8,0,Thin.data(),IntDemote(ThinSize),nullptr,0);
            Ret.resize(WideLength,L'\0');
            ::MultiByteToWideChar(CP_UTF8,0,Thin.data(),IntDemote(ThinSize),&Ret[0],IntDemote(WideLength));
        }
        return Ret;
    }

    /// @brief Converts a wide (16-bit) String to a narrow (8-bit) String.
    /// @param Wide The String to be converted.
    /// @param Length The length of the String to be converted.
    /// @return Returns a narrow String with the converted contents.
    [[nodiscard]]
    String ConvertToNarrowString(const wchar_t* Wide, const size_t Length)
    {
        String Ret;
        if( Length > 0 && Length < size_t( std::numeric_limits<int>::max() ) ) {
            int CastedLength = static_cast<int>(Length);
            int NarrowLength = ::WideCharToMultiByte(CP_UTF8,0,Wide,CastedLength,nullptr,0,nullptr,nullptr);
            Ret.resize(static_cast<size_t>(NarrowLength),'\0');
            ::WideCharToMultiByte(CP_UTF8,0,Wide,CastedLength,&Ret[0],static_cast<int>(Ret.size()),nullptr,nullptr);
        }
        return Ret;
    }

    /// @brief Gets whether or not the back of a String can be trimmed.
    /// @param ToCheck The String to check for trimming.
    /// @return Returns true if the back of the String can be trimmed/removed.
    [[nodiscard]]
    Boole CanTrimBack(const String& ToCheck)
        { return !ToCheck.empty() && ( ToCheck.back() == '\n' || ToCheck.back() == '\r' ); }

    /// @brief Creates and launches a new process.
    /// @param ExecutablePath Specifies the path to the exe that will be launched. Can be empty.
    /// @param Arguments The space separated arguments given to the exe being launched. This MUST include
    /// the path the executable as the first argument.
    /// @return Returns a ProcessInfo struct containing information about the launched process or it's failure.
    [[nodiscard]]
    ProcessInfo CreateCommandProcess(StringView ExecutablePath, const StringView Arguments)
    {
        HANDLE Child_STDOUT_Read = NULL;
        HANDLE Child_STDOUT_Write = NULL;

        // SECURITY_ATTRIBUTES setup
        SECURITY_ATTRIBUTES SecurityAttrib;
        SecurityAttrib.nLength = sizeof(SECURITY_ATTRIBUTES);
        SecurityAttrib.bInheritHandle = TRUE;
        SecurityAttrib.lpSecurityDescriptor = NULL;

        // Create our Pipe
        if( !::CreatePipe(&Child_STDOUT_Read,&Child_STDOUT_Write,&SecurityAttrib,0) ) {
            throw std::runtime_error("Unable to create pipes for child process.");
        }
        // Ensure handle isn't inherited.
        if( !::SetHandleInformation(Child_STDOUT_Read,HANDLE_FLAG_INHERIT,0) ) {
            throw std::runtime_error("Unable to set handle information for child process pipe.");
        }

        // PROCESS_INFORMATION setup
        PROCESS_INFORMATION ProcessInfo;
        ZeroMemory(&ProcessInfo,sizeof(ProcessInfo));

        // STARTUPINFO setup
        STARTUPINFOW ProcessStartUp;
        ZeroMemory(&ProcessStartUp,sizeof(ProcessStartUp));
        ProcessStartUp.cb = sizeof(ProcessStartUp);
        //ProcessStartUp.hStdError = Child_STDOUT_Write;
        ProcessStartUp.hStdOutput = Child_STDOUT_Write;
        ProcessStartUp.dwFlags |= STARTF_USESTDHANDLES;

        WideString WideExecutablePath = ConvertToWideString(ExecutablePath);
        WideString WideArguments = ConvertToWideString(Arguments);

        BOOL ChildLaunch = ::CreateProcessW(
            WideExecutablePath.empty() ? nullptr : WideExecutablePath.data(),
            WideArguments.empty() ? nullptr : WideArguments.data(),
            nullptr,
            nullptr,
            TRUE,
            0,
            nullptr,
            nullptr,
            &ProcessStartUp,
            &ProcessInfo
        );

        ::CloseHandle(Child_STDOUT_Write);
        if( !ChildLaunch ) {
            ::CloseHandle(Child_STDOUT_Read);
            DWORD ErrorNum = GetLastError();
            StringStream ErrorStream;
            ErrorStream << "Process Error: ";
            wchar_t WideBuffer[256];
            DWORD WrittenChars = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                                nullptr,
                                                ErrorNum,
                                                MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                                                WideBuffer,
                                                sizeof(WideBuffer) / sizeof(wchar_t),
                                                nullptr);
            ErrorStream << ConvertToNarrowString(WideBuffer,WrittenChars);
            return { 0, 0, ErrorNum, ErrorStream.str() };
        }
        return { Child_STDOUT_Read, ProcessInfo.hProcess, 0, String() };
    }
#else // Mezz_Windows
    /// @brief A simple struct containing the basic information of a launched child process.
    struct MEZZ_LIB ProcessInfo
    {
        int ChildPipe = 0;
        int ChildPID = 0;
    };//ProcessInfo

    /// @brief Gets whether or not the back of a String can be trimmed.
    /// @param ToCheck The String to check for trimming.
    /// @return Returns true if the back of the String can be trimmed/removed.
    [[nodiscard]]
    Boole CanTrimBack(const String& ToCheck)
        { return ( !ToCheck.empty() && ToCheck.back() == '\n' ); }

    // Move this out of the if/else should windows actually need it.
    /// @brief Extracts just the file system path to the executable from a complete command.
    /// @param ExtractFrom The String containing the complete command.
    /// @return Returns a substring of the parameter (presumably) containing the path to the executable.
    [[nodiscard]]
    String ExtractExecPath(const StringView ExtractFrom)
    {
        /// @todo Maybe handle filename paths with spaces?
        size_t SplitPos = ExtractFrom.find_first_of(" \t");
        return ExtractFrom.substr(0,SplitPos);
    }

    /// @brief Creates and launches a new process.
    /// @remarks The path to the executable to be launched must appear in both parameters to this function.
    /// @param ExecutablePath Specifies the path to the exe that will be launched. MUST contain
    /// the path to the executable being launched.
    /// @param Arguments The space separated arguments given to the exe being launched. This MUST include
    /// the path the executable as the first argument.
    /// @return Returns a ProcessInfo struct containing information about the launched process.
    [[nodiscard]]
    ProcessInfo CreateCommandProcess(StringView ExecutablePath, const StringView Arguments)
    {
        int Pipes[2];
        if( ::pipe(Pipes) < 0 ) {
            throw std::runtime_error("Unable to create pipe for child process.");
        }

        std::cout.flush(); // Clean out the pipes before they may be important.
        pid_t ProcessID = ::fork();
        if( ProcessID == 0 ) { // Child
            ::close( Pipes[0] ); // Close Read end of pipe.
            ::dup2( Pipes[1], 1 ); // Direct cout file descriptor to our pipe.
            //::dup2( Pipes[1], 2 ); // Direct cerr file descriptor to our pipe.
            ::close( Pipes[1] ); // Done mangling pipes.

            std::vector<Mezzanine::String> ArgVector;
            String TempStr;
            for( StringView::iterator StrIt = Arguments.begin() ; StrIt != Arguments.end() ; ++StrIt )
            {
                if( (*StrIt) == ' ' || (*StrIt) == '\t' ) {
                    if( !TempStr.empty() ) {
                        ArgVector.push_back(TempStr);
                        TempStr.clear();
                    }
                }else{
                    TempStr.push_back(*StrIt);
                }
            }
            if( !TempStr.empty() ) {
                ArgVector.push_back(TempStr);
            }

            char** ArgV = new char*[ArgVector.size() + 1];// +1 for the nullptr at end.
            for( size_t Idx = 0 ; Idx < ArgVector.size() ; ++Idx )
                { ArgV[Idx] = strdup( ArgVector[Idx].c_str() ); }
            ArgV[ArgVector.size()] = nullptr;//*/

            if( execvp(ExecutablePath.data(),ArgV) < 0 ) {
                // Welp...it's been a good ride.
                int ErrorNum = errno;
                std::cout << "Process Error: " << ::strerror(ErrorNum);
                std::exit(EXIT_FAILURE);
            }
            // If all goes well we disappear into a puff of logic at this point
            // But to appease compilers, we'll write code that pretends we didn't
            return { 0, 0 };
        }else if( ProcessID > 0 ) { // Parent
            ::close( Pipes[1] ); // Close Write end of pipe
            return { Pipes[0], ProcessID };
        }else{
            throw std::runtime_error("Unable to create forked process.");
        }
    }
#endif // MEZZ_Windows

    /// @brief Launches a new process with the given command and collects it's output.
    /// @param ExecutablePath The path to the executable to be launched.
    /// @param Command The arguments given to the launched executable.
    /// @return Returns a command result, containing the exit code and console output of the launched executable.
    [[nodiscard]]
    Testing::CommandResult RunCommandImpl(const StringView ExecutablePath, const StringView Command)
    {
        Testing::CommandResult Result;
#ifdef MEZZ_Windows
        ProcessInfo ChildInfo = CreateCommandProcess( ExecutablePath, Command );
        if( ChildInfo.ErrorNum != 0 ) {
            Result.ExitCode = 1;
            Result.ConsoleOutput = ChildInfo.ErrorStr;
            return Result;
        }

        DWORD BytesRead = 0;
        CHAR PipeBuf[1024];
        while( ::ReadFile(ChildInfo.ChildPipe,PipeBuf,sizeof(PipeBuf),&BytesRead,NULL) )
        {
            if( BytesRead == 0 ) {
                break;
            }
            Result.ConsoleOutput.append(PipeBuf,BytesRead);
        }
        ::CloseHandle(ChildInfo.ChildPipe);

        DWORD ExitStatus;
        ::GetExitCodeProcess(ChildInfo.ChildProcess,&ExitStatus);
        Result.ExitCode = ExitStatus;
        ::CloseHandle(ChildInfo.ChildProcess);
#else // Mezz_Windows
        String NonConstExecPath{ ExecutablePath };
        ProcessInfo ChildInfo = CreateCommandProcess( NonConstExecPath, Command );

        ssize_t BytesRead = -1;
        char PipeBuf[1024];
        // Start reading and keep on reading until we hit an error or EoF.
        while( ( BytesRead = ::read(ChildInfo.ChildPipe,PipeBuf,sizeof(PipeBuf)) ) > 0 )
            { Result.ConsoleOutput.append(PipeBuf,static_cast<size_t>(BytesRead)); }
        ::close(ChildInfo.ChildPipe);

        int Status = -1;
        ::waitpid(ChildInfo.ChildPID,&Status,0);
        if( WIFEXITED(Status) ) {
            Result.ExitCode = WEXITSTATUS(Status);
        }else if( WIFSIGNALED(Status) ) {
            Result.ExitCode = WTERMSIG(Status);
        }else{
            // No idea what else could have happened
            Result.ExitCode = Status;
        }
#endif // MEZZ_Windows
        // Trim newlines
        while( CanTrimBack(Result.ConsoleOutput) )
            { Result.ConsoleOutput.pop_back(); }
        return Result;
    }
}

namespace Mezzanine {
namespace Testing {
    ///////////////////////////////////////////////////////////////////////////////
    // Output to String

    CommandResult RunCommand(const StringView ExecutablePath, const StringView Command)
    {
        const Mezzanine::String SafePath( SanitizeProcessCommand(ExecutablePath) );
        const Mezzanine::String SafeCommand( SanitizeProcessCommand(Command) );
        if( SafeCommand != Command )
            { throw std::runtime_error("Command included unsafe characters, it would not run correctly."); }
        return RunCommandImpl(SafePath,SafeCommand);
    }

    CommandResult RunCommand(const StringView Command)
    {
#ifdef MEZZ_Windows
        // Windows is happy to parse just a single string for everything.
        StringView Empty;
        return RunCommand(Empty,Command);
#else // Mezz_Windows
        // Posix is NOT happy to do the same.  The strings must be separate.
        const Mezzanine::String ExecPath{ ExtractExecPath(Command) };
        return RunCommand(ExecPath,Command);
#endif // MEZZ_Windows
    }

    String GetCommandOutput(const StringView ExecutablePath, const StringView Command)
    {
        const Mezzanine::String SafePath( SanitizeProcessCommand(ExecutablePath) );
        const Mezzanine::String SafeCommand( SanitizeProcessCommand(Command) );
        if( SafeCommand != Command )
            { throw std::runtime_error("Command included unsafe characters, it would not run correctly."); }
        return RunCommandImpl(SafePath,SafeCommand).ConsoleOutput;
    }

    String GetCommandOutput(const StringView Command)
    {
#ifdef MEZZ_Windows
        // Windows is happy to parse just a single string for everything.
        StringView Empty;
        return GetCommandOutput(Empty,Command);
#else // Mezz_Windows
        // Posix is NOT happy to do the same.  The strings must be separate.
        const Mezzanine::String ExecPath{ ExtractExecPath(Command) };
        return GetCommandOutput(ExecPath,Command);
#endif // MEZZ_Windows
    }

    ///////////////////////////////////////////////////////////////////////////////
    // Output to File

    Integer OutputCommandToFile(const StringView ExecutablePath,
                                const StringView Command,
                                const StringView OutputFileName)
    {
        const Mezzanine::String SafePath( SanitizeProcessCommand(ExecutablePath) );
        const Mezzanine::String SafeCommand( SanitizeProcessCommand(Command) );
        const Mezzanine::String SafeOutputFileName( SanitizeProcessCommand(OutputFileName) );
        if( SafeCommand != Command )
            { throw std::runtime_error("Command included unsafe characters, it would not run correctly."); }
        CommandResult Result = RunCommandImpl(SafePath,SafeCommand);
        std::ofstream OutputFile(SafeOutputFileName,std::ios::trunc);
        OutputFile << Result.ConsoleOutput;
        return Result.ExitCode;
    }

    Integer OutputCommandToFile(const StringView Command,
                                const StringView OutputFileName)
    {
#ifdef MEZZ_Windows
        // Windows is happy to parse just a single string for everything.
        StringView Empty;
        return OutputCommandToFile(Empty,Command,OutputFileName);
#else // Mezz_Windows
        // Posix is NOT happy to do the same.  The strings must be separate.
        const Mezzanine::String ExecPath{ ExtractExecPath(Command) };
        return OutputCommandToFile(ExecPath,Command,OutputFileName);
#endif // MEZZ_Windows
    }

    String GetFileContents(const StringView FileName)
    {
        const String PickyName(FileName); // Picky because std::iostreams are not StringView aware yet.
        std::ifstream ResultFile(PickyName);
        return String( std::istreambuf_iterator<char>(ResultFile), {} );
    }
}// Testing
}// Mezzanine

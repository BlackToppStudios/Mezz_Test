// © Copyright 2010 - 2018 BlackTopp Studios Inc.
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
#ifndef Mezz_Test_TimingTools_h
#define Mezz_Test_TimingTools_h

/// @file
/// @brief TestData, TestDataStorage and UnitTestGroup class definitions.

#include "DataTypes.h"
#include "SuppressWarnings.h"

#include <chrono>


namespace Mezzanine
{
    namespace Testing
    {
        SAVE_WARNING_STATE
        SUPPRESS_CLANG_WARNING("-Wpadded") // Emscripten complains here.
            /// @brief A simple piece of data to represent the length of a named period of time.
            struct MEZZ_LIB NamedDuration
            {
                /// @brief What was it called?
                Mezzanine::String Name;

                /// @brief How long did it take.
                std::chrono::nanoseconds Duration;
            };
        RESTORE_WARNING_STATE

        /// @brief An easy way to get the time something took to execute.
        class MEZZ_LIB TestTimer
        {
        private:
            /// @brief The time this was constructed.
            std::chrono::high_resolution_clock::time_point BeginTimer;

        public:
            /// @brief Simply Creating this starts the timer.
            TestTimer()
                : BeginTimer(std::chrono::high_resolution_clock::now())
                {}

            /// @brief How long since this started.
            /// @return An std::chrono::duration in nanoseconds containing the difference between now and when
            /// timing was started.
            std::chrono::nanoseconds GetLength();

            /// @brief How long since this started and give it a name for added meaning.
            /// @param Name The name of the time period that just elapsed.
            NamedDuration GetNameDuration(const Mezzanine::String& Name);
        };// TestTimer

        /// @brief Get a human readable string, annotated with minutes, seconds, etc...
        /// @param Duration Some amount of nano seconds that is probably preposterous for humans to grok.
        Mezzanine::String MEZZ_LIB PrettyDurationString(std::chrono::nanoseconds Duration);

        /// @brief Pretty print a NamedDuration.
        /// @param Stream the stream, likely cout to send it.
        /// @param TimingToStream A single NameDuration.
        /// @return The modified stream.
        std::ostream& MEZZ_LIB operator<<(std::ostream& Stream, const NamedDuration& TimingToStream);

        SAVE_WARNING_STATE
        SUPPRESS_CLANG_WARNING("-Wpadded") // Emscripten complains about this and it not performance sensitive

            /// @brief A set of numbers all the Microbenchmarks return.
            struct MEZZ_LIB MicroBenchmarkResults
            {
                /// @brief A integral type suitable for counting any reasonable execution counts.
                using CountType = Mezzanine::UInt64;
                /// @brief A time precise enough for very small benchmarks.
                using TimeType = std::chrono::nanoseconds;
                /// @brief A collection of timings not yet processed
                using TimingLists = std::vector<TimeType>;

                /// @brief From a collection of nanoseconds fill out this structure.
                /// @param Timings A list of nanoseconds to measure and get the interesting numbers from.
                /// @param PrecalculatedTotal Sometimes the total runtime is acquired while running tests. If this is
                /// non-zero this will be used instead of calculated.
                MicroBenchmarkResults(const TimingLists& Timings, const TimeType& PrecalculatedTotal = TimeType{0});
                MicroBenchmarkResults(MicroBenchmarkResults&) = default;
                ~MicroBenchmarkResults() = default;

                /// @brief Get an Index that corresponds to the percentile of performance.
                /// @param Percent Where to reach into the timings. With 1.0 the slowest and 0.0 the fastest.
                /// @return A index for a value from the Original Timings vector.
                TimingLists::size_type GetIndexFromPercent(PreciseReal Percent) const;
                /// @brief Get a value from the original timings that corresponds to a percentage into to it.
                /// @param Percent Where to reach into the timings. With 1.0 the slowest and 0.0 the fastest.
                /// @return A value from the Original Timings vector.
                TimeType GetIndexValueFromPercent(PreciseReal Percent) const;

                /// @brief How many times was the timed item executed/
                CountType Iterations = 0;
                /// @brief How much was the total runtime with as much of the benchmark removed as possibble.
                TimeType Total = TimeType{0};
                /// @brief The mean execution time; the Total time divided by the number of iterations.
                TimeType Average = TimeType{0};
                /// @brief The fastest (fewest time units) execution time. Defaults to one hour
                TimeType Fastest = TimeType{0};
                /// @brief The timing that beats out only 1 percent of the others at being the fastest.
                /// @sa GetIndexValueFromPercent
                TimeType FasterThan99Percent = TimeType{0};
                /// @brief The timing that beats out 10 percent of the others at being the fastest.
                /// @sa GetIndexValueFromPercent
                TimeType FasterThan90Percent = TimeType{0};
                /// @brief The meduan execution time; the execution time in the middle.
                TimeType Median = TimeType{0};
                /// @brief The timing that beats out 90 percent of the others at being the fastest.
                /// @sa GetIndexValueFromPercent
                TimeType FasterThan10Percent = TimeType{0};
                /// @brief The timing that beats out 99 percent of the others at being the fastest.
                /// @sa GetIndexValueFromPercent
                TimeType FasterThan1Percent = TimeType{0};
                /// @brief The slowest (most time units) execution time.
                TimeType Slowest = TimeType{0};
                /// @brief The raw times gathered by a test, sorted by performance.
                TimingLists SortedTimings;
                /// @brief The unsorted timings to help study caching effects.
                TimingLists UnsortOriginalTimings;

            };
        RESTORE_WARNING_STATE

        /// @brief Time a single execution of some functor
        /// @tparam Functor Any function-like callable item which accepts no parameters and returns none.
        /// @param  ToTime A functor to time the execution of.
        /// @return A performanc profile as an instance of MicroBenchmarkResults.
        template<typename Functor>
        MicroBenchmarkResults MicroBenchmark(Functor&& ToTime)
        {
            TestTimer Bench;
            ToTime();
            return MicroBenchmarkResults{ {Bench.GetLength()} };
        }

        /// @brief Run the passed functor a number of times and track run times of these.
        /// @tparam Functor Any function-like callable item which accepts no parameters and returns none.
        /// @param ToTime A functor to time the execution of.
        /// @return A performanc profile as an instance of MicroBenchmarkResults.
        template<typename Functor>
        MicroBenchmarkResults MicroBenchmark(const Mezzanine::UInt32& Iterations, Functor&& ToTime)
        {
            MicroBenchmarkResults::TimingLists Results;
            Results.reserve(Iterations);

            for(Mezzanine::UInt32 Counter{0}; Counter<Iterations; Counter++)
            {
                std::chrono::high_resolution_clock::time_point Begin{std::chrono::high_resolution_clock::now()};
                ToTime();
                std::chrono::high_resolution_clock::time_point End{std::chrono::high_resolution_clock::now()};

                MicroBenchmarkResults::TimeType Length
                    {std::chrono::duration_cast<MicroBenchmarkResults::TimeType>(End-Begin)};
                Results.push_back(Length);
            }
            return MicroBenchmarkResults{Results};
        }

        /// @brief Run the passed functor repeatedly until the total execution time exceeds the minumum duration.
        /// @tparam Functor Any function-like callable item which accepts no parameters and returns none.
        /// @details This starts and stops a
        /// @return A performance profile as an instance of MicroBenchmarkResults.
        template<typename Functor>
        MicroBenchmarkResults MicroBenchmark(const std::chrono::nanoseconds& MinimumDuration, Functor&& ToTime)
        {
            MicroBenchmarkResults::TimingLists Results;
            MicroBenchmarkResults::TimeType RunningTotal{0};

            while(MinimumDuration >= RunningTotal)
            {
                std::chrono::high_resolution_clock::time_point Begin{std::chrono::high_resolution_clock::now()};
                ToTime();
                std::chrono::high_resolution_clock::time_point End{std::chrono::high_resolution_clock::now()};

                MicroBenchmarkResults::TimeType Length
                    {std::chrono::duration_cast<MicroBenchmarkResults::TimeType>(End-Begin)};
                RunningTotal += Length;
                Results.push_back(Length);
            }
            return MicroBenchmarkResults{Results, RunningTotal};
        }
    }// Testing
}// Mezzanine

#endif

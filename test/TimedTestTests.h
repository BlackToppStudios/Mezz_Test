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
#ifndef Mezz_Test_TimedTests_h
#define Mezz_Test_TimedTests_h

/// @file
/// @brief Header for unit tests for the testing framework, but only the time sensitive parts

#include "MezzTest.h"
#include "TimingTools.h"

#include <stdexcept>
#include <thread>
#include <random>
#include <vector>
#include <numeric>

/// @brief A class for
class MultilengthSleeper
{
public:
    using Sleep = Mezzanine::Testing::MicroBenchmarkResults::TimeType;
    using SleepContainer = std::vector<Sleep>;

    const SleepContainer Sleeps;
    SleepContainer::const_iterator CurrentSleep;

    MultilengthSleeper(const SleepContainer& StartingSleeps)
        : Sleeps(StartingSleeps), CurrentSleep{Sleeps.cbegin()}
        {}

    // Force passing this by reference to prevent copies and appease apple clang inability to have
    // reasonable move and copy constructors on vectors
    MultilengthSleeper(const MultilengthSleeper&) = delete;
    ~MultilengthSleeper() = default;
    MultilengthSleeper& operator=(const MultilengthSleeper&) = delete;
    MultilengthSleeper& operator=(MultilengthSleeper&&) = delete;

    void operator()()
    {
        if(Sleeps.cend() == CurrentSleep)
            { CurrentSleep = Sleeps.cbegin(); }
        std::this_thread::sleep_for(*CurrentSleep);
        CurrentSleep++;
    }
};

/// @brief TestTests to verify that Warnings works correctly.
/// @details This class is not called directly by the Unit Test framework and is called by the TimedTestTest
/// to verify everything fails.
SILENT_TEST_GROUP(WarningTimedTestTests, WarningTimedTest)
{
    // Here are some examples of test that should warn.
    TEST_TIMED("TestTimedWarning", std::chrono::microseconds(5000), std::chrono::microseconds(1000), []{});
    TEST_TIMED_UNDER("TestTimedUnderWarning", std::chrono::microseconds(1),
               []{ std::this_thread::sleep_for( std::chrono::milliseconds(5) ); });
}

/// @brief This is the actual Test class. This tests our Test Macros that are time sensitive.
BENCHMARK_TEST_GROUP(TimedTestTests, TimedTest)
{
    // Lets make a random time for these tests so if this is run on a bunch of VMs on the same hardware there will be
    // subtle variations in the timing and it won't cause all the VMs to wake at the same time and possibly delay some
    // of the tests.
    std::mt19937 MersennTwisterRandomSource;
    MersennTwisterRandomSource.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> DistLength(10000,15000);
    std::uniform_int_distribution<std::mt19937::result_type>::result_type SleepTicks =
            DistLength(MersennTwisterRandomSource);

    const std::chrono::milliseconds SleepTime(SleepTicks);
    auto Sleeper = [SleepTime]{ std::this_thread::sleep_for(SleepTime); };
    const std::chrono::milliseconds MilliSecondEpsilon{2000};

    // Positive tests This should serve as examples for how to use this and get tests that passed.
    // These amounts of time very short to be measuring this way. longer running tests can be more precise.
    TEST_TIMED("TestTimedPassing", SleepTime, MilliSecondEpsilon, Sleeper);

    TEST_TIMED_UNDER("TestTimedUnderPassing", std::chrono::microseconds(5000), []{ });

    // Warning Timed Tests
    class WarningTimedTestTests Warnifier;
    Warnifier();
    for(const Mezzanine::Testing::TestData& SingleResult : Warnifier)
        { TEST_EQUAL(SingleResult.TestName, Mezzanine::Testing::TestResult::Warning, SingleResult.Results); }

    // Tests of Benchmark tools need a little and serve as poor examples of how to use this. These are testing
    // the framework tools and not actually using the gaurantees that statistical results provice. See below for a
    // better example
    using Mezzanine::Testing::MicroBenchmarkResults;
    using Mezzanine::Testing::MicroBenchmark;

    const MicroBenchmarkResults SingleBench = MicroBenchmark(Sleeper);
    const MicroBenchmarkResults::TimeType SingleLowerRange{SleepTime - MilliSecondEpsilon};
    const MicroBenchmarkResults::TimeType SingleUpperRange{SleepTime + MilliSecondEpsilon};

    TEST_EQUAL("MicroBenchmarkSingleIterations", MicroBenchmarkResults::CountType{1}, SingleBench.Iterations);

    TEST_EQUAL("MicroBenchmarkSingleTimingsSet",
               MicroBenchmarkResults::CountType{1},
               SingleBench.SortedTimings.size());

    TEST_WITHIN_RANGE("MicroBenchmarkSingleTotal",
                      SingleLowerRange.count(),
                      SingleUpperRange.count(),
                      SingleBench.Total.count());

    TEST_WITHIN_RANGE("MicroBenchmarkSingleAverage",
                      SingleLowerRange.count(),
                      SingleUpperRange.count(),
                      SingleBench.Average.count());

    TEST_WITHIN_RANGE("MicroBenchmarkSingleFastest",
                      SingleLowerRange.count(),
                      SingleUpperRange.count(),
                      SingleBench.Fastest.count());

    TEST_WITHIN_RANGE("MicroBenchmarkSinglePercentile99th",
                      SingleLowerRange.count(),
                      SingleUpperRange.count(),
                      SingleBench.FasterThan1Percent.count());

    TEST_WITHIN_RANGE("MicroBenchmarkSinglePercentile90th",
                      SingleLowerRange.count(),
                      SingleUpperRange.count(),
                      SingleBench.FasterThan10Percent.count());

    TEST_WITHIN_RANGE("MicroBenchmarkSingleMedian",
                      SingleLowerRange.count(),
                      SingleUpperRange.count(),
                      SingleBench.Median.count());

    TEST_WITHIN_RANGE("MicroBenchmarkSingleSlowest",
                      SingleLowerRange.count(),
                      SingleUpperRange.count(),
                      SingleBench.Slowest.count());

    // Setup for triple bench test
    const MultilengthSleeper::Sleep FastestTime{std::chrono::milliseconds{1000}};
    const MultilengthSleeper::Sleep AverageTime{std::chrono::milliseconds{3000}};
    const MultilengthSleeper::Sleep SlowestTime{std::chrono::milliseconds{5000}};
    const MultilengthSleeper::Sleep TripleSleepUpwardEpsilon{std::chrono::milliseconds{1000}};
    const MultilengthSleeper::Sleep TripleSleepDownwardEpsilon{std::chrono::milliseconds{100}};

    MultilengthSleeper TripleSleeps({FastestTime, AverageTime, SlowestTime});

    const MicroBenchmarkResults::TimeType TotalLowerRange{FastestTime + AverageTime + SlowestTime -
        TripleSleepDownwardEpsilon};
    const MicroBenchmarkResults::TimeType TotalUpperRange{FastestTime + AverageTime + SlowestTime +
        TripleSleepUpwardEpsilon};

    const MicroBenchmarkResults::TimeType AverageLowerRange{AverageTime - TripleSleepDownwardEpsilon};
    const MicroBenchmarkResults::TimeType AverageUpperRange{AverageTime + TripleSleepUpwardEpsilon};

    const MicroBenchmarkResults::TimeType FastestLowerRange{FastestTime - TripleSleepDownwardEpsilon};
    const MicroBenchmarkResults::TimeType FastestUpperRange{FastestTime + TripleSleepUpwardEpsilon};

    const MicroBenchmarkResults::TimeType SlowestLowerRange{SlowestTime - TripleSleepDownwardEpsilon};
    const MicroBenchmarkResults::TimeType SlowestUpperRange{SlowestTime + TripleSleepUpwardEpsilon};

    const MicroBenchmarkResults ThreeIterationBench = MicroBenchmark(3, std::move(TripleSleeps));

    TEST_EQUAL("MicroBenchmarkIterationsIterations",
               MicroBenchmarkResults::CountType{3},
               ThreeIterationBench.Iterations);

    TEST_EQUAL("MicroBenchmarkIterationsTimingsSet",
               MicroBenchmarkResults::CountType{3},
               ThreeIterationBench.SortedTimings.size());

    TEST_WITHIN_RANGE("MicroBenchmarkIterationsTotal",
                      TotalLowerRange.count(),
                      TotalUpperRange.count(),
                      ThreeIterationBench.Total.count());

    TEST_WITHIN_RANGE("MicroBenchmarkIterationsAverage",
                      AverageLowerRange.count(),
                      AverageUpperRange.count(),
                      ThreeIterationBench.Average.count());

    TEST_WITHIN_RANGE("MicroBenchmarkIterationsFastest",
                      FastestLowerRange.count(),
                      FastestUpperRange.count(),
                      ThreeIterationBench.Fastest.count());

    TEST_WITHIN_RANGE("MicroBenchmarkIterationsPercentile1st",
                      FastestLowerRange.count(),
                      AverageUpperRange.count(),
                      ThreeIterationBench.FasterThan99Percent.count());

    TEST_WITHIN_RANGE("MicroBenchmarkIterationsPercentile10th",
                      FastestLowerRange.count(),
                      AverageUpperRange.count(),
                      ThreeIterationBench.FasterThan90Percent.count());

    TEST_WITHIN_RANGE("MicroBenchmarkIterationsMedian",
                      AverageLowerRange.count(),
                      AverageUpperRange.count(),
                      ThreeIterationBench.Median.count());

    TEST_WITHIN_RANGE("MicroBenchmarkIterationsPercentile90th",
                      AverageLowerRange.count(),
                      SlowestUpperRange.count(),
                      ThreeIterationBench.FasterThan10Percent.count());

    TEST_WITHIN_RANGE("MicroBenchmarkIterationsPercentile99th",
                      AverageLowerRange.count(),
                      SlowestUpperRange.count(),
                      ThreeIterationBench.FasterThan1Percent.count());

    TEST_WITHIN_RANGE("MicroBenchmarkIterationsSlowest",
                      SlowestLowerRange.count(),
                      SlowestUpperRange.count(),
                      ThreeIterationBench.Slowest.count());

    // Duration based benchmarks
    const MultilengthSleeper::Sleep DurationTestMultiplier{3000000};

    const MultilengthSleeper::Sleep Pentile1Time{1 * DurationTestMultiplier};
    const MultilengthSleeper::Sleep Pentile2Time{3 * DurationTestMultiplier};
    const MultilengthSleeper::Sleep Pentile3Time{5 * DurationTestMultiplier};
    const MultilengthSleeper::Sleep Pentile4Time{7 * DurationTestMultiplier};
    const MultilengthSleeper::Sleep Pentile5Time{9 * DurationTestMultiplier};

    MultilengthSleeper PentileSleeps({Pentile1Time, Pentile2Time, Pentile3Time, Pentile4Time, Pentile5Time});

    const MultilengthSleeper::Sleep PentileDelta{MicroBenchmarkResults::TimeType {3 * DurationTestMultiplier}};
    const MultilengthSleeper::Sleep Pentile1TimeUpper{Pentile1Time > PentileDelta ?
                                                      Pentile1Time + PentileDelta :
                                                      PentileDelta + PentileDelta};
    const MultilengthSleeper::Sleep Pentile1TimeLower{Pentile1Time > PentileDelta ?
                                                      Pentile1Time - PentileDelta :
                                                      MultilengthSleeper::Sleep{0}};
    //const MultilengthSleeper::Sleep Pentile2TimeUpper{Pentile2Time + PentileDelta};
    //const MultilengthSleeper::Sleep Pentile2TimeLower{Pentile2Time - PentileDelta};
    //const MultilengthSleeper::Sleep Pentile3TimeUpper{Pentile3Time + PentileDelta};
    //const MultilengthSleeper::Sleep Pentile3TimeLower{Pentile3Time - PentileDelta};
    //const MultilengthSleeper::Sleep Pentile4TimeUpper{Pentile4Time + PentileDelta};
    //const MultilengthSleeper::Sleep Pentile4TimeLower{Pentile4Time - PentileDelta};
    //const MultilengthSleeper::Sleep Pentile5TimeUpper{Pentile5Time + PentileDelta};
    const MultilengthSleeper::Sleep Pentile5TimeLower{Pentile5Time - PentileDelta};

    const MultilengthSleeper::Sleep PentileBenchmarkDuration{25000 * DurationTestMultiplier};
    const MultilengthSleeper::Sleep PentileTotalDelta{PentileDelta * 10};

    // Some test systems are just slow and take extra time so we accept double the delta upward
    const MultilengthSleeper::Sleep PentileExpectedTotalUpper{PentileBenchmarkDuration + (2 * PentileTotalDelta)};
    const MultilengthSleeper::Sleep PentileExpectedTotalLower{PentileBenchmarkDuration - PentileTotalDelta};

    const MicroBenchmarkResults::CountType PentileSinglePassExpectedCount
        { MicroBenchmarkResults::CountType(Pentile1Time.count() +  Pentile2Time.count() + Pentile3Time.count() +
            Pentile4Time.count() + Pentile5Time.count()) };

    // 5 is the expected multiple, but these are often slower
    //const MicroBenchmarkResults::CountType PentileBenchmarkExpectedCountLower
    //    { PentileBenchmarkDuration.count() / PentileSinglePassExpectedCount * 3};
    const MicroBenchmarkResults::CountType PentileBenchmarkExpectedCountUpper
        { PentileBenchmarkDuration.count() / PentileSinglePassExpectedCount * 6};

    // The actual work
    const MicroBenchmarkResults DurationBench = MicroBenchmark(PentileBenchmarkDuration,
                                                               std::move(PentileSleeps));

    TEST("MicroBenchmarkDurationIterations", PentileBenchmarkExpectedCountUpper > DurationBench.Iterations);

    TEST_EQUAL("MicroBenchmarkDurationTimingsSet",
               DurationBench.Iterations,
               DurationBench.SortedTimings.size());

    TEST_WITHIN_RANGE("MicroBenchmarkDurationTotal",
                      PentileExpectedTotalLower.count(),
                      PentileExpectedTotalUpper.count(),
                      DurationBench.Total.count());

    MicroBenchmarkResults::TimeType ExpectedAverage
        { std::accumulate(DurationBench.UnsortOriginalTimings.cbegin(),
                          DurationBench.UnsortOriginalTimings.cend(),
                           MicroBenchmarkResults::TimeType{0})
          / DurationBench.UnsortOriginalTimings.size() };
    TEST_WITHIN_RANGE("MicroBenchmarkDurationAverage",
                      ExpectedAverage.count() - 1,
                      ExpectedAverage.count() + 1,
                      DurationBench.Average.count());

    TEST_WITHIN_RANGE("MicroBenchmarkDurationFastest",
                      Pentile1TimeLower.count(),
                      Pentile1TimeUpper.count(),
                      DurationBench.Fastest.count());

    TEST_WITHIN_RANGE("MicroBenchmarkDurationPercentile1st",
                      Pentile1TimeLower.count(),
                      Pentile1TimeUpper.count(),
                      DurationBench.FasterThan99Percent.count());

    TEST_WITHIN_RANGE("MicroBenchmarkDurationPercentile10th",
                      Pentile1TimeLower.count(),
                      DurationBench.GetIndexValueFromPercent(0.2).count(),
                      DurationBench.FasterThan90Percent.count());

    TEST_WITHIN_RANGE("MicroBenchmarkDurationMedian",
                      DurationBench.GetIndexValueFromPercent(0.4).count(),
                      DurationBench.GetIndexValueFromPercent(0.6).count(),
                      DurationBench.Median.count());

    TEST_WITHIN_RANGE("MicroBenchmarkDurationPercentile90th",
                      Pentile5TimeLower.count(),
                      DurationBench.FasterThan1Percent.count(),
                      DurationBench.FasterThan10Percent.count());

    TEST("MicroBenchmarkDurationPercentile99th", Pentile5TimeLower.count()< DurationBench.FasterThan1Percent.count());
    TEST("MicroBenchmarkDurationSlowest", Pentile5TimeLower.count() < DurationBench.Slowest.count());

    // This is a halfway reasonable example to compare two potentially close performing tasks. In your tests you should
    // never use a hardcoded number because any number of factors could change it. Rather generate two measurements and
    // and somehow compare those.

    // These represent two different algorithms to check. Actually having both in tests lets us check them as platform,
    // cache situations, type implementations, compilers and any other conditions.
    auto FastThingToCheck = []{ std::this_thread::sleep_for(std::chrono::milliseconds{12}); };
    auto SlowThingToCheck = []{ std::this_thread::sleep_for(std::chrono::milliseconds{17}); };

    // Do 1000 iterations of each
    const MicroBenchmarkResults FastMeasurements = MicroBenchmark(1000, std::move(FastThingToCheck));
    const MicroBenchmarkResults SlowMeasurements = MicroBenchmark(1000, std::move(SlowThingToCheck));

    TEST("ExampleAlgorithmComparis", FastMeasurements.FasterThan10Percent < SlowMeasurements.FasterThan90Percent);

}

#endif

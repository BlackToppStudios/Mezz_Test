# README #

The Mezzanine is a set of multi platform C++ libraries designed to help create high performance
videos games. The Mezzanine is focused on games requiring 3d physics, 3d graphics, 3d sounds and
common facilities that such games need. This is just one component of several.

## About this Library ##

This library, Test, contains tools for unit testing, recovering from segfaults/sigaborts/crashes,
and some minor tools for faciliting manual testing. This library can also emit JUnit style test results
in XML that tools like Jenkins can use to produce reports on your test results.

Currently this does not include tools for automated User interface testing or mocking, but that could be
possible in the future.

## Build Status ##

The current Linux/Travis build status is: 
[![Build Status](https://travis-ci.org/BlackToppStudios/Mezz_Test.svg?branch=master)](https://travis-ci.org/BlackToppStudios/Mezz_Test)

The current Windows/Appveyor build status is:
[![Build Status](https://ci.appveyor.com/api/projects/status/github/BlackToppStudios/Mezz_Test?branch=master&svg=true)](https://ci.appveyor.com/project/Sqeaky/mezz-test)

The current test coverage of our code on our last successful build is:
[![codecov](https://codecov.io/gh/BlackToppStudios/Mezz_Test/branch/master/graph/badge.svg)](https://codecov.io/gh/BlackToppStudios/Mezz_Test)

The current Jenkins, which covers Linux (Emscripten, Rasberry Pi, Ubuntu and Fedora), old Mac OS X (High Sierra) and old windows (7 64 bit msvc and mingw), build status is available on the [BTS Jenkins Instance](http://blacktopp.ddns.net:8080/blue/organizations/jenkins/Mezz_Test/activity). The current status is: [![Build Status](http://blacktopp.ddns.net:8080/job/Mezz_Test/job/master/badge/icon)](http://blacktopp.ddns.net:8080/blue/organizations/jenkins/Mezz_Test/activity)

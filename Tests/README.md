These tests use gtest (Google C++ Testing Framework).

You need to build gtest with "OS X Deployment Target: 10.9" setting.
And you need to put gtest.framework into ~/Library/Frameworks.  You
need to build gtest using the compiler switches in Makefile.common
because of C++'s One Definition Rule.

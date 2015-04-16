Splay Tree Map
=============
Map implementation using splay tree. Implemented for PromCPP course, NSU FIT, 8 semester, March - April 2015

Build
==============
Build map_splay_tree library + unit_tests + gtest + measure coverage
--------------
1. cmake -G "<your_platform>"
2. make
3. make coverage (if you want measure tests code coverage)

Notes:

1. All output files will be in build/
2. Coverage info will be in coverage/. Open coverage/index.html, if you want to see detailed information
3. gtest may compile with warnings (g++ bug)
4. Total coverage percent is small, because gtest coverage measuring too. Open coverage/index.html, if you want to see "real" info

Build performance test
--------------
1. cd tests/performance
2. cmake -G "<your_platform>"
3. make

If your want build only map_splay_tree library
--------------
1. cd src/
2. cmake -G "<your_platform>"
3. make

Run
==============
Run all unit-tests
--------------
./build/run_tests

Run all perf-test
--------------
./tests/performance/performance

Files locations
==============
1. map_splay_tree sources are in src/
2. unit-tests are in tests/
3. perf-test is in tests/performance/
4. gtest sources are in gtest/
5. coverage info files are in coverage/ (if you run "make coverage")
6. binaries and libraries in build/ (if you built project)


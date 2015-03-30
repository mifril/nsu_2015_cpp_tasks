Build
==============
Build map_splay_tree + unit_tests + gtest
--------------
cmake -G "<your_platform>"

All build files will be in build/

! gtest may compile with warnings

Build performance test
--------------
cd tests/performance and run cmake -G "<platform>"

If your want build only map_splay_tree library
--------------
go to src/ and run cmake

Run
==============

Run all unit-tests
--------------
./build/task1

Run all perf-test
--------------
./tests/performance/performance

Files locations
==============
1. map_splay_tree sources are in src/
2. unit-tests are in tests/
3. perf-test is in tests/performance/
4. gtest sources are in gtest/


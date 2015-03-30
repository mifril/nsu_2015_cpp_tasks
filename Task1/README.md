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
map_splay_tree sources are in src/
unit-tests are in tests/
perf-test is in tests/performance/
gtest sources are in gtest/


Splay Tree Map
=============
Scheduled executor (C++11)

Build
==============
Build scheduled_executor library + unit_tests + gtest + measure coverage
--------------
1. cmake -G "<your_platform>"
2. make
3. make coverage (if you want measure tests code coverage)

Notes:

1. All output files will be in build/
2. Coverage info will be in coverage/. Open coverage/index.html, if you want to see detailed information
3. gtest may compile with warnings (g++ bug)
4. Total coverage percent is small, because gtest coverage measuring too. Open coverage/index.html, if you want to see "real" info

If your want build only scheduled_executor library
--------------
1. cd src/
2. cmake -G "<your_platform>"
3. make

Run
==============
Run all unit-tests
--------------
./build/run_tests

Files locations
==============
1. scheduled_executor sources are in src/
2. unit-tests are in tests/
3. gtest sources are in gtest/
4. coverage info files are in coverage/ (if you run "make coverage")
5. binaries and libraries in build/ (if you built project)


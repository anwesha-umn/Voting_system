### Unit Testing Files Build Using CMake
To build unit test files, ensure you are in the `testing` directory (which is this one). Within CMakeLists.txt update the unit test file name within `add_executable()`, `target_link_libraries()` and `gtest_discover_tests()`.
Then type the following:

```sh
Project2/testing $ cmake -S . -B build
Project2/testing $ cmake --build build
Project2/testing $ cd build && ctest
```
Note: For running every unit test file you will need to modify CMakeLists.txt.


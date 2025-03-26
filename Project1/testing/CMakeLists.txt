# Adapted from https://google.github.io/googletest/quickstart-cmake.html
# USAGE:
# add testing exe and .cc as well as target testing files (in this case ../src/candidate and ../src/candidate.cpp)
# compile using `cmake -S . -B build` from the testing directory
# then use `cmake --build build`
# and then finally `cd build && ctest`
# Test output in build/Testing/Temporary/LastTest.log

cmake_minimum_required(VERSION 3.14)
project(Project1)

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE  # Add this line
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
  test_src
  ../src/test_src.cc
  ../src/Election
  ../src/Election.cpp
  ../src/candidate
  ../src/candidate.cpp
  ../src/ballot
  ../src/ballot.cpp
  ../src/stvballot
  ../src/stvballot.cpp
  ../src/pluralityballot
  ../src/pluralityballot.cpp
  ../src/userinterface
  ../src/userinterface.cpp

)
target_link_libraries(
  test_src
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(test_src)
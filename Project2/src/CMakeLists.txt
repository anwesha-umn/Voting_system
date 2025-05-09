cmake_minimum_required(VERSION 3.30)
project(Project2)

# Set C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add a flag to ensure the console subsystem is used (for Windows)
if (WIN32)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-subsystem,console")
endif()

# List all source and header files
set(SOURCES
        main.cpp
        userinterface.cpp
        candidate.cpp
        Election.cpp
        ballot.cpp
        pluralityballot.cpp
        stvballot.cpp
        plurality.cpp
        stv.cpp
        MVlogic.cpp
        mvballot.cpp
)

set(HEADERS
        userinterface.h
        candidate.h
        Election.h
        ballot.h
        pluralityballot.h
        stvballot.h
        plurality.h
        stv.h
        MVlogic.h
        mvballot.h
)

# Add the executable
add_executable(Project2 ${SOURCES} ${HEADERS})

# Include current directory for header files
target_include_directories(Project1 PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Enable warnings
target_compile_options(Project1 PRIVATE -Wall -Wextra -pedantic)

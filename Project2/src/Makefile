# File: Makefile
# Description: allows for easy compilation and cleaning of the election_app executable
# Authored by Zoe Sepersky

# **NOTE**: In stv.h, FRIEND_TEST is being used for testing a few private methods. In case any issue arises while compiling
# using makefile, please comment out the FRIEND_TESTs in stv.h. 

# Compiler settings
CXX := g++
CXXFLAGS := -std=c++23 -Wall -Wextra -I.
BUILD_DIR := build

# Source files
SRCS := ballot.cpp candidate.cpp Election.cpp plurality.cpp stv.cpp MVlogic.cpp userinterface.cpp \
        pluralityballot.cpp stvballot.cpp mvballot.cpp main.cpp

# Object files
OBJS := $(addprefix $(BUILD_DIR)/, $(SRCS:.cpp=.o))


# Executables
MAIN_EXEC := election_app


# Targets
all: $(MAIN_EXEC) $(TEST_EXEC)

$(MAIN_EXEC): $(filter-out $(BUILD_DIR)/test_%, $(OBJS))
	$(CXX) $(LDFLAGS) -o $@ $^

# Pattern rule for .cpp files
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# if you want to run right away, runs executable right away
run: 
	make; clear; ./election_app;

# Clean
clean:
	rm -rf $(BUILD_DIR) $(MAIN_EXEC)

# Doxygen docs
docs:
	cd ../documentation; doxygen Doxyfile; cd ../src;

.PHONY: all clean


#######################################################
# File: Makefile
# Description: Windows-compatible Makefile for election_app
# Updated for Windows systems

# Compiler settings
# CXX := g++
# CXXFLAGS := -std=c++17 -Wall -Wextra -I.
# LDFLAGS := 
# BUILD_DIR := build

# # Source files
# SRCS := ballot.cpp candidate.cpp Election.cpp plurality.cpp stv.cpp MVlogic.cpp userinterface.cpp \
#         pluralityballot.cpp stvballot.cpp mvballot.cpp main.cpp

# # Object files
# OBJS := $(addprefix $(BUILD_DIR)/, $(SRCS:.cpp=.o))

# # Executables
# MAIN_EXEC := election_app.exe

# # Targets
# all: $(MAIN_EXEC)

# $(MAIN_EXEC): $(OBJS)
# 	$(CXX) $(LDFLAGS) -o $@ $^

# # Create build directory if it doesn't exist
# $(BUILD_DIR):
# 	@if not exist "$(BUILD_DIR)" mkdir "$(BUILD_DIR)"

# # Pattern rule for .cpp files
# $(BUILD_DIR)/%.o: %.cpp | $(BUILD_DIR)
# 	$(CXX) $(CXXFLAGS) -c $< -o $@

# # Clean (Windows version)
# clean:
# 	@if exist "$(BUILD_DIR)" rmdir /s /q "$(BUILD_DIR)"
# 	@if exist "$(MAIN_EXEC)" del /q "$(MAIN_EXEC)"

# .PHONY: all clean
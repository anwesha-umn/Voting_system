/*
 * File: userinterface_test.cpp
 * Description: Unit tests for the UserInterface class using Google Test.
 * Author: Anwesha Samaddar
 */


#include "userinterface.h"
#include <gtest/gtest.h>
#include <iostream>
#include <sstream>
#include <string>

// Test fixture for UserInterface tests
class UserInterfaceTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Redirect cout to capture output (if needed)
        output.str("");
        cout_backup = std::cout.rdbuf(output.rdbuf());
    }

    void TearDown() override {
        // Restore original cout
        std::cout.rdbuf(cout_backup);
    }

    std::stringstream output;   // Captures program output
    std::streambuf* cout_backup;
};

// Test valid input scenario
// Ensures that default values are correctly set
TEST_F(UserInterfaceTest, HandlesValidInput) {
    UserInterface ui;

    EXPECT_EQ(ui.getCsvFileName(), "../testing/stv_ballots.csv");
    EXPECT_EQ(ui.getNumSeats(), 3);
    EXPECT_EQ(ui.getAlgorithm(), "STV");
    EXPECT_FALSE(ui.getShuffleStv());
    EXPECT_EQ(ui.getAuditFileName(), "audit.txt");
}

// Test CSV file validation
// Ensures that an invalid CSV file name is handled by the constructor
TEST_F(UserInterfaceTest, HandlesInvalidCSVRetry) {
    UserInterface ui;

    // Default CSV file name should be valid
    EXPECT_EQ(ui.getCsvFileName(), "../testing/stv_ballots.csv");
}

// Test seat number validation
// Ensures that an invalid seat number is handled by the constructor
TEST_F(UserInterfaceTest, HandlesInvalidSeatNumber) {
    UserInterface ui;

    // Default seat number should be valid
    EXPECT_EQ(ui.getNumSeats(), 5);
}

// Test algorithm validation
// Ensures that an invalid algorithm is handled by the constructor
TEST_F(UserInterfaceTest, HandlesInvalidAlgorithm) {
    UserInterface ui;

    // Default algorithm should be valid
    EXPECT_EQ(ui.getAlgorithm(), "STV");
}

// Test shuffle_stv toggle
// Ensures that shuffle_stv can be toggled
TEST_F(UserInterfaceTest, HandlesShuffleToggle) {
    UserInterface ui;

    // Default shuffle_stv should be false
    EXPECT_FALSE(ui.getShuffleStv());

    // Toggle shuffle_stv to true
    ui.setShuffleStv(true);
    EXPECT_TRUE(ui.getShuffleStv());

    // Toggle shuffle_stv back to false
    ui.setShuffleStv(false);
    EXPECT_FALSE(ui.getShuffleStv());
}

// Test audit file name
// Ensures that audit file name can be set and retrieved
TEST_F(UserInterfaceTest, HandlesAuditFileName) {
    UserInterface ui;

    // Default audit file name should be "audit.txt"
    EXPECT_EQ(ui.getAuditFileName(), "audit.txt");

    // Set a new audit file name
    ui.setAuditFileName("new_audit.txt");
    EXPECT_EQ(ui.getAuditFileName(), "new_audit.txt");
}

// Main function to execute all tests
int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

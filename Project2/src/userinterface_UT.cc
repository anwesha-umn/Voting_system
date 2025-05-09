/*
 * File: userinterface_UT.cc 
 * Description: Unit tests for UserInterface class - verifies:
 *              - CSV file handling
 *              - Election parameter parsing  
 *              - Input validation
 *              - Getter unit tests
 * Author: Anwesha Samaddar
 */

 #include "stv.h"
 #include "userinterface.h"
 #include <gtest/gtest.h>
 #include <string>
 
 // Fixture for testing class UserInterface.
 class UserInterfaceTest : public testing::Test {
  protected:
  
   UserInterfaceTest() {
   }
 
   ~UserInterfaceTest() override {
   }
 
 
   void SetUp() override {
      csv_files = {"../../testing/stv_ballots.csv","../../testing/stv_ballots2.csv"}; // Default CSV filenames
      num_seats = 3;
      algorithm = "STV";
      audit_file = "audit.txt";
      shuffle = true;
   }
 
   void TearDown() override {
   }

 
   // Class members for testing
   std::vector<std::string> csv_files;   
   int num_seats;
   std::string algorithm;
   std::string audit_file;
   bool shuffle;
   UserInterface ui;  // Instance under test
 };
 

 // Tests that UserInterface gets the correct CSV filename
 TEST_F(UserInterfaceTest, GetCsvFileNameReturnsCorrectValue) {
   EXPECT_EQ(ui.getCsvFileNames(), csv_files);
 }
 

 // Tests that UserInterface gets the correct number of seats
 TEST_F(UserInterfaceTest, getNumSeatsFromCsv) {
  UserInterface ui;
  
  // Simulate user input
  std::stringstream input_stream;
  input_stream << "1\n";             // Number of files
  input_stream << "../../testing/plurality_all_inputs_3.csv" << "\n";  // CSV file name
  input_stream << "audit.txt\n";     // Audit file name
  
  // Redirect cin
  auto original_cin = std::cin.rdbuf(input_stream.rdbuf());
  
  ui.getInfo();
  
  // Verify the number of seats was read correctly from CSV
  EXPECT_EQ(ui.getNumSeats(), 3);
  
  // Restore cin
  std::cin.rdbuf(original_cin);
 }
 

 // Tests that UserInterface gets the correct algorithm type from CSV
 TEST_F(UserInterfaceTest, getAlgorithmFromCsv) {
  UserInterface ui;
  
  // Simulate user input
  std::stringstream input_stream;
  input_stream << "1\n";             // Number of files
  input_stream << "../../testing/plurality_all_inputs_3.csv" << "\n";  // CSV file name
  input_stream << "audit.txt\n";     // Audit file name
  
  // Redirect cin
  auto original_cin = std::cin.rdbuf(input_stream.rdbuf());
  
  ui.getInfo();
  
  // Verify the algorithm was read correctly from CSV
  EXPECT_EQ(ui.getAlgorithm(), "PV");
  
  // Restore cin
  std::cin.rdbuf(original_cin);
 }

 
 // Tests that UserInterface gets the correct audit filename
 TEST_F(UserInterfaceTest, GetAuditFileNameReturnsCorrectValue) {
   EXPECT_EQ(ui.getAuditFileName(), audit_file);
 }
 

 // Tests that UserInterface gets the correct shuffle setting
 TEST_F(UserInterfaceTest, GetShuffleStvReturnsCorrectValue) {
   EXPECT_TRUE(ui.getShuffleStv());
 }


 // Tests that audit filename is not empty and equal to simulated audit file name
 TEST_F(UserInterfaceTest, AuditFileNameNotEmpty) {
  UserInterface ui;

  // Simulate user input
  std::stringstream input_stream;
  input_stream << "1\n";             // Number of files
  input_stream << "../../testing/plurality_all_inputs_3.csv\n";  // CSV file
  input_stream << "audit_file.txt\n"; // Audit file name

  // Redirect std::cin
  auto original_cin = std::cin.rdbuf(input_stream.rdbuf());

  ui.getInfo();  // This populates the audit file name

  // Assert that the audit file name is not empty
  EXPECT_FALSE(ui.getAuditFileName().empty());
  EXPECT_EQ(ui.getAuditFileName(),"audit_file.txt");

  // Restore std::cin
  std::cin.rdbuf(original_cin);
}


 // Tests that all CSV filenames have .csv extension
 TEST_F(UserInterfaceTest, AllCsvFilesHaveCorrectExtension) {
  for (const auto& file : ui.getCsvFileNames()) {
    EXPECT_EQ(file.substr(file.size() - 4), ".csv");
  }
 }

 // Tests if invalid file number and invalid file names are handled correctly 
 TEST(UserInterfaceSimpleTest, HandlesInvalidInputs) {
  UserInterface ui;
  
  std::stringstream input_stream;
  input_stream << "-10\nabc\n0\n";  // Three invalid inputs
  
  auto original_cin = std::cin.rdbuf(input_stream.rdbuf());
  
  testing::internal::CaptureStdout();
  EXPECT_EXIT(ui.getInfo(), testing::ExitedWithCode(1), "");
  
  std::string output = testing::internal::GetCapturedStdout();
  std::cin.rdbuf(original_cin);
  
  // verify the program exited after invalid inputs
  SUCCEED();  
}

 
 int main(int argc, char **argv) {
   testing::InitGoogleTest(&argc, argv);
   return RUN_ALL_TESTS();
 }

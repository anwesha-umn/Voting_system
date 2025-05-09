
/*
 * File: userinterface.cpp
 * Description: Implements the user interface. Handles user input for election configuration including:
 *              - Number of csv files entered
 *              - CSV file input/validation
 *              - Extracting Election parameters from CSV files
 *              - Entering Audit file name
 * Author: Anwesha Samaddar & Annabelle Coler
 */

#include "userinterface.h"
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include <sstream>
#include "stv.h"

using namespace std;

// Implements multiple csv files input 
// Allows 3 attempts for each input with clear error messages
void UserInterface::getInfo() {
    string input;
    int attempts;
    bool valid;

    // Get number of files
    int numFiles = 0;
    attempts = 0;
    valid = false;
    while (!valid && attempts < 3) {
        cout << "How many CSV files do you want to input? ";
        cin >> input;

        try {
            // Try converting input to int
            numFiles = stoi(input);
            if (numFiles <= 0) {
                throw invalid_argument("Number must be positive");
            }
            valid = true;
        } catch (...) {
            cout << "Invalid input. Please enter a positive integer." << endl;
            attempts++;
        }
    }

    if (!valid) {
        cerr << "Too many failed attempts. Exiting." << endl;
        exit(1);
    }

    // Get each file name 
    csvFileNames.clear();
    for (int i = 0; i < numFiles; i++) {
        attempts = 0;
        valid = false;
        string filename;
        
        while (!valid && attempts < 3) {
            cout << "Enter CSV file #" << (i+1) << ": ";
            cin >> filename;

            // Validate file extension
            if (filename.size() < 4 || filename.substr(filename.size() - 4) != ".csv") {
                cout << "Error: File must be a CSV file with .csv extension." << endl;
                attempts++;
                continue;
            }

            // Check if file exists and is readable
            ifstream file(filename);
            if (!file) {
                cout << "Error: File does not exist or cannot be opened." << endl;
                attempts++;
            } else {
                // Read file contents 
                string line;
                getline(file, line);
                string currentAlgorithm;  // get election type
                istringstream iss(line);
                getline(iss, currentAlgorithm, ',');

                // For first file, set algorithm and seats
                if (i == 0) {
                    algorithm = currentAlgorithm;
                    // Set shuffle for STV (by default shuffle_stv = true)
                    if (algorithm == "stv" or algorithm == "STV"){ 
                        shuffle_stv = true; 
                    }
                    
                    // Read number of seats
                    getline(file, line);
                    numSeats = stoi(line);
                } 
                // For subsequent files, verify consistency - if algorithm type and number of seats do not match, throw error messages
                else {
                    if (currentAlgorithm != algorithm) {
                        cout << "Error: Election type doesn't match first file." << endl;
                        attempts++;
                        continue;
                    }
                    getline(file, line);
                    if (stoi(line) != numSeats) {
                        cout << "Error: Number of seats doesn't match first file." << endl;
                        attempts++;
                        continue;
                    }
                }
                
                csvFileNames.push_back(filename);
                valid = true;
                file.close();
            }
        }
        // Exit after 3 failed attempts
        if (!valid) {
            cerr << "Too many failed attempts. Exiting." << endl;
            exit(1);
        }
    }

    // Get audit file name input 
    cout << "Enter audit file name: ";
    cin >> auditFileName;
}

// Getters implementation 
// return number of seats available
int UserInterface::getNumSeats() const {
    return numSeats;
}

// return Vector of validated CSV filenames
const vector<string>& UserInterface::getCsvFileNames() const {
    return csvFileNames;
}

// return Election algorithm type (STV, PV, MV)
const string& UserInterface::getAlgorithm() const {
    return algorithm;
}

// return filename for audit log output
const string& UserInterface::getAuditFileName() const {
    return auditFileName;
}

// return ballot shuffle is enabled (STV only)
bool UserInterface::getShuffleStv() const {
    return shuffle_stv;
}









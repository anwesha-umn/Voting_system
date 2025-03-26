#include "userinterface.h"
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <fstream>
#include <filesystem>
#include "stv.h"

using namespace std;

void UserInterface::getInfo() {
    string input;
    int attempts;
    bool valid;

    // Get CSV file name
    attempts = 0;
    valid = false;
    while (!valid && attempts < 3) {
        cout << "Enter CSV file name: ";
        cin >> csvFileName;



        // Check extension
        if (csvFileName.size() < 4 || csvFileName.substr(csvFileName.size() - 4) != ".csv") {
            cout << "Error: File must be a CSV file with .csv extension." << endl;
            attempts++;
            continue;
        }

        // Check if file exists
        ifstream file(csvFileName);
        if (!file) {
            cout << "Error: File does not exist or cannot be opened." << endl;
            attempts++;
        } else {
            valid = true;
            file.close();
        }
    }
    if (!valid) {
        cerr << "Too many failed attempts. Exiting." << endl;
        exit(1);
    }

    // Get number of seats
    attempts = 0;
    valid = false;
    while (!valid && attempts < 3) {
        cout << "Enter number of seats: ";
        cin >> input;
        numSeats = stoi(input);

        try {
            if (numSeats <= 0) {
                throw invalid_argument("Number must be positive");
            }
            valid = true;
        } catch (const invalid_argument& e) {
            cout << "Invalid input. Please enter a positive integer." << endl;
            attempts++;
        } catch (const out_of_range& e) {
            cout << "Input out of range. Please enter a smaller number." << endl;
            attempts++;
        }
    }
    if (!valid) {
        cerr << "Too many failed attempts. Exiting." << endl;
        exit(1);
    }

    // Get algorithm
    attempts = 0;
    valid = false;
    while (!valid && attempts < 3) {
        cout << "Select algorithm (STV or Plurality): ";
        cin >> input;

        string lowerInput = input;
        transform(lowerInput.begin(), lowerInput.end(), lowerInput.begin(), ::tolower);

        if (lowerInput == "stv" || lowerInput == "plurality") {
            algorithm = (lowerInput == "stv") ? "STV" : "Plurality";
            valid = true;
        } else {
            cout << "Invalid algorithm. Please enter STV or Plurality." << endl;
            attempts++;
        }
    }
    if (!valid) {
        cerr << "Too many failed attempts. Exiting." << endl;
        exit(1);
    }

    // Get shuffle STV option
    if (algorithm == "STV") {
        attempts = 0;
        valid = false;
        while (!valid && attempts < 3) {
            cout << "Disable ballot shuffle? (true/false): ";
            cin >> input;

            if (input.empty()) {
                cout << "Error: Input cannot be empty." << endl;
                attempts++;
                continue;
            }


            transform(input.begin(), input.end(), input.begin(), ::tolower);

            if (input == "true") {
                shuffle_stv = false; // disable shuffle
                valid = true;
            } else if (input == "false") {
                shuffle_stv = true; // enable shuffle
                valid = true;
            } else {
                cout << "Invalid input. Please enter true or false." << endl;
                attempts++;
            }
        }
        if (!valid) {
            cerr << "Too many failed attempts. Exiting." << endl;
            exit(1);
        }
    } else {
        shuffle_stv = false; // irrelevant for Plurality
    }

    // Get audit file name
    cout << "Enter audit file name: ";
    cin >> auditFileName;

}

// Getters implementation
int UserInterface::getNumSeats() const {
    return numSeats;
}

const string& UserInterface::getCsvFileName() const {
    return csvFileName;
}

const string& UserInterface::getAlgorithm() const {
    return algorithm;
}

const string& UserInterface::getAuditFileName() const {
    return auditFileName;
}

bool UserInterface::getShuffleStv() const {
    return shuffle_stv;
}

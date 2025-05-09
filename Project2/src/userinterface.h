/*
 * File: userinterface.h
 * Description: Defines the user interface for election configuration - handles CSV inputs, 
 *              algorithm selection, and audit logging setup
 * Author: Anwesha Samaddar
 */

#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#include <string>
#include <vector>

/**
 * @class UserInterface 
 * @brief represents user interface for election officials and testers to interact with 
 */

class UserInterface {
private:
    int numSeats;
    //std::string csvFileName;
    std::string algorithm;
    std::string auditFileName;
    bool shuffle_stv;
    std::vector<std::string> csvFileNames;  

public:
    // Constructor to initialize default values
    /**
     * @brief constructor 
     */
    UserInterface() : numSeats(3), 
                     algorithm("STV"), 
                     auditFileName("audit.txt"), 
                     shuffle_stv(true) {
        // Initialize with multiple default files
        csvFileNames = {
            "../../testing/stv_ballots.csv",
            "../../testing/stv_ballots2.csv",
        };
        
    }
    /**
     * @brief takes in election information from a CSV file and user input 
     */
    void getInfo();

    // Getters
    /**
     * @brief returns number of seats up for election 
     * @return int numSeats
     */
    int getNumSeats() const;
    /**
     * @brief returns names of input csv files
     * @return vector of strings where strings are names of csv files 
     */
    const std::vector<std::string>& getCsvFileNames() const;
    /**
     * @brief returns algorithm selection
     * @return string algorithm selectiom
     */
    const std::string& getAlgorithm() const;
    /**
     * @brief returns user-chosen audit file name
     * @return string audit file name 
     */
    const std::string& getAuditFileName() const;
    /**
     * @brief returns whether shuffle has been toggled on or not 
     * @return bool representing whether shuffle is on or off
     */
    bool getShuffleStv() const;

};

#endif // USERINTERFACE_H
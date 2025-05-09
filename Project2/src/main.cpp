/*
* File: main.cpp
 * Description: Main function to run the voting system.
 * Handles election setup, ballot processing, and result display.
 * Supports Multi-file csv inputs for Plurality, STV, and MV voting algorithms.
 * Author: Anwesha Samaddar, Annabelle Coler
 */

#include "userinterface.h"
#include "Election.h"
#include "plurality.h"
#include "stv.h"
#include <iostream>
#include <string>
#include <fstream>
#include "MVlogic.h"

using namespace std;

int main() {
    // Create the UserInterface object
    UserInterface ui;  
    
    try {
        // Get user input and setup election
        ui.getInfo();

        Election election(ui.getCsvFileNames(), ui.getAlgorithm(), ui.getNumSeats(), ui.getAuditFileName());
        
        // Load and parse ballots
        election.setBallots();
        
        //election.displayBallotAllocation();
          

        // Run appropriate election type
        if (ui.getAlgorithm() == "pv" or ui.getAlgorithm() == "PV") {
            // Execute plurality voting
            Plurality plurality(election.getBallots(), election.getCandidates(), election.getNumSeats());
            plurality.runElection(election.getBallots(), election.getCandidates(), election.getNumSeats());

            // Transfer winners and losers from Plurality to Election
            for (auto* winner : plurality.getWinners()) {
                election.addWinner(winner);
            }
            for (auto* loser : plurality.getLosers()) {
                election.addLoser(loser);
            }

            // displays plurality election stats
            election.displayResults();
        } else if (ui.getAlgorithm() == "stv" or ui.getAlgorithm() == "STV"){
            
            // Execute STV voting
            std::vector<Candidate*> stvWinners;
            std::vector<Candidate*> stvLosers;

            STV stv(election.getBallots(), election.getCandidates(), election.getNumSeats());

            // Set ballot shuffle option - by default set as true in userinterface.h
            stv.setShuffle(ui.getShuffleStv());
            stv.runElection(stvWinners, stvLosers);

            //cout << "UI : "<<ui.getAlgorithm() <<" elcection:"<<election.getAlgorithm()<<" shuffle: "<<ui.getShuffleStv();

            // Transfer results
            for (auto* winner : stvWinners) {
                election.addWinner(winner);
            }
            for (auto* loser : stvLosers) {
                election.addLoser(loser);
            }

            // Display STV election stats
            election.displayResults();
        } else if (ui.getAlgorithm() == "mv" or ui.getAlgorithm() == "MV"){

            // Execute MV voting
            MV mv(election.getBallots(), election.getCandidates(), election.getNumSeats());
            mv.runElection(election.getBallots(), election.getCandidates(), election.getNumSeats());

            // Transfer winners and losers from MVlogic to Election
            for (auto* winner : mv.getWinners()) {
                election.addWinner(winner);
            }
            for (auto* loser : mv.getLosers()) {
                election.addLoser(loser);
            }

            // displays MV election stats
            election.displayResults();
        } 

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1; // Exit with error code
    }

    //Pause before exiting
    cout << "Press Enter to exit...";
    cin.ignore();
    cin.get();
    return 0;

}

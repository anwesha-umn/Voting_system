//
// Created by ASUS on 3/25/2025.
//
#include "userinterface.h"
#include "Election.h"
#include "ballot.h"
#include "plurality.h"
#include <iostream>
#include <string>
#include <cctype>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "stvballot.h"
#include "stv.h"

using namespace std;

int main() {
    UserInterface ui;

    try {
        ui.getInfo();

        Election election(ui.getCsvFileName(), ui.getAlgorithm(), ui.getNumSeats(), ui.getAuditFileName());
        election.setBallots();
        election.displayBallotAllocation();

        if (ui.getAlgorithm() == "Plurality") {
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
        } else if (ui.getAlgorithm() == "STV"){
            std::vector<Candidate*> stvWinners;
            std::vector<Candidate*> stvLosers;

            STV stv(election.getBallots(), election.getCandidates(), election.getNumSeats());
            stv.setShuffle(ui.getShuffleStv());
            stv.runElection(stvWinners, stvLosers);

            // Transfer results
            for (auto* winner : stvWinners) {
                election.addWinner(winner);
            }
            for (auto* loser : stvLosers) {
                election.addLoser(loser);
            }

            // Display STV election stats
            election.displayResults();
        }


    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Pause before exiting
    std::cout << "Press Enter to exit...";
    std::cin.ignore();
    std::cin.get();

    return 0;
}
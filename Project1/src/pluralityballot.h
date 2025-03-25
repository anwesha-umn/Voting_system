//
// Created by ASUS on 3/16/2025.
//

#ifndef PLURALITYBALLOT_H
#define PLURALITYBALLOT_H

#include "ballot.h"

class PluralityBallot : public Ballot {
private:
    int preference; // Single candidate preference

public:
    // Constructor
    PluralityBallot(std::vector<int> votes, int ballotID);

    // Getter
    int getPreference() const;
};

#endif // PLURALITYBALLOT_H
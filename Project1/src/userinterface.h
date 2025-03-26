#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#include <string>

class UserInterface {
private:
    int numSeats;
    std::string csvFileName;
    std::string algorithm;
    std::string auditFileName;
    bool shuffle_stv;

public:
    // Constructor to initialize default values
    UserInterface() : numSeats(3), csvFileName("../testing/stv_ballots.csv"),
                      algorithm("STV"), auditFileName("audit.txt"), shuffle_stv(true) {}

    void getInfo();

    // Getters
    int getNumSeats() const;
    const std::string& getCsvFileName() const;
    const std::string& getAlgorithm() const;
    const std::string& getAuditFileName() const;
    bool getShuffleStv() const;
};

#endif // USERINTERFACE_H
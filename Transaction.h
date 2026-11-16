#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>

class Transaction {
private:
    std::string transactionID;
    std::string type;
    double amount;
    std::string dateTime;
    std::string fromWallet;
    std::string toWallet;
    std::string status;

public:
    Transaction(const std::string& id, const std::string& type, double amount,
                const std::string& dateTime, const std::string& from,
                const std::string& to, const std::string& status);

    std::string toFileRecord() const;
    void display() const;
    bool involves(const std::string& walletID) const;
};

#endif

#ifndef WALLET_H
#define WALLET_H

#include "User.h"
#include <string>

class Wallet : public User {
private:
    double balance;
    std::string pin;

public:
    Wallet(const std::string& name, const std::string& phone,
           const std::string& id, const std::string& pin,
           double balance = 0.0);
    virtual ~Wallet() = default;

    virtual std::string getWalletType() const;
    virtual double calculateTransferFee(double amount) const;

    bool verifyPIN(const std::string& enteredPIN) const;
    bool addMoney(double amount);
    bool withdrawMoney(double amount);
    double getBalance() const;
    bool changePIN(const std::string& oldPIN, const std::string& newPIN);
    void displayProfile() const override;

    std::string saveRecord() const;
};

class PremiumWallet : public Wallet {
public:
    PremiumWallet(const std::string& name, const std::string& phone,
                  const std::string& id, const std::string& pin,
                  double balance = 0.0);

    std::string getWalletType() const override;
    double calculateTransferFee(double amount) const override;
    void displayProfile() const override;
};

#endif

#include "Wallet.h"
#include <iostream>
#include <sstream>
#include <iomanip>

Wallet::Wallet(const std::string& userName, const std::string& phone,
               const std::string& id, const std::string& userPIN, double amount)
    : User(userName, phone, id), balance(amount), pin(userPIN) {}

std::string Wallet::getWalletType() const { return "Regular"; }

double Wallet::calculateTransferFee(double amount) const {
    return amount * 0.005; // 0.5% fee for regular wallets
}

bool Wallet::verifyPIN(const std::string& enteredPIN) const {
    return pin == enteredPIN;
}

bool Wallet::addMoney(double amount) {
    if (amount <= 0) return false;
    balance += amount;
    return true;
}

bool Wallet::withdrawMoney(double amount) {
    if (amount <= 0 || amount > balance) return false;
    balance -= amount;
    return true;
}

double Wallet::getBalance() const { return balance; }

bool Wallet::changePIN(const std::string& oldPIN, const std::string& newPIN) {
    if (pin != oldPIN || newPIN.length() != 4) return false;
    for (char ch : newPIN) {
        if (ch < '0' || ch > '9') return false;
    }
    pin = newPIN;
    return true;
}

void Wallet::displayProfile() const {
    User::displayProfile();
    std::cout << "Wallet type: " << getWalletType() << '\n';
    std::cout << "Balance    : Rs. " << std::fixed << std::setprecision(2) << balance << '\n';
}

std::string Wallet::saveRecord() const {
    std::ostringstream out;
    out << name << '|' << phoneNumber << '|' << walletID << '|'
        << pin << '|' << std::setprecision(15) << balance << "|0";
    return out.str();
}

PremiumWallet::PremiumWallet(const std::string& userName, const std::string& phone,
                             const std::string& id, const std::string& userPIN, double amount)
    : Wallet(userName, phone, id, userPIN, amount) {}

std::string PremiumWallet::getWalletType() const { return "Premium"; }

double PremiumWallet::calculateTransferFee(double) const { return 0.0; }

void PremiumWallet::displayProfile() const {
    Wallet::displayProfile();
    std::cout << "Benefits   : Zero transfer fee\n";
}

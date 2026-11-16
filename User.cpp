#include "User.h"
#include <iostream>

User::User(const std::string& userName, const std::string& phone, const std::string& id)
    : name(userName), phoneNumber(phone), walletID(id) {}

std::string User::getName() const { return name; }
std::string User::getPhoneNumber() const { return phoneNumber; }
std::string User::getWalletID() const { return walletID; }

void User::setPhoneNumber(const std::string& phone) {
    phoneNumber = phone;
}

void User::displayProfile() const {
    std::cout << "\n----- USER PROFILE -----\n";
    std::cout << "Name       : " << name << '\n';
    std::cout << "Phone      : " << phoneNumber << '\n';
    std::cout << "Wallet ID  : " << walletID << '\n';
}

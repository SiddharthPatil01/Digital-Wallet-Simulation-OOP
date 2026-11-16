#include "Transaction.h"
#include <iostream>
#include <iomanip>
#include <sstream>

Transaction::Transaction(const std::string& id, const std::string& transactionType,
                         double transactionAmount, const std::string& when,
                         const std::string& from, const std::string& to,
                         const std::string& transactionStatus)
    : transactionID(id), type(transactionType), amount(transactionAmount),
      dateTime(when), fromWallet(from), toWallet(to), status(transactionStatus) {}

std::string Transaction::toFileRecord() const {
    std::ostringstream out;
    out << transactionID << '|' << type << '|' << std::setprecision(15) << amount
        << '|' << dateTime << '|' << fromWallet << '|' << toWallet << '|' << status;
    return out.str();
}

void Transaction::display() const {
    std::cout << "\nTransaction ID : " << transactionID
              << "\nType           : " << type
              << "\nAmount         : Rs. " << std::fixed << std::setprecision(2) << amount
              << "\nDate & Time    : " << dateTime
              << "\nFrom Wallet    : " << (fromWallet.empty() ? "-" : fromWallet)
              << "\nTo Wallet      : " << (toWallet.empty() ? "-" : toWallet)
              << "\nStatus         : " << status << "\n";
}

bool Transaction::involves(const std::string& id) const {
    return fromWallet == id || toWallet == id;
}

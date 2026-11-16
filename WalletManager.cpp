#include "WalletManager.h"
#include "Payment.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <filesystem>
#include <limits>
#include <cctype>

namespace {
std::string readLine(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

double readAmount(const std::string& prompt) {
    double amount;
    while (true) {
        std::cout << prompt;
        if (std::cin >> amount && amount > 0) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return amount;
        }
        std::cout << "Please enter a valid amount greater than zero.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

bool validPIN(const std::string& pin) {
    if (pin.size() != 4) return false;
    for (char ch : pin) if (!std::isdigit(static_cast<unsigned char>(ch))) return false;
    return true;
}
}

WalletManager::WalletManager()
    : usersFile("data/users.txt"), transactionsFile("data/transactions.txt"),
      nextWalletNumber(1001), nextTransactionNumber(1) {
    std::filesystem::create_directories("data");
    loadWallets();

    // Continue wallet numbering after the highest saved wallet ID.
    for (const auto& wallet : wallets) {
        try {
            int number = std::stoi(wallet->getWalletID());
            if (number >= nextWalletNumber) nextWalletNumber = number + 1;
        } catch (...) {}
    }

    // Continue transaction numbering after the latest saved transaction ID.
    std::ifstream in(transactionsFile);
    std::string line;
    while (std::getline(in, line)) {
        std::stringstream ss(line);
        std::string id;
        if (std::getline(ss, id, '|') && id.rfind("TX", 0) == 0) {
            try {
                int number = std::stoi(id.substr(2));
                if (number >= nextTransactionNumber) nextTransactionNumber = number + 1;
            } catch (...) {}
        }
    }
}

void WalletManager::loadWallets() {
    std::ifstream file(usersFile);
    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, phone, id, pin, balanceText, premiumText;
        if (!std::getline(ss, name, '|') || !std::getline(ss, phone, '|') ||
            !std::getline(ss, id, '|') || !std::getline(ss, pin, '|') ||
            !std::getline(ss, balanceText, '|') || !std::getline(ss, premiumText, '|'))
            continue;
        try {
            double balance = std::stod(balanceText);
            if (premiumText == "1")
                wallets.push_back(std::make_unique<PremiumWallet>(name, phone, id, pin, balance));
            else
                wallets.push_back(std::make_unique<Wallet>(name, phone, id, pin, balance));
        } catch (...) {
            std::cout << "Skipping an invalid saved account record.\n";
        }
    }
}

void WalletManager::saveWallets() const {
    std::ofstream file(usersFile);
    for (const auto& wallet : wallets) {
        // Serialization is performed in the output pass below.
    }
    file.close();

    std::ofstream output(usersFile, std::ios::trunc);
    for (const auto& wallet : wallets) {
        std::string record = wallet->saveRecord();
        std::size_t lastSeparator = record.rfind('|');
        record = record.substr(0, lastSeparator + 1)
               + (wallet->getWalletType() == "Premium" ? "1" : "0");
        output << record << '\n';
    }
}
Wallet* WalletManager::findWallet(const std::string& id) const {
    for (const auto& wallet : wallets)
        if (wallet->getWalletID() == id) return wallet.get();
    return nullptr;
}

std::string WalletManager::makeDateTime() const {
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    char buffer[32];
    std::strftime(buffer, sizeof(buffer), "%d-%m-%Y %H:%M:%S", local);
    return buffer;
}

std::string WalletManager::makeTransactionID() {
    std::ostringstream out;
    out << "TX" << std::setw(5) << std::setfill('0') << nextTransactionNumber++;
    return out.str();
}

void WalletManager::recordTransaction(const std::string& type, double amount,
                                      const std::string& from, const std::string& to,
                                      const std::string& status) {
    Transaction transaction(makeTransactionID(), type, amount, makeDateTime(), from, to, status);
    std::ofstream file(transactionsFile, std::ios::app);
    if (file) file << transaction.toFileRecord() << '\n';
}

void WalletManager::createAccount() {
    std::cout << "\n===== CREATE WALLET =====\n";
    std::string name = readLine("Enter full name: ");
    std::string phone = readLine("Enter phone number: ");
    if (name.empty() || phone.empty() || name.find('|') != std::string::npos ||
        phone.find('|') != std::string::npos) {
        std::cout << "Name and phone are required; do not use the | character.\n";
        return;
    }

    std::string pin = readLine("Create 4-digit PIN: ");
    if (!validPIN(pin)) {
        std::cout << "PIN must contain exactly 4 digits.\n";
        return;
    }

    std::string choice = readLine("Choose wallet type (1-Regular, 2-Premium): ");
    bool premium = choice == "2";
    std::string id = std::to_string(nextWalletNumber++);
    if (premium)
        wallets.push_back(std::make_unique<PremiumWallet>(name, phone, id, pin));
    else
        wallets.push_back(std::make_unique<Wallet>(name, phone, id, pin));

    saveWallets();
    std::cout << "\nAccount created successfully! Your Wallet ID is: " << id << "\n";
}

Wallet* WalletManager::authenticate() {
    std::string id = readLine("Enter Wallet ID: ");
    Wallet* wallet = findWallet(id);
    if (!wallet) {
        std::cout << "Wallet not found.\n";
        return nullptr;
    }

    for (int attempt = 1; attempt <= 3; ++attempt) {
        std::string pin = readLine("Enter 4-digit PIN: ");
        if (wallet->verifyPIN(pin)) return wallet;
        std::cout << "Incorrect PIN. Attempts left: " << (3 - attempt) << '\n';
    }
    std::cout << "Login failed.\n";
    return nullptr;
}

void WalletManager::login() {
    Wallet* wallet = authenticate();
    if (!wallet) return;

    int choice = 0;
    do {
        std::cout << "\n========== DIGITAL WALLET =========="
                  << "\nWelcome, " << wallet->getName()
                  << "\n1. Add Money"
                  << "\n2. Withdraw Money"
                  << "\n3. Transfer Money"
                  << "\n4. Check Balance"
                  << "\n5. Transaction History"
                  << "\n6. View Profile"
                  << "\n7. Change PIN"
                  << "\n0. Logout"
                  << "\nEnter choice: ";
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: addMoney(*wallet); break;
            case 2: withdrawMoney(*wallet); break;
            case 3: transferMoney(*wallet); break;
            case 4: showBalance(*wallet); break;
            case 5: showHistory(*wallet); break;
            case 6: wallet->displayProfile(); break;
            case 7: changePIN(*wallet); break;
            case 0: std::cout << "Logged out successfully.\n"; break;
            default: std::cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);
}

void WalletManager::addMoney(Wallet& wallet) {
    double amount = readAmount("Enter amount to add: Rs. ");
    AddMoneyPayment payment;
    bool success = payment.process(wallet, amount);
    if (success) {
        recordTransaction(payment.getType(), amount, "", wallet.getWalletID(), "Success");
        saveWallets();
        std::cout << "Money added successfully.\n";
    } else {
        recordTransaction(payment.getType(), amount, "", wallet.getWalletID(), "Failed");
        std::cout << "Could not add money.\n";
    }
}

void WalletManager::withdrawMoney(Wallet& wallet) {
    double amount = readAmount("Enter amount to withdraw: Rs. ");
    WithdrawPayment payment;
    bool success = payment.process(wallet, amount);
    recordTransaction(payment.getType(), amount, wallet.getWalletID(), "", success ? "Success" : "Failed");
    if (success) {
        saveWallets();
        std::cout << "Please collect your money.\n";
    } else {
        std::cout << "Insufficient balance.\n";
    }
}

void WalletManager::transferMoney(Wallet& wallet) {
    std::string receiverID = readLine("Enter receiver Wallet ID: ");
    Wallet* receiver = findWallet(receiverID);
    if (!receiver || receiver == &wallet) {
        std::cout << "Invalid receiver wallet.\n";
        return;
    }

    double amount = readAmount("Enter amount to transfer: Rs. ");
    double fee = wallet.calculateTransferFee(amount);
    TransferPayment payment;
    bool success = payment.process(wallet, amount, receiver);
    recordTransaction(payment.getType(), amount, wallet.getWalletID(), receiverID,
                      success ? "Success" : "Failed");

    if (success) {
        saveWallets();
        std::cout << "Transfer successful!\nTransfer fee: Rs. "
                  << std::fixed << std::setprecision(2) << fee << '\n';
    } else {
        std::cout << "Transfer failed. Check your balance.\n";
    }
}

void WalletManager::showBalance(const Wallet& wallet) const {
    std::cout << "Available balance: Rs. " << std::fixed << std::setprecision(2)
              << wallet.getBalance() << '\n';
}

void WalletManager::showHistory(const Wallet& wallet) const {
    std::ifstream file(transactionsFile);
    std::string line;
    bool found = false;
    std::cout << "\n===== TRANSACTION HISTORY =====\n";
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string id, type, amountText, date, from, to, status;
        if (!std::getline(ss, id, '|') || !std::getline(ss, type, '|') ||
            !std::getline(ss, amountText, '|') || !std::getline(ss, date, '|') ||
            !std::getline(ss, from, '|') || !std::getline(ss, to, '|') ||
            !std::getline(ss, status, '|')) continue;

        if (from == wallet.getWalletID() || to == wallet.getWalletID()) {
            try {
                Transaction transaction(id, type, std::stod(amountText), date, from, to, status);
                transaction.display();
                found = true;
            } catch (...) {}
        }
    }
    if (!found) std::cout << "No transactions found.\n";
}

void WalletManager::changePIN(Wallet& wallet) {
    std::string oldPIN = readLine("Enter current PIN: ");
    std::string newPIN = readLine("Enter new 4-digit PIN: ");
    if (wallet.changePIN(oldPIN, newPIN)) {
        saveWallets();
        std::cout << "PIN changed successfully.\n";
    } else {
        std::cout << "PIN change failed. Check your current PIN and new PIN format.\n";
    }
}

#ifndef WALLET_MANAGER_H
#define WALLET_MANAGER_H

#include "Wallet.h"
#include "Transaction.h"
#include <memory>
#include <vector>
#include <string>

class WalletManager {
private:
    std::vector<std::unique_ptr<Wallet>> wallets;
    std::string usersFile;
    std::string transactionsFile;
    int nextWalletNumber;
    int nextTransactionNumber;

    void loadWallets();
    void saveWallets() const;
    void recordTransaction(const std::string& type, double amount,
                           const std::string& from, const std::string& to,
                           const std::string& status);
    std::string makeDateTime() const;
    std::string makeTransactionID();

public:
    WalletManager();
    void createAccount();
    void login();
    Wallet* findWallet(const std::string& id) const;
    Wallet* authenticate();
    void addMoney(Wallet& wallet);
    void withdrawMoney(Wallet& wallet);
    void transferMoney(Wallet& wallet);
    void showBalance(const Wallet& wallet) const;
    void showHistory(const Wallet& wallet) const;
    void changePIN(Wallet& wallet);
};

#endif

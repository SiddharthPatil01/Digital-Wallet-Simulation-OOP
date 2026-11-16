#ifndef PAYMENT_H
#define PAYMENT_H

#include "Wallet.h"
#include <string>

class Payment {
public:
    virtual ~Payment() = default;
    virtual bool process(Wallet& wallet, double amount, Wallet* receiver = nullptr) = 0;
    virtual std::string getType() const = 0;
};

class AddMoneyPayment : public Payment {
public:
    bool process(Wallet& wallet, double amount, Wallet* receiver = nullptr) override;
    std::string getType() const override;
};

class WithdrawPayment : public Payment {
public:
    bool process(Wallet& wallet, double amount, Wallet* receiver = nullptr) override;
    std::string getType() const override;
};

class TransferPayment : public Payment {
public:
    bool process(Wallet& wallet, double amount, Wallet* receiver = nullptr) override;
    std::string getType() const override;
};

#endif

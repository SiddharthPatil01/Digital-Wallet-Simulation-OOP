#include "Payment.h"

bool AddMoneyPayment::process(Wallet& wallet, double amount, Wallet*) {
    return wallet.addMoney(amount);
}
std::string AddMoneyPayment::getType() const { return "Add Money"; }

bool WithdrawPayment::process(Wallet& wallet, double amount, Wallet*) {
    return wallet.withdrawMoney(amount);
}
std::string WithdrawPayment::getType() const { return "Withdraw"; }

bool TransferPayment::process(Wallet& sender, double amount, Wallet* receiver) {
    if (receiver == nullptr || receiver == &sender || amount <= 0) return false;

    double fee = sender.calculateTransferFee(amount);
    if (sender.getBalance() < amount + fee) return false;

    if (!sender.withdrawMoney(amount + fee)) return false;
    if (!receiver->addMoney(amount)) {
        sender.addMoney(amount + fee); // restore sender balance if credit fails
        return false;
    }
    return true;
}
std::string TransferPayment::getType() const { return "Transfer"; }

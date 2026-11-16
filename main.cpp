#include "WalletManager.h"
#include <iostream>
#include <limits>

int main() {
    WalletManager manager;
    int choice = -1;

    std::cout << "====================================\n";
    std::cout << "       DIGITAL WALLET SYSTEM        \n";
    std::cout << "====================================\n";

    do {
        std::cout << "\n========== MAIN MENU ==========\n"
                  << "1. Create New Wallet\n"
                  << "2. Login to Wallet\n"
                  << "0. Exit\n"
                  << "Enter your choice: ";

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Please enter a number.\n";
            continue;
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (choice) {
            case 1: manager.createAccount(); break;
            case 2: manager.login(); break;
            case 0: std::cout << "Thank you for using Digital Wallet!\n"; break;
            default: std::cout << "Invalid choice. Try again.\n";
        }
    } while (choice != 0);

    return 0;
}

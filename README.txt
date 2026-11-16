DIGITAL WALLET SIMULATION - C++ OOP MINI PROJECT

FILES
-----
main.cpp             Main menu and program entry point
User.h / User.cpp    Base User class
Wallet.h / Wallet.cpp Wallet and PremiumWallet classes
Transaction.h/.cpp  Transaction record class
Payment.h / Payment.cpp Abstract Payment class and derived payment types
WalletManager.h/.cpp Account, login, menus, file handling

HOW TO COMPILE (VS CODE TERMINAL / POWERSHELL)
----------------------------------------------
1. Open the DigitalWallet folder in VS Code.
2. Open Terminal > New Terminal.
3. Run:

   g++ -std=c++17 main.cpp User.cpp Wallet.cpp Transaction.cpp Payment.cpp WalletManager.cpp -o wallet.exe

4. Run:

   .\wallet.exe

The program automatically creates a "data" folder and saves account and
transaction records in text files inside it. Keep the data folder with the
program if you want to retain your saved demo accounts.

OOP CONCEPTS
------------
Encapsulation: Wallet balance and PIN are private.
Inheritance: Wallet inherits User; PremiumWallet inherits Wallet.
Polymorphism: virtual wallet type, transfer fee and display functions.
Abstraction: Payment is an abstract base class with a pure virtual process().

DEMO
----
Create two wallets, remember each Wallet ID and PIN, then log in and test:
- Add money
- Withdraw money
- Transfer money between wallets
- Check balance
- View transaction history
- Change PIN



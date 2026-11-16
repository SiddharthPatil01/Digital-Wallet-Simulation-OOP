#ifndef USER_H
#define USER_H

#include <string>

class User {
protected:
    std::string name;
    std::string phoneNumber;
    std::string walletID;

public:
    User(const std::string& name, const std::string& phone, const std::string& id);
    virtual ~User() = default;

    std::string getName() const;
    std::string getPhoneNumber() const;
    std::string getWalletID() const;
    void setPhoneNumber(const std::string& phone);
    virtual void displayProfile() const;
};

#endif

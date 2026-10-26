#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>

class Account
{
private:
    int accountNumber;
    std::string customerName;
    std::string passwordHash;
    long long balance;
    bool active;

public:
    Account(int accountNumber,
            const std::string& customerName,
            const std::string& passwordHash);

    int getAccountNumber() const;

    std::string getCustomerName() const;

    long long getBalance() const;

    bool isActive() const;

    bool verifyPassword(const std::string& password) const;

    bool deposit(long long amount);

    bool withdraw(long long amount);

    bool changePassword(const std::string& newPasswordHash);

    bool closeAccount();
};

#endif
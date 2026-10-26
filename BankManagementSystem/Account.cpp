#include "Account.h"

#include <limits>
#include <utility>

// Constructor
Account::Account(
    int accountNumber,
    const std::string& customerName,
    const std::string& passwordHash
)
    : accountNumber(accountNumber),
      customerName(customerName),
      passwordHash(passwordHash),
      balance(0),
      active(true)
{
}

// Get account number
int Account::getAccountNumber() const
{
    return accountNumber;
}

// Get customer name
std::string Account::getCustomerName() const
{
    return customerName;
}

// Get account balance in paise
long long Account::getBalance() const
{
    return balance;
}

// Check whether account is active
bool Account::isActive() const
{
    return active;
}

// Verify password against stored password data
bool Account::verifyPassword(const std::string& password) const
{
    return active && passwordHash == password;
}

// Deposit money
bool Account::deposit(long long amount)
{
    if (!active || amount <= 0)
    {
        return false;
    }

    // Prevent integer overflow
    if (balance > std::numeric_limits<long long>::max() - amount)
    {
        return false;
    }

    balance += amount;

    return true;
}

// Withdraw money
bool Account::withdraw(long long amount)
{
    if (!active || amount <= 0)
    {
        return false;
    }

    if (amount > balance)
    {
        return false;
    }

    balance -= amount;

    return true;
}

// Change password data
bool Account::changePassword(const std::string& newPasswordHash)
{
    if (!active || newPasswordHash.empty())
    {
        return false;
    }

    passwordHash = newPasswordHash;

    return true;
}

// Close account
bool Account::closeAccount()
{
    if (!active || balance != 0)
    {
        return false;
    }

    active = false;

    return true;
}
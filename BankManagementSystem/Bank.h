#ifndef BANK_H
#define BANK_H

#include "Account.h"
#include "Transaction.h"

#include <cstddef>
#include <string>
#include <vector>

class Bank
{
private:
    // Stores all accounts managed by the bank.
    std::vector<Account> accounts;

    // Central transaction ledger.
    std::vector<Transaction> transactions;

    // Automatically generated account numbers.
    int nextAccountNumber;

    // Used to generate unique transaction IDs.
    unsigned long long nextTransactionNumber;

    // Finds an account by its account number.
    Account* findAccount(int accountNumber);
    const Account* findAccount(int accountNumber) const;

    // Records a transaction in the central ledger.
    void recordTransaction(
        TransactionType type,
        int sourceAccountNumber,
        int destinationAccountNumber,
        long long amount,
        TransactionStatus status
    );

public:
    // Constructor.
    Bank();

    // Account management.
    int createAccount(
        const std::string& customerName,
        const std::string& password
    );

    bool authenticate(
        int accountNumber,
        const std::string& password
    ) const;

    bool closeAccount(int accountNumber);

    // Banking operations.
    bool deposit(int accountNumber, long long amount);

    bool withdraw(int accountNumber, long long amount);

    bool transfer(
        int sourceAccountNumber,
        int destinationAccountNumber,
        long long amount
    );

    bool checkBalance(
        int accountNumber,
        long long& balance
    ) const;

    // Password management.
    bool changePassword(
        int accountNumber,
        const std::string& oldPassword,
        const std::string& newPassword
    );

    // Returns the most recent transactions involving an account.
    std::vector<Transaction> getMiniStatement(
        int accountNumber,
        std::size_t limit = 5
    ) const;
};

#endif
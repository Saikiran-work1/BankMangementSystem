#include "Bank.h"

#include <limits>
#include <string>

Bank::Bank()
    : nextAccountNumber(100001),
      nextTransactionNumber(1001)
{
}

Account* Bank::findAccount(int accountNumber)
{
    for (Account& account : accounts)
    {
        if (account.getAccountNumber() == accountNumber)
        {
            return &account;
        }
    }

    return nullptr;
}

const Account* Bank::findAccount(int accountNumber) const
{
    for (const Account& account : accounts)
    {
        if (account.getAccountNumber() == accountNumber)
        {
            return &account;
        }
    }

    return nullptr;
}

int Bank::createAccount(
    const std::string& customerName,
    const std::string& password
)
{
    if (customerName.empty() || password.empty())
    {
        return -1;
    }

    int accountNumber = nextAccountNumber;

    Account newAccount(
        accountNumber,
        customerName,
        password
    );

    accounts.push_back(newAccount);

    ++nextAccountNumber;

    return accountNumber;
}

bool Bank::authenticate(
    int accountNumber,
    const std::string& password
) const
{
    const Account* account = findAccount(accountNumber);

    if (account == nullptr)
    {
        return false;
    }

    return account->verifyPassword(password);
}


void Bank::recordTransaction(
    TransactionType type,
    int sourceAccountNumber,
    int destinationAccountNumber,
    long long amount,
    TransactionStatus status
)
{
    std::string transactionId =
        "TXN" + std::to_string(nextTransactionNumber);

    ++nextTransactionNumber;

    transactions.emplace_back(
        transactionId,
        type,
        sourceAccountNumber,
        destinationAccountNumber,
        amount,
        status
    );
}

bool Bank::deposit(int accountNumber, long long amount)
{
    Account* account = findAccount(accountNumber);

    if (account == nullptr)
    {
        return false;
    }

    if (!account->deposit(amount))
    {
        return false;
    }

    recordTransaction(
        TransactionType::DEPOSIT,
        0,
        accountNumber,
        amount,
        TransactionStatus::SUCCESS
    );

    return true;
}

bool Bank::withdraw(int accountNumber, long long amount)
{
    Account* account = findAccount(accountNumber);

    if (account == nullptr)
    {
        return false;
    }

    if (!account->withdraw(amount))
    {
        return false;
    }

    recordTransaction(
        TransactionType::WITHDRAWAL,
        accountNumber,
        0,
        amount,
        TransactionStatus::SUCCESS
    );

    return true;
}

bool Bank::checkBalance(
    int accountNumber,
    long long& balance
) const
{
    const Account* account = findAccount(accountNumber);

    if (account == nullptr)
    {
        return false;
    }

    if (!account->isActive())
    {
        return false;
    }

    balance = account->getBalance();

    return true;
}

bool Bank::transfer(
    int sourceAccountNumber,
    int destinationAccountNumber,
    long long amount
)
{
    // Prevent transferring money to the same account.
    if (sourceAccountNumber == destinationAccountNumber)
    {
        return false;
    }

    // Find both accounts.
    Account* source = findAccount(sourceAccountNumber);
    Account* destination = findAccount(destinationAccountNumber);

    // Both accounts must exist and be active.
    if (source == nullptr || destination == nullptr)
    {
        return false;
    }

    if (!source->isActive() || !destination->isActive())
    {
        return false;
    }

    // The transfer amount must be positive.
    if (amount <= 0)
    {
        return false;
    }

    // Ensure the recipient's balance will not overflow.
    if (destination->getBalance() >
        std::numeric_limits<long long>::max() - amount)
    {
        return false;
    }

    // Deduct money from the sender.
    if (!source->withdraw(amount))
    {
        return false;
    }

    // Credit the recipient.
    if (!destination->deposit(amount))
    {
        // Roll back the deduction if crediting fails.
        source->deposit(amount);
        return false;
    }

    // Record one transaction for the entire transfer.
    recordTransaction(
        TransactionType::TRANSFER,
        sourceAccountNumber,
        destinationAccountNumber,
        amount,
        TransactionStatus::SUCCESS
    );

    return true;
}


std::vector<Transaction> Bank::getMiniStatement(
    int accountNumber,
    std::size_t limit
) const
{
    std::vector<Transaction> statement;

    // Return an empty statement if the account doesn't exist.
    const Account* account = findAccount(accountNumber);

    if (account == nullptr || !account->isActive())
    {
        return statement;
    }

    // Search the central transaction ledger.
    for (const Transaction& transaction : transactions)
    {
        if (transaction.getSourceAccountNumber() == accountNumber ||
            transaction.getDestinationAccountNumber() == accountNumber)
        {
            statement.push_back(transaction);
        }
    }

    // Keep only the most recent 'limit' transactions.
    if (statement.size() > limit)
    {
        statement.erase(
            statement.begin(),
            statement.end() - limit
        );
    }

    return statement;
}


bool Bank::changePassword(
    int accountNumber,
    const std::string& oldPassword,
    const std::string& newPassword
)
{
    Account* account = findAccount(accountNumber);

    if (account == nullptr)
    {
        return false;
    }

    if (!account->verifyPassword(oldPassword))
    {
        return false;
    }

    if (newPassword.empty() || newPassword == oldPassword)
    {
        return false;
    }

    return account->changePassword(newPassword);
}

bool Bank::closeAccount(int accountNumber)
{
    Account* account = findAccount(accountNumber);

    if (account == nullptr)
    {
        return false;
    }

    return account->closeAccount();
}
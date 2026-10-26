#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <ctime>

enum class TransactionType
{
    DEPOSIT,
    WITHDRAWAL,
    TRANSFER
};

enum class TransactionStatus
{
    SUCCESS,
    FAILED
};

class Transaction
{
private:
    std::string transactionId;
    TransactionType transactionType;

    int sourceAccountNumber;
    int destinationAccountNumber;

    long long amount;  // Amount in paise

    std::time_t timestamp;

    TransactionStatus status;

public:
    Transaction(
        const std::string& transactionId,
        TransactionType transactionType,
        int sourceAccountNumber,
        int destinationAccountNumber,
        long long amount,
        TransactionStatus status
    );

    std::string getTransactionId() const;

    TransactionType getTransactionType() const;

    int getSourceAccountNumber() const;

    int getDestinationAccountNumber() const;

    long long getAmount() const;

    std::time_t getTimestamp() const;

    TransactionStatus getStatus() const;

    std::string getTransactionTypeName() const;

    std::string getStatusName() const;

    std::string getFormattedTimestamp() const;

    std::string getTransactionDetails() const;
};

#endif
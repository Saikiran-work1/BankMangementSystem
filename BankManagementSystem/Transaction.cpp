#include "Transaction.h"

#include <ctime>
#include <iomanip>
#include <sstream>
#include <utility>

// Constructor
Transaction::Transaction(
    const std::string& transactionId,
    TransactionType transactionType,
    int sourceAccountNumber,
    int destinationAccountNumber,
    long long amount,
    TransactionStatus status
)
    : transactionId(transactionId),
      transactionType(transactionType),
      sourceAccountNumber(sourceAccountNumber),
      destinationAccountNumber(destinationAccountNumber),
      amount(amount),
      timestamp(std::time(nullptr)),
      status(status)
{
}

// Get transaction ID
std::string Transaction::getTransactionId() const
{
    return transactionId;
}

// Get transaction type
TransactionType Transaction::getTransactionType() const
{
    return transactionType;
}

// Get source account number
int Transaction::getSourceAccountNumber() const
{
    return sourceAccountNumber;
}

// Get destination account number
int Transaction::getDestinationAccountNumber() const
{
    return destinationAccountNumber;
}

// Get transaction amount in paise
long long Transaction::getAmount() const
{
    return amount;
}

// Get transaction timestamp
std::time_t Transaction::getTimestamp() const
{
    return timestamp;
}

// Get transaction status
TransactionStatus Transaction::getStatus() const
{
    return status;
}

// Convert transaction type to readable text
std::string Transaction::getTransactionTypeName() const
{
    switch (transactionType)
    {
        case TransactionType::DEPOSIT:
            return "Deposit";

        case TransactionType::WITHDRAWAL:
            return "Withdrawal";

        case TransactionType::TRANSFER:
            return "Transfer";
    }

    return "Unknown";
}

// Convert transaction status to readable text
std::string Transaction::getStatusName() const
{
    switch (status)
    {
        case TransactionStatus::SUCCESS:
            return "Successful";

        case TransactionStatus::FAILED:
            return "Failed";
    }

    return "Unknown";
}

// Format timestamp for display
std::string Transaction::getFormattedTimestamp() const
{
    std::tm* timeInfo = std::localtime(&timestamp);

    if (timeInfo == nullptr)
    {
        return "Unknown";
    }

    std::ostringstream output;

    output << std::put_time(
        timeInfo,
        "%d-%m-%Y %H:%M:%S"
    );

    return output.str();
}

// Generate a readable transaction description
std::string Transaction::getTransactionDetails() const
{
    std::ostringstream output;

    output << "Transaction ID: " << transactionId << '\n';

    output << "Type: "
           << getTransactionTypeName() << '\n';

    output << "Source Account: "
           << sourceAccountNumber << '\n';

    output << "Destination Account: "
           << destinationAccountNumber << '\n';

    output << "Amount: Rs. "
           << std::fixed << std::setprecision(2)
           << static_cast<double>(amount) / 100.0 << '\n';

    output << "Date and Time: "
           << getFormattedTimestamp() << '\n';

    output << "Status: "
           << getStatusName();

    return output.str();
}
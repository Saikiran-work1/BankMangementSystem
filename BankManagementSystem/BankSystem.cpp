#include "BankSystem.h"

#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

namespace
{
    // Read an integer without accepting extra characters.
    bool readInteger(const std::string& prompt, int& value)
    {
        std::string input;

        std::cout << prompt;

        if (!std::getline(std::cin, input))
        {
            return false;
        }

        std::istringstream stream(input);

        if (!(stream >> value))
        {
            return false;
        }

        stream >> std::ws;
        return stream.eof();
    }

    // Read a positive amount in rupees and convert it to paise.
    bool readAmount(const std::string& prompt, long long& amount)
    {
        std::string input;

        std::cout << prompt;

        if (!std::getline(std::cin, input))
        {
            return false;
        }

        std::istringstream stream(input);
        long double rupees;

        if (!(stream >> rupees))
        {
            return false;
        }

        stream >> std::ws;

        if (!stream.eof() ||
            !std::isfinite(rupees) ||
            rupees <= 0 ||
            rupees >
                static_cast<long double>(
                    std::numeric_limits<long long>::max()
                ) / 100.0L)
        {
            return false;
        }

        long double paise = std::round(rupees * 100.0L);

        if (paise < 1 ||
            paise >
                static_cast<long double>(
                    std::numeric_limits<long long>::max()
                ))
        {
            return false;
        }

        amount = static_cast<long long>(paise);
        return true;
    }

    // Display a balance stored in paise.
    void displayAmount(long long paise)
    {
        std::cout << "Rs. "
                  << paise / 100
                  << '.';

        long long remainder = paise % 100;

        if (remainder < 10)
        {
            std::cout << '0';
        }

        std::cout << remainder;
    }
}

// Constructor
BankSystem::BankSystem()
{
}

// Display the main menu.
void BankSystem::displayMainMenu() const
{
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "       BANK MANAGEMENT SYSTEM\n";
    std::cout << "========================================\n";
    std::cout << "1. Create Account\n";
    std::cout << "2. Customer Login\n";
    std::cout << "3. Admin Login\n";
    std::cout << "4. Exit\n";
    std::cout << "========================================\n";
}

// Display the customer menu.
void BankSystem::displayCustomerMenu() const
{
    std::cout << "\n";
    std::cout << "========================================\n";
    std::cout << "             CUSTOMER MENU\n";
    std::cout << "========================================\n";
    std::cout << "1. Deposit Money\n";
    std::cout << "2. Withdraw Money\n";
    std::cout << "3. Transfer Money\n";
    std::cout << "4. Check Balance\n";
    std::cout << "5. Mini Statement\n";
    std::cout << "6. Change Password\n";
    std::cout << "7. Close Account\n";
    std::cout << "8. Logout\n";
    std::cout << "========================================\n";
}

// Create a new customer account.
void BankSystem::createCustomerAccount()
{
    std::string customerName;
    std::string password;

    std::cout << "\n========== CREATE ACCOUNT ==========\n";

    std::cout << "Enter customer name: ";

    if (!std::getline(std::cin, customerName))
    {
        return;
    }

    std::cout << "Create a password: ";

    if (!std::getline(std::cin, password))
    {
        return;
    }

    int accountNumber =
        bank.createAccount(customerName, password);

    if (accountNumber == -1)
    {
        std::cout << "\nAccount creation failed.\n";
        std::cout << "Name and password must not be empty.\n";
        return;
    }

    std::cout << "\nAccount created successfully!\n";
    std::cout << "Customer Name: " << customerName << '\n';
    std::cout << "Account Number: " << accountNumber << '\n';
    std::cout << "Opening Balance: Rs. 0.00\n";

    std::cout << "Keep your account number safe.\n";
}

// Handle customer login and the authenticated session.
void BankSystem::customerLogin()
{
    int accountNumber;
    std::string password;

    std::cout << "\n========== CUSTOMER LOGIN ==========\n";

    if (!readInteger("Enter account number: ", accountNumber))
    {
        std::cout << "Invalid account number input.\n";
        return;
    }

    std::cout << "Enter password: ";

    if (!std::getline(std::cin, password))
    {
        return;
    }

    if (!bank.authenticate(accountNumber, password))
    {
        std::cout << "Login failed. Check your credentials.\n";
        return;
    }

    std::cout << "\nLogin successful.\n";

    bool loggedIn = true;

    while (loggedIn)
    {
        displayCustomerMenu();

        int choice;

        if (!readInteger("Enter your choice: ", choice))
        {
            if (std::cin.eof())
            {
                return;
            }

            std::cout << "Invalid input. Enter a menu number.\n";
            continue;
        }

        switch (choice)
        {
            case 1:
            {
                long long amount;

                if (!readAmount(
                        "Enter deposit amount in Rs.: ",
                        amount))
                {
                    std::cout << "Invalid amount.\n";
                    break;
                }

                if (bank.deposit(accountNumber, amount))
                {
                    std::cout << "Deposit successful. Amount: ";
                    displayAmount(amount);
                    std::cout << '\n';
                }
                else
                {
                    std::cout << "Deposit failed.\n";
                }

                break;
            }

            case 2:
            {
                long long amount;

                if (!readAmount(
                        "Enter withdrawal amount in Rs.: ",
                        amount))
                {
                    std::cout << "Invalid amount.\n";
                    break;
                }

                if (bank.withdraw(accountNumber, amount))
                {
                    std::cout << "Withdrawal successful. Amount: ";
                    displayAmount(amount);
                    std::cout << '\n';
                }
                else
                {
                    std::cout
                        << "Withdrawal failed. Check your balance.\n";
                }

                break;
            }

            case 3:
            {
                int receiverAccount;
                long long amount;

                if (!readInteger(
                        "Enter recipient account number: ",
                        receiverAccount))
                {
                    std::cout << "Invalid account number.\n";
                    break;
                }

                if (!readAmount(
                        "Enter transfer amount in Rs.: ",
                        amount))
                {
                    std::cout << "Invalid amount.\n";
                    break;
                }

                if (bank.transfer(
                        accountNumber,
                        receiverAccount,
                        amount))
                {
                    std::cout << "Transfer successful.\n";
                    std::cout << "Amount transferred: ";
                    displayAmount(amount);
                    std::cout << "\nRecipient account: "
                              << receiverAccount << '\n';
                }
                else
                {
                    std::cout
                        << "Transfer failed. Check the recipient "
                        << "account, balance, and account status.\n";
                }

                break;
            }

            case 4:
            {
                long long balance;

                if (bank.checkBalance(accountNumber, balance))
                {
                    std::cout << "\n========== ACCOUNT BALANCE ==========\n";
                    std::cout << "Account Number: "
                              << accountNumber << '\n';
                    std::cout << "Available Balance: ";
                    displayAmount(balance);
                    std::cout << '\n';
                }
                else
                {
                    std::cout << "Unable to retrieve balance.\n";
                }

                break;
            }

            case 5:
            {
                std::vector<Transaction> statement =
                    bank.getMiniStatement(accountNumber);

                std::cout << "\n========== MINI STATEMENT ==========\n";

                if (statement.empty())
                {
                    std::cout << "No transactions available.\n";
                }
                else
                {
                    for (const Transaction& transaction : statement)
                    {
                        std::cout << '\n'
                                  << transaction.getTransactionDetails()
                                  << "\n------------------------------------\n";
                    }
                }

                break;
            }

            case 6:
            {
                std::string oldPassword;
                std::string newPassword;

                std::cout << "Enter current password: ";

                if (!std::getline(std::cin, oldPassword))
                {
                    return;
                }

                std::cout << "Enter new password: ";

                if (!std::getline(std::cin, newPassword))
                {
                    return;
                }

                if (bank.changePassword(
                        accountNumber,
                        oldPassword,
                        newPassword))
                {
                    std::cout << "Password changed successfully.\n";
                }
                else
                {
                    std::cout
                        << "Password change failed. Check your "
                        << "current and new passwords.\n";
                }

                break;
            }

            case 7:
            {
                std::string confirmation;

                std::cout
                    << "Close your account? Enter Y to confirm: ";

                if (!std::getline(std::cin, confirmation))
                {
                    return;
                }

                if (confirmation == "Y" || confirmation == "y")
                {
                    if (bank.closeAccount(accountNumber))
                    {
                        std::cout << "Account closed successfully.\n";
                        loggedIn = false;
                    }
                    else
                    {
                        std::cout
                            << "Account closure failed. The account "
                            << "must have a zero balance.\n";
                    }
                }
                else
                {
                    std::cout << "Account closure cancelled.\n";
                }

                break;
            }

            case 8:
                std::cout << "Logged out successfully.\n";
                loggedIn = false;
                break;

            default:
                std::cout << "Invalid choice. Select 1-8.\n";
        }
    }
}

// Main application loop.
void BankSystem::run()
{
    bool running = true;

    while (running)
    {
        displayMainMenu();

        int choice;

        if (!readInteger("Enter your choice: ", choice))
        {
            if (std::cin.eof())
            {
                break;
            }

            std::cout << "Invalid input. Enter a menu number.\n";
            continue;
        }

        switch (choice)
        {
            case 1:
                createCustomerAccount();
                break;

            case 2:
                customerLogin();
                break;

            case 3:
                std::cout
                    << "\nAdmin panel is not implemented yet.\n";
                break;

            case 4:
                std::cout
                    << "\nThank you for using our banking system.\n";
                running = false;
                break;

            default:
                std::cout << "Invalid choice. Select 1-4.\n";
        }
    }
}

// Program entry point.
int main()
{
    BankSystem system;
    system.run();

    return 0;
}
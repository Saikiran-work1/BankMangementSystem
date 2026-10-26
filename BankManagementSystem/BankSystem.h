#ifndef BANKSYSTEM_H
#define BANKSYSTEM_H

#include "Bank.h"

class BankSystem
{
private:
    Bank bank;

    void displayMainMenu() const;
    void displayCustomerMenu() const;

    void createCustomerAccount();
    void customerLogin();

public:
    BankSystem();

    void run();
};

#endif
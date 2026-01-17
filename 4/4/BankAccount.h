#pragma once
#include "Date.h"
#include <string>
#include <iostream>

enum class AccountType {
    CHECKING,
    SAVINGS,
    BUSINESS,
    FIXED_DEPOSIT
};

enum class AccountStatus {
    ACTIVE,
    CLOSED,
    FROZEN
};

class BankAccount {
private:
    std::string accountNumber;
    int clientId;
    double balance;
    AccountType type;
    Date openingDate;
    AccountStatus status;

    static int accountCounter;

public:
    BankAccount();
    BankAccount(int clientId, AccountType type, double initialBalance = 0.0);

    std::string getAccountNumber() const;
    int getClientId() const;
    double getBalance() const;
    AccountType getType() const;
    Date getOpeningDate() const;
    AccountStatus getStatus() const;
    std::string getTypeString() const;
    std::string getStatusString() const;

    void setClientId(int id);
    void setType(AccountType type);
    void setStatus(AccountStatus status);

    bool deposit(double amount);
    bool withdraw(double amount);

    bool hasSufficientFunds(double amount) const;
    bool isActive() const;

    void activate();
    void close();
    void freeze();

    static int getTotalAccounts();

    friend std::ostream& operator<<(std::ostream& os, const BankAccount& account);
};
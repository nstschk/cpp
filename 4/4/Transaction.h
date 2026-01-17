#pragma once
#include "Date.h"
#include <string>
#include <iostream>

enum class TransactionType {
    DEPOSIT,
    WITHDRAWAL,
    TRANSFER,
    FEE
};

class Transaction {
private:
    std::string transactionId;
    std::string fromAccount;
    std::string toAccount;
    double amount;
    Date transactionDate;
    TransactionType type;

    static int transactionCounter;

public:
    Transaction();
    Transaction(const std::string& from, const std::string& to,
        double amount, TransactionType type);

    std::string getTransactionId() const;
    std::string getFromAccount() const;
    std::string getToAccount() const;
    double getAmount() const;
    Date getTransactionDate() const;
    TransactionType getType() const;
    std::string getTypeString() const;

    static int getTotalTransactions();

    friend std::ostream& operator<<(std::ostream& os, const Transaction& transaction);
};
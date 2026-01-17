#include "Transaction.h"
#include <sstream>
#include <iomanip>

int Transaction::transactionCounter = 1;

Transaction::Transaction()
    : transactionId("TXN" + std::to_string(transactionCounter++)), amount(0.0),
    type(TransactionType::DEPOSIT) {
}

Transaction::Transaction(const std::string& from, const std::string& to,
    double amount, TransactionType type)
    : transactionId("TXN" + std::to_string(transactionCounter++)),
    fromAccount(from), toAccount(to), amount(amount), type(type) {
}

std::string Transaction::getTransactionId() const { return transactionId; }
std::string Transaction::getFromAccount() const { return fromAccount; }
std::string Transaction::getToAccount() const { return toAccount; }
double Transaction::getAmount() const { return amount; }
Date Transaction::getTransactionDate() const { return transactionDate; }
TransactionType Transaction::getType() const { return type; }

std::string Transaction::getTypeString() const {
    switch (type) {
    case TransactionType::DEPOSIT: return "Deposit";
    case TransactionType::WITHDRAWAL: return "Withdrawal";
    case TransactionType::TRANSFER: return "Transfer";
    case TransactionType::FEE: return "Fee";
    default: return "Unknown";
    }
}

int Transaction::getTotalTransactions() { return transactionCounter - 1; }

std::ostream& operator<<(std::ostream& os, const Transaction& transaction) {
    os << "[" << transaction.transactionDate << "] "
        << transaction.transactionId << " | "
        << transaction.getTypeString() << " | ";

    if (!transaction.fromAccount.empty()) {
        os << "From: " << transaction.fromAccount << " ";
    }
    if (!transaction.toAccount.empty()) {
        os << "To: " << transaction.toAccount << " ";
    }

    os << "| Amount: $" << std::fixed << std::setprecision(2) << transaction.amount;
    return os;
}
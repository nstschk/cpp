#include "BankAccount.h"
#include <sstream>
#include <iomanip>

int BankAccount::accountCounter = 1000000;

BankAccount::BankAccount()
    : accountNumber("ACC" + std::to_string(++accountCounter)), clientId(0), balance(0.0),
    type(AccountType::CHECKING), status(AccountStatus::ACTIVE) {
}

BankAccount::BankAccount(int clientId, AccountType type, double initialBalance)
    : accountNumber("ACC" + std::to_string(++accountCounter)), clientId(clientId),
    balance(initialBalance), type(type), status(AccountStatus::ACTIVE) {
    if (initialBalance < 0) {
        throw std::invalid_argument("Initial balance cannot be negative");
    }
}

std::string BankAccount::getAccountNumber() const { return accountNumber; }
int BankAccount::getClientId() const { return clientId; }
double BankAccount::getBalance() const { return balance; }
AccountType BankAccount::getType() const { return type; }
Date BankAccount::getOpeningDate() const { return openingDate; }
AccountStatus BankAccount::getStatus() const { return status; }

std::string BankAccount::getTypeString() const {
    switch (type) {
    case AccountType::CHECKING: return "Checking";
    case AccountType::SAVINGS: return "Savings";
    case AccountType::BUSINESS: return "Business";
    case AccountType::FIXED_DEPOSIT: return "Fixed Deposit";
    default: return "Unknown";
    }
}

std::string BankAccount::getStatusString() const {
    switch (status) {
    case AccountStatus::ACTIVE: return "Active";
    case AccountStatus::CLOSED: return "Closed";
    case AccountStatus::FROZEN: return "Frozen";
    default: return "Unknown";
    }
}

void BankAccount::setClientId(int id) { clientId = id; }
void BankAccount::setType(AccountType type) { this->type = type; }
void BankAccount::setStatus(AccountStatus status) { this->status = status; }

bool BankAccount::deposit(double amount) {
    if (amount <= 0 || !isActive()) {
        return false;
    }
    balance += amount;
    return true;
}

bool BankAccount::withdraw(double amount) {
    if (amount <= 0 || !isActive() || !hasSufficientFunds(amount)) {
        return false;
    }
    balance -= amount;
    return true;
}

bool BankAccount::hasSufficientFunds(double amount) const {
    return balance >= amount;
}

bool BankAccount::isActive() const {
    return status == AccountStatus::ACTIVE;
}

void BankAccount::activate() { status = AccountStatus::ACTIVE; }
void BankAccount::close() { status = AccountStatus::CLOSED; }
void BankAccount::freeze() { status = AccountStatus::FROZEN; }

int BankAccount::getTotalAccounts() { return accountCounter - 1000000; }

std::ostream& operator<<(std::ostream& os, const BankAccount& account) {
    os << "Account: " << account.accountNumber
        << " | Owner ID: " << account.clientId
        << " | Balance: $" << std::fixed << std::setprecision(2) << account.balance
        << " | Type: " << account.getTypeString()
        << " | Status: " << account.getStatusString()
        << " | Opened: " << account.openingDate;
    return os;
}
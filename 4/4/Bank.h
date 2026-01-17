#pragma once
#include "Client.h"
#include "PremiumClient.h"
#include "BankAccount.h"
#include "Transaction.h"
#include <vector>
#include <memory>
#include <string>
#include <algorithm>

class Bank {
private:
    std::string bankName;
    std::string bankCode;

    std::vector<std::shared_ptr<Client>> clients;
    std::vector<BankAccount> accounts;
    std::vector<Transaction> transactions;

public:
    Bank(const std::string& name = "SimpleBank", const std::string& code = "SB001");

    int addClient(const std::string& firstName, const std::string& lastName,
        const Address& address, bool isPremium = false,
        const std::string& premiumLevel = "Gold");

    bool removeClient(int clientId);
    std::shared_ptr<Client> findClient(int clientId) const;
    void displayAllClients() const;
    bool updateClientAddress(int clientId, const Address& newAddress);
    int getClientCount() const;

    std::string openAccount(int clientId, AccountType type, double initialDeposit = 0.0);
    bool closeAccount(const std::string& accountNumber);
    BankAccount* findAccount(const std::string& accountNumber);
    std::vector<BankAccount> getClientAccounts(int clientId) const;
    void displayAllAccounts() const;
    int getAccountCount() const;

    bool deposit(const std::string& accountNumber, double amount);
    bool withdraw(const std::string& accountNumber, double amount);
    bool transfer(const std::string& fromAccount, const std::string& toAccount, double amount);

    void addTransaction(const Transaction& transaction);
    void displayTransactionHistory() const;
    void displayAccountTransactions(const std::string& accountNumber) const;
    int getTransactionCount() const;

    void displayBankInfo() const;
    void displayStatistics() const;
    double getTotalBankBalance() const;

    bool clientExists(int clientId) const;
    bool accountExists(const std::string& accountNumber) const;
    bool canClientBeRemoved(int clientId) const;

private:
    void logTransaction(const std::string& from, const std::string& to,
        double amount, TransactionType type);
};
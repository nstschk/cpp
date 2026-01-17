#include "Bank.h"
#include <iostream>
#include <iomanip>

Bank::Bank(const std::string& name, const std::string& code)
    : bankName(name), bankCode(code) {
}

int Bank::addClient(const std::string& firstName, const std::string& lastName,
    const Address& address, bool isPremium, const std::string& premiumLevel) {

    if (isPremium) {
        auto client = std::make_shared<PremiumClient>(
            firstName, lastName, address, premiumLevel);
        clients.push_back(client);
        return client->getClientId();
    }
    else {
        auto client = std::make_shared<Client>(firstName, lastName, address);
        clients.push_back(client);
        return client->getClientId();
    }
}

bool Bank::removeClient(int clientId) {
    if (!canClientBeRemoved(clientId)) {
        return false;
    }

    auto it = std::remove_if(clients.begin(), clients.end(),
        [clientId](const std::shared_ptr<Client>& client) {
            return client->getClientId() == clientId;
        });

    if (it != clients.end()) {
        clients.erase(it, clients.end());

        accounts.erase(
            std::remove_if(accounts.begin(), accounts.end(),
                [clientId](const BankAccount& account) {
                    return account.getClientId() == clientId;
                }),
            accounts.end()
        );

        return true;
    }

    return false;
}

std::shared_ptr<Client> Bank::findClient(int clientId) const {
    for (const auto& client : clients) {
        if (client->getClientId() == clientId) {
            return client;
        }
    }
    return nullptr;
}

void Bank::displayAllClients() const {
    std::cout << "\n=== ALL CLIENTS ===" << std::endl;
    std::cout << "Total: " << clients.size() << " clients\n" << std::endl;

    for (const auto& client : clients) {
        std::cout << *client << std::endl;
    }
}

bool Bank::updateClientAddress(int clientId, const Address& newAddress) {
    auto client = findClient(clientId);
    if (client) {
        client->setAddress(newAddress);
        return true;
    }
    return false;
}

int Bank::getClientCount() const {
    return clients.size();
}

std::string Bank::openAccount(int clientId, AccountType type, double initialDeposit) {
    if (!clientExists(clientId)) {
        throw std::invalid_argument("Client does not exist");
    }

    if (initialDeposit < 0) {
        throw std::invalid_argument("Initial deposit cannot be negative");
    }

    BankAccount account(clientId, type, initialDeposit);
    accounts.push_back(account);

    if (initialDeposit > 0) {
        logTransaction("", account.getAccountNumber(),
            initialDeposit, TransactionType::DEPOSIT);
    }

    return account.getAccountNumber();
}

bool Bank::closeAccount(const std::string& accountNumber) {
    auto account = findAccount(accountNumber);
    if (account && account->getBalance() == 0 && account->isActive()) {
        account->close();
        return true;
    }
    return false;
}

BankAccount* Bank::findAccount(const std::string& accountNumber) {
    for (auto& account : accounts) {
        if (account.getAccountNumber() == accountNumber) {
            return &account;
        }
    }
    return nullptr;
}

std::vector<BankAccount> Bank::getClientAccounts(int clientId) const {
    std::vector<BankAccount> clientAccounts;
    for (const auto& account : accounts) {
        if (account.getClientId() == clientId) {
            clientAccounts.push_back(account);
        }
    }
    return clientAccounts;
}

void Bank::displayAllAccounts() const {
    std::cout << "\n=== ALL ACCOUNTS ===" << std::endl;
    std::cout << "Total: " << accounts.size() << " accounts\n" << std::endl;

    for (const auto& account : accounts) {
        std::cout << account << std::endl;
    }
}

int Bank::getAccountCount() const {
    return accounts.size();
}

bool Bank::deposit(const std::string& accountNumber, double amount) {
    auto account = findAccount(accountNumber);
    if (account && account->deposit(amount)) {
        logTransaction("", accountNumber, amount, TransactionType::DEPOSIT);
        return true;
    }
    return false;
}

bool Bank::withdraw(const std::string& accountNumber, double amount) {
    auto account = findAccount(accountNumber);
    if (account && account->withdraw(amount)) {
        logTransaction(accountNumber, "", amount, TransactionType::WITHDRAWAL);
        return true;
    }
    return false;
}

bool Bank::transfer(const std::string& fromAccount, const std::string& toAccount,
    double amount) {
    auto from = findAccount(fromAccount);
    auto to = findAccount(toAccount);

    if (!from || !to || !from->isActive() || !to->isActive()) {
        return false;
    }

    if (from->withdraw(amount) && to->deposit(amount)) {
        logTransaction(fromAccount, toAccount, amount, TransactionType::TRANSFER);
        return true;
    }

    return false;
}

void Bank::addTransaction(const Transaction& transaction) {
    transactions.push_back(transaction);
}

void Bank::displayTransactionHistory() const {
    std::cout << "\n=== TRANSACTION HISTORY ===" << std::endl;
    std::cout << "Total: " << transactions.size() << " transactions\n" << std::endl;

    for (const auto& transaction : transactions) {
        std::cout << transaction << std::endl;
    }
}

void Bank::displayAccountTransactions(const std::string& accountNumber) const {
    std::cout << "\n=== TRANSACTIONS FOR ACCOUNT: " << accountNumber << " ===" << std::endl;

    int count = 0;
    for (const auto& transaction : transactions) {
        if (transaction.getFromAccount() == accountNumber ||
            transaction.getToAccount() == accountNumber) {
            std::cout << transaction << std::endl;
            count++;
        }
    }

    if (count == 0) {
        std::cout << "No transactions found for this account." << std::endl;
    }
}

int Bank::getTransactionCount() const {
    return transactions.size();
}

void Bank::displayBankInfo() const {
    std::cout << "\n=== BANK INFORMATION ===" << std::endl;
    std::cout << "Bank Name: " << bankName << std::endl;
    std::cout << "Bank Code: " << bankCode << std::endl;
    std::cout << "Total Clients: " << getClientCount() << std::endl;
    std::cout << "Total Accounts: " << getAccountCount() << std::endl;
    std::cout << "Total Balance: $" << std::fixed << std::setprecision(2)
        << getTotalBankBalance() << std::endl;
}

void Bank::displayStatistics() const {
    std::cout << "\n=== BANK STATISTICS ===" << std::endl;

    int activeAccounts = 0;
    double totalBalance = 0.0;

    for (const auto& account : accounts) {
        if (account.isActive()) {
            activeAccounts++;
            totalBalance += account.getBalance();
        }
    }

    std::cout << "Active Accounts: " << activeAccounts << std::endl;
    std::cout << "Total Balance: $" << std::fixed << std::setprecision(2)
        << totalBalance << std::endl;
    std::cout << "Average Balance: $" << std::fixed << std::setprecision(2)
        << (activeAccounts > 0 ? totalBalance / activeAccounts : 0) << std::endl;
}

double Bank::getTotalBankBalance() const {
    double total = 0.0;
    for (const auto& account : accounts) {
        if (account.isActive()) {
            total += account.getBalance();
        }
    }
    return total;
}

bool Bank::clientExists(int clientId) const {
    return findClient(clientId) != nullptr;
}

bool Bank::accountExists(const std::string& accountNumber) const {
    for (const auto& account : accounts) {
        if (account.getAccountNumber() == accountNumber) {
            return true;
        }
    }
    return false;
}

bool Bank::canClientBeRemoved(int clientId) const {
    for (const auto& account : accounts) {
        if (account.getClientId() == clientId &&
            account.isActive() &&
            account.getBalance() > 0) {
            return false;
        }
    }
    return true;
}

void Bank::logTransaction(const std::string& from, const std::string& to,
    double amount, TransactionType type) {
    Transaction transaction(from, to, amount, type);
    transactions.push_back(transaction);
}
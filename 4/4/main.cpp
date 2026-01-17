#include "Bank.h"
#include <iostream>
#include <limits>

void displayMenu() {
    std::cout << "\n=== SIMPLE BANKING SYSTEM ===" << std::endl;
    std::cout << "1. Add New Client" << std::endl;
    std::cout << "2. Display All Clients" << std::endl;
    std::cout << "3. Find Client by ID" << std::endl;
    std::cout << "4. Open New Account" << std::endl;
    std::cout << "5. Display All Accounts" << std::endl;
    std::cout << "6. Deposit Money" << std::endl;
    std::cout << "7. Withdraw Money" << std::endl;
    std::cout << "8. Transfer Money" << std::endl;
    std::cout << "9. Display Account Transactions" << std::endl;
    std::cout << "10. Display Bank Information" << std::endl;
    std::cout << "11. Display Transaction History" << std::endl;
    std::cout << "12. Display Statistics" << std::endl;
    std::cout << "13. Close Account" << std::endl;
    std::cout << "14. Remove Client" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "=============================" << std::endl;
    std::cout << "Enter your choice: ";
}

void clearInput() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int getIntInput(const std::string& prompt) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInput();
            return value;
        }
        else {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            clearInput();
        }
    }
}

double getDoubleInput(const std::string& prompt) {
    double value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value) {
            clearInput();
            return value;
        }
        else {
            std::cout << "Invalid input. Please enter a number." << std::endl;
            clearInput();
        }
    }
}

std::string getStringInput(const std::string& prompt) {
    std::string value;
    std::cout << prompt;
    std::getline(std::cin, value);
    return value;
}

Address createAddress() {
    std::cout << "\n--- Enter Address ---" << std::endl;
    std::string street = getStringInput("Street: ");
    std::string city = getStringInput("City: ");
    std::string postalCode = getStringInput("Postal Code: ");
    std::string country = getStringInput("Country: ");

    return Address(street, city, postalCode, country);
}

void addClient(Bank& bank) {
    std::cout << "\n--- Add New Client ---" << std::endl;

    std::string firstName = getStringInput("First Name: ");
    std::string lastName = getStringInput("Last Name: ");

    std::cout << "Client Type:" << std::endl;
    std::cout << "1. Standard Client" << std::endl;
    std::cout << "2. Premium Client" << std::endl;
    int typeChoice = getIntInput("Choose type (1-2): ");

    Address address = createAddress();

    if (typeChoice == 1) {
        int clientId = bank.addClient(firstName, lastName, address, false);
        std::cout << "Standard client added successfully! Client ID: " << clientId << std::endl;
    }
    else if (typeChoice == 2) {
        std::string level = getStringInput("Premium Level (Gold/Platinum/Diamond): ");
        int clientId = bank.addClient(firstName, lastName, address, true, level);
        std::cout << "Premium client added successfully! Client ID: " << clientId << std::endl;
    }
    else {
        std::cout << "Invalid choice. Client not added." << std::endl;
    }
}

void findClientById(Bank& bank) {
    int clientId = getIntInput("Enter Client ID: ");
    auto client = bank.findClient(clientId);

    if (client) {
        std::cout << "\n--- Client Found ---" << std::endl;
        client->displayInfo();

        auto accounts = bank.getClientAccounts(clientId);
        if (!accounts.empty()) {
            std::cout << "\nClient's Accounts:" << std::endl;
            for (const auto& account : accounts) {
                std::cout << "  " << account << std::endl;
            }
        }
        else {
            std::cout << "No accounts found for this client." << std::endl;
        }
    }
    else {
        std::cout << "Client not found!" << std::endl;
    }
}

void openAccount(Bank& bank) {
    std::cout << "\n--- Open New Account ---" << std::endl;

    int clientId = getIntInput("Client ID: ");

    if (!bank.clientExists(clientId)) {
        std::cout << "Client does not exist!" << std::endl;
        return;
    }

    std::cout << "\nAccount Types:" << std::endl;
    std::cout << "1. Checking Account" << std::endl;
    std::cout << "2. Savings Account" << std::endl;
    std::cout << "3. Business Account" << std::endl;
    std::cout << "4. Fixed Deposit Account" << std::endl;

    int typeChoice = getIntInput("Choose account type (1-4): ");
    AccountType type;

    switch (typeChoice) {
    case 1: type = AccountType::CHECKING; break;
    case 2: type = AccountType::SAVINGS; break;
    case 3: type = AccountType::BUSINESS; break;
    case 4: type = AccountType::FIXED_DEPOSIT; break;
    default:
        std::cout << "Invalid choice!" << std::endl;
        return;
    }

    double initialDeposit = getDoubleInput("Initial Deposit: $");

    try {
        std::string accountNumber = bank.openAccount(clientId, type, initialDeposit);
        std::cout << "Account opened successfully!" << std::endl;
        std::cout << "Account Number: " << accountNumber << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Error: " << e.what() << std::endl;
    }
}

void depositMoney(Bank& bank) {
    std::cout << "\n--- Deposit Money ---" << std::endl;
    std::string accountNumber = getStringInput("Account Number: ");
    double amount = getDoubleInput("Amount to deposit: $");

    if (bank.deposit(accountNumber, amount)) {
        std::cout << "Deposit successful!" << std::endl;
    }
    else {
        std::cout << "Deposit failed." << std::endl;
    }
}

void withdrawMoney(Bank& bank) {
    std::cout << "\n--- Withdraw Money ---" << std::endl;
    std::string accountNumber = getStringInput("Account Number: ");
    double amount = getDoubleInput("Amount to withdraw: $");

    if (bank.withdraw(accountNumber, amount)) {
        std::cout << "Withdrawal successful!" << std::endl;
    }
    else {
        std::cout << "Withdrawal failed." << std::endl;
    }
}

void transferMoney(Bank& bank) {
    std::cout << "\n--- Transfer Money ---" << std::endl;
    std::string fromAccount = getStringInput("From Account Number: ");
    std::string toAccount = getStringInput("To Account Number: ");
    double amount = getDoubleInput("Amount to transfer: $");

    if (bank.transfer(fromAccount, toAccount, amount)) {
        std::cout << "Transfer successful!" << std::endl;
    }
    else {
        std::cout << "Transfer failed." << std::endl;
    }
}

void displayAccountTransactions(Bank& bank) {
    std::string accountNumber = getStringInput("Enter Account Number: ");
    bank.displayAccountTransactions(accountNumber);
}

void closeAccount(Bank& bank) {
    std::string accountNumber = getStringInput("Enter Account Number to close: ");

    if (bank.closeAccount(accountNumber)) {
        std::cout << "Account closed successfully!" << std::endl;
    }
    else {
        std::cout << "Cannot close account." << std::endl;
    }
}

void removeClient(Bank& bank) {
    int clientId = getIntInput("Enter Client ID to remove: ");

    if (bank.removeClient(clientId)) {
        std::cout << "Client removed successfully!" << std::endl;
    }
    else {
        std::cout << "Cannot remove client." << std::endl;
    }
}

int main() {
    Bank bank;

    // Добавление тестовых данных
    try {
        Address addr1("123 Main St", "New York", "10001", "USA");
        Address addr2("456 Oak Ave", "Los Angeles", "90001", "USA");

        int client1 = bank.addClient("John", "Doe", addr1, false);
        int client2 = bank.addClient("Jane", "Smith", addr2, true, "Gold");

        bank.openAccount(client1, AccountType::CHECKING, 1000.0);
        bank.openAccount(client1, AccountType::SAVINGS, 5000.0);
        bank.openAccount(client2, AccountType::CHECKING, 2000.0);

        std::cout << "=== Welcome to Simple Banking System ===" << std::endl;
        std::cout << "Test data loaded successfully!" << std::endl;
        std::cout << "Test Client IDs: " << client1 << ", " << client2 << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "Initialization error: " << e.what() << std::endl;
    }

    int choice;
    do {
        displayMenu();
        choice = getIntInput("");

        switch (choice) {
        case 1:
            addClient(bank);
            break;
        case 2:
            bank.displayAllClients();
            break;
        case 3:
            findClientById(bank);
            break;
        case 4:
            openAccount(bank);
            break;
        case 5:
            bank.displayAllAccounts();
            break;
        case 6:
            depositMoney(bank);
            break;
        case 7:
            withdrawMoney(bank);
            break;
        case 8:
            transferMoney(bank);
            break;
        case 9:
            displayAccountTransactions(bank);
            break;
        case 10:
            bank.displayBankInfo();
            break;
        case 11:
            bank.displayTransactionHistory();
            break;
        case 12:
            bank.displayStatistics();
            break;
        case 13:
            closeAccount(bank);
            break;
        case 14:
            removeClient(bank);
            break;
        case 0:
            std::cout << "Thank you for using Simple Banking System!" << std::endl;
            break;
        default:
            std::cout << "Invalid choice. Please try again." << std::endl;
        }

        if (choice != 0) {
            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        }

    } while (choice != 0);

    return 0;
}
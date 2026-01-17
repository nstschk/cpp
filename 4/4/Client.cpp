#include "Client.h"
#include <iostream>

// Инициализация статического счетчика
int Client::clientCounter = 1000;

// Конструктор по умолчанию
Client::Client()
    : clientId(++clientCounter),
    firstName("Unknown"),
    lastName("Unknown"),
    address(),
    registrationDate() {
}

// Параметризованный конструктор
Client::Client(const std::string& firstName, const std::string& lastName,
    const Address& address)
    : clientId(++clientCounter),
    firstName(firstName),
    lastName(lastName),
    address(address),
    registrationDate() {
}

// Виртуальный деструктор
Client::~Client() {
    // Пустая реализация
}

// Геттеры
int Client::getClientId() const {
    return clientId;
}

std::string Client::getFirstName() const {
    return firstName;
}

std::string Client::getLastName() const {
    return lastName;
}

Address Client::getAddress() const {
    return address;
}

Date Client::getRegistrationDate() const {
    return registrationDate;
}

std::string Client::getClientType() const {
    return "Standard";
}

// Сеттеры
void Client::setFirstName(const std::string& firstName) {
    this->firstName = firstName;
}

void Client::setLastName(const std::string& lastName) {
    this->lastName = lastName;
}

void Client::setAddress(const Address& address) {
    this->address = address;
}

// Виртуальные методы
void Client::displayInfo() const {
    std::cout << "\n=== Client Information ===" << std::endl;
    std::cout << "Client ID: " << clientId << std::endl;
    std::cout << "Name: " << firstName << " " << lastName << std::endl;
    std::cout << "Type: " << getClientType() << std::endl;
    std::cout << "Address: " << address << std::endl;
    std::cout << "Registration Date: " << registrationDate << std::endl;
}

double Client::calculateServiceFee(double amount) const {
    // Стандартная комиссия 1%
    return amount * 0.01;
}

// Статические методы
int Client::getTotalClients() {
    return clientCounter - 1000;
}

// Оператор вывода
std::ostream& operator<<(std::ostream& os, const Client& client) {
    os << "ID: " << client.clientId
        << " | Name: " << client.firstName << " " << client.lastName
        << " | Type: " << client.getClientType();
    return os;
}
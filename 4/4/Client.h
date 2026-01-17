#pragma once
#include "Address.h"
#include "Date.h"
#include <string>
#include <memory>
#include <iostream>

class Client {
protected:
    int clientId;
    std::string firstName;
    std::string lastName;
    Address address;
    Date registrationDate;

    static int clientCounter;

public:
    Client();
    Client(const std::string& firstName, const std::string& lastName,
        const Address& address);

    virtual ~Client();

    int getClientId() const;
    std::string getFirstName() const;
    std::string getLastName() const;
    Address getAddress() const;
    Date getRegistrationDate() const;
    virtual std::string getClientType() const;

    void setFirstName(const std::string& firstName);
    void setLastName(const std::string& lastName);
    void setAddress(const Address& address);

    virtual void displayInfo() const;
    virtual double calculateServiceFee(double amount) const;

    static int getTotalClients();

    friend std::ostream& operator<<(std::ostream& os, const Client& client);
};
#pragma once
#include <string>
#include <iostream>

class Address {
private:
    std::string street;
    std::string city;
    std::string postalCode;
    std::string country;

public:
    Address();
    Address(const std::string& street, const std::string& city,
        const std::string& postalCode, const std::string& country);

    std::string getStreet() const;
    std::string getCity() const;
    std::string getPostalCode() const;
    std::string getCountry() const;

    void setStreet(const std::string& street);
    void setCity(const std::string& city);
    void setPostalCode(const std::string& postalCode);
    void setCountry(const std::string& country);

    void display() const;
    std::string toString() const;

    friend std::ostream& operator<<(std::ostream& os, const Address& address);
};
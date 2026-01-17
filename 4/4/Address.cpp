#include "Address.h"

Address::Address()
    : street("Unknown"), city("Unknown"), postalCode("000000"), country("Unknown") {
}

Address::Address(const std::string& street, const std::string& city,
    const std::string& postalCode, const std::string& country)
    : street(street), city(city), postalCode(postalCode), country(country) {
}

std::string Address::getStreet() const { return street; }
std::string Address::getCity() const { return city; }
std::string Address::getPostalCode() const { return postalCode; }
std::string Address::getCountry() const { return country; }

void Address::setStreet(const std::string& street) { this->street = street; }
void Address::setCity(const std::string& city) { this->city = city; }
void Address::setPostalCode(const std::string& postalCode) { this->postalCode = postalCode; }
void Address::setCountry(const std::string& country) { this->country = country; }

void Address::display() const {
    std::cout << street << ", " << city << ", " << postalCode << ", " << country;
}

std::string Address::toString() const {
    return street + ", " + city + ", " + postalCode + ", " + country;
}

std::ostream& operator<<(std::ostream& os, const Address& address) {
    os << address.toString();
    return os;
}
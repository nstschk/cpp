#include "PremiumClient.h"

PremiumClient::PremiumClient()
    : Client(), premiumLevel("Gold"), discountPercentage(10.0) {
}

PremiumClient::PremiumClient(const std::string& firstName, const std::string& lastName,
    const Address& address, const std::string& level, double discount)
    : Client(firstName, lastName, address),
    premiumLevel(level), discountPercentage(discount) {
    if (discount < 0 || discount > 50) {
        discountPercentage = 10.0;
    }
}

std::string PremiumClient::getPremiumLevel() const { return premiumLevel; }
double PremiumClient::getDiscountPercentage() const { return discountPercentage; }
std::string PremiumClient::getClientType() const {
    return "Premium (" + premiumLevel + ")";
}

void PremiumClient::setPremiumLevel(const std::string& level) {
    premiumLevel = level;
}

void PremiumClient::setDiscountPercentage(double discount) {
    if (discount >= 0 && discount <= 50) {
        discountPercentage = discount;
    }
}

void PremiumClient::displayInfo() const {
    Client::displayInfo();
    std::cout << "Premium Level: " << premiumLevel << std::endl;
    std::cout << "Discount: " << discountPercentage << "%" << std::endl;
}

double PremiumClient::calculateServiceFee(double amount) const {
    double baseFee = amount * 0.01;
    return baseFee * (1.0 - discountPercentage / 100.0);
}

double PremiumClient::applyDiscount(double amount) const {
    return amount * (1.0 - discountPercentage / 100.0);
}

std::ostream& operator<<(std::ostream& os, const PremiumClient& client) {
    os << static_cast<const Client&>(client)
        << " | Level: " << client.premiumLevel
        << " | Discount: " << client.discountPercentage << "%";
    return os;
}
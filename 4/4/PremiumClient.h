#pragma once
#include "Client.h"

class PremiumClient : public Client {
private:
    std::string premiumLevel;
    double discountPercentage;

public:
    PremiumClient();
    PremiumClient(const std::string& firstName, const std::string& lastName,
        const Address& address, const std::string& level = "Gold", double discount = 10.0);

    std::string getPremiumLevel() const;
    double getDiscountPercentage() const;
    std::string getClientType() const override;

    void setPremiumLevel(const std::string& level);
    void setDiscountPercentage(double discount);

    void displayInfo() const override;
    double calculateServiceFee(double amount) const override;

    double applyDiscount(double amount) const;

    friend std::ostream& operator<<(std::ostream& os, const PremiumClient& client);
};
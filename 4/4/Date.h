#pragma once
#include <string>
#include <iostream>
#include <ctime>

class Date {
private:
    int day;
    int month;
    int year;

    bool isValidDate(int d, int m, int y) const;
    bool isLeapYear(int y) const;
    int daysInMonth(int m, int y) const;

public:
    Date();
    Date(int d, int m, int y);

    int getDay() const;
    int getMonth() const;
    int getYear() const;

    void setDay(int d);
    void setMonth(int m);
    void setYear(int y);

    std::string toString() const;

    friend std::ostream& operator<<(std::ostream& os, const Date& date);
    bool operator==(const Date& other) const;
    bool operator<(const Date& other) const;
};
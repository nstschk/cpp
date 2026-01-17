#include "Date.h"
#include <sstream>
#include <iomanip>

bool Date::isLeapYear(int y) const {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

int Date::daysInMonth(int m, int y) const {
    switch (m) {
    case 1: case 3: case 5: case 7: case 8: case 10: case 12:
        return 31;
    case 4: case 6: case 9: case 11:
        return 30;
    case 2:
        return isLeapYear(y) ? 29 : 28;
    default:
        return 0;
    }
}

bool Date::isValidDate(int d, int m, int y) const {
    if (y < 1900 || y > 2100) return false;
    if (m < 1 || m > 12) return false;
    if (d < 1 || d > daysInMonth(m, y)) return false;
    return true;
}

Date::Date() {
    std::time_t t = std::time(nullptr);
    std::tm now;
    localtime_s(&now, &t);
    day = now.tm_mday;
    month = now.tm_mon + 1;
    year = now.tm_year + 1900;
}

Date::Date(int d, int m, int y) {
    if (!isValidDate(d, m, y)) {
        throw std::invalid_argument("Invalid date");
    }
    day = d;
    month = m;
    year = y;
}

int Date::getDay() const { return day; }
int Date::getMonth() const { return month; }
int Date::getYear() const { return year; }

void Date::setDay(int d) {
    if (!isValidDate(d, month, year)) {
        throw std::invalid_argument("Invalid day");
    }
    day = d;
}

void Date::setMonth(int m) {
    if (!isValidDate(day, m, year)) {
        throw std::invalid_argument("Invalid month");
    }
    month = m;
}

void Date::setYear(int y) {
    if (!isValidDate(day, month, y)) {
        throw std::invalid_argument("Invalid year");
    }
    year = y;
}

std::string Date::toString() const {
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << day << "."
        << std::setw(2) << month << "." << year;
    return ss.str();
}

std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << date.toString();
    return os;
}

bool Date::operator==(const Date& other) const {
    return day == other.day && month == other.month && year == other.year;
}

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}
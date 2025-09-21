#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <iomanip>
#include <limits>
#include <string>
#include <algorithm>
#include <cctype>

using namespace std;


bool isValidNumber(const string& num, int base) {
    if (num.empty()) return false;

    for (char c : num) {
        int digit;
        if (c >= '0' && c <= '9') {
            digit = c - '0';
        }
        else if (c >= 'A' && c <= 'Z') {
            digit = 10 + (c - 'A');
        }
        else if (c >= 'a' && c <= 'z') {
            digit = 10 + (c - 'a');
        }
        else {
            return false;
        }

        if (digit >= base) {
            return false;
        }
    }
    return true;
}

long long anyToDecimal(const string& number, int fromBase) {
    long long decimal = 0;
    long long power = 1;

    string num = number;
    reverse(num.begin(), num.end());

    for (char c : num) {
        int digit;
        if (c >= '0' && c <= '9') digit = c - '0';
        else if (c >= 'A' && c <= 'Z') digit = 10 + (c - 'A');
        else digit = 10 + (c - 'a');

        decimal += digit * power;
        power *= fromBase;
    }
    return decimal;
}

string decimalToAny(long long decimal, int toBase) {
    if (decimal == 0) return "0";

    string result;
    while (decimal > 0) {
        int remainder = decimal % toBase;
        char digit;

        if (remainder < 10) digit = '0' + remainder;
        else digit = 'A' + (remainder - 10);

        result = digit + result;
        decimal /= toBase;
    }
    return result;
}

void numberSystemConverter() {
    string number;
    int fromBase, toBase;
    cout << "Введите число, которое необходимо перевести: ";
    cin >> number;

    cout << "Из системы счисления, выберите систему счисления от 2 до 36: ";
    cin >> fromBase;

    cout << "В систему счисления, выберите систему счисления от 2 до 36: ";
    cin >> toBase;

    if (fromBase < 2 || fromBase > 36 || toBase < 2 || toBase > 36) {
        cout << "Недопустимая система счисления" << endl;
        return;
    }

    if (!isValidNumber(number, fromBase)) {
        cout << "Ошибка: число '" << number << "' содержит недопустимые символы для системы с основанием " << fromBase << endl;
        cout << "Для системы с основанием " << fromBase << " допустимы символы: ";

        if (fromBase <= 10) {
            cout << "0-" << (fromBase - 1);
        }
        else {
            cout << "0-9, A-" << (char)('A' + fromBase - 11);
        }
        cout << endl;
        return;
    }

    try {
        long long decimalValue = anyToDecimal(number, fromBase);
        string result = decimalToAny(decimalValue, toBase);

        cout << "Результат: " << number << " (основание " << fromBase << ") = "
            << result << " (основание " << toBase << ")" << endl;
        cout << "Десятичное значение: " << decimalValue << endl;

    }
    catch (const exception& e) {
        cout << "Ошибка конвертации: " << e.what() << endl;
    }
}

bool selectAngleMode() {
    int choice;
    cout << "Выберите режим измерения углов" << endl;
    cout << "1 - Градусы" << endl;
    cout << "2 - Радианы" << endl;
    cout << "Выберите режим: ";
    cin >> choice;

    while (choice != 1 && choice != 2) {
        cout << "Неверный выбор! Введите 1 или 2: ";
        cin >> choice;
    }

    bool useRadians = (choice == 2);
    return useRadians;
}

double toRadians(double angle, bool useRadians) {
    if (useRadians) {
        return angle;
    }
    else {
        return angle * M_PI / 180.0;
    }
}

string getAngleUnit(bool useRadians) {
    return useRadians ? "rad" : "°";
}


int main()
{
    setlocale(LC_ALL, "rus");
    string operation;
    cout.precision(15);
    double const e = M_E;
    double x = 0, y = 0;
    double R = 0;
    bool useRadians = false;

 
    cout << "Математические операции: +, -, *, /, ^, sqrt, !, ln, log, sin, cos, tan, ctg, ||, e^" << endl;
    cout << "Конвертер систем: convert" << endl;

    cout << "Введите операцию: ";
    cin >> operation;

    if (operation == "convert") {
        numberSystemConverter();
        return 0;
    }

    if (operation == "sqrt" || operation == "!" || operation == "ln" ||
        operation == "sin" || operation == "cos" ||
        operation == "tan" || operation == "ctg" || operation == "||" || operation == "e^") {
        cout << "Введите число: ";
        cin >> x;
    }
    else if (operation == "+" || operation == "-" || operation == "*" ||
        operation == "/" || operation == "log" || operation == "^") {
        cout << "Введите число 1 (в случае функции log - аргумент): ";
        cin >> x;
        cout << "Введите число 2 (в случае функции log - основание): ";
        cin >> y;
    }
    else {
        cout << "Неизвестная операция!" << endl;
        return 1;
    }

    if (operation == "sin" || operation == "cos" || operation == "tan" || operation == "ctg") {
        useRadians = selectAngleMode();
    }

    if (operation == "+") {
        R = x + y;
        cout << x << " + " << y << " = " << R;
    }
    else if (operation == "-") {
        R = x - y;
        cout << x << " - " << y << " = " << R;
    }
    else if (operation == "*") {
        R = x * y;
        cout << x << " * " << y << " = " << R;
    }
    else if (operation == "/") {
        if (y == 0) {
            cout << "Ошибка: на 0 делить нельзя!";
        }
        else {
            R = x / y;
            cout << x << " / " << y << " = " << R;
        }
    }
    else if (operation == "||") {
        R = abs(x);
        cout << "|" << x << "| = " << R;
    }
    else if (operation == "^") {
        R = pow(x, y);
        cout << x << " ^ " << y << " = " << R;
    }
    else if (operation == "e^") {
        R = pow(e, x);
        cout << e << " ^ " << x << " = " << R;
    }
    else if (operation == "sqrt") {
        if (x < 0) {
            cout << "Ошибка: корень из отрицательного числа!";
        }
        else {
            R = sqrt(x);
            cout << "sqrt(" << x << ") = " << R;
        }
    }
    else if (operation == "!") {
        if (x < 0 || x != (int)x) {
            cout << "Ошибка: факториал определен только для целых неотрицательных чисел!";
        }
        else {
            R = 1;
            for (int i = 1; i <= (int)x; i++) {
                R *= i;
            }
            cout << (int)x << "! = " << R;
        }
    }
    else if (operation == "ln") {
        if (x <= 0) {
            cout << "Ошибка: логарифм определен только для положительных чисел!";
        }
        else {
            R = log(x);
            cout << "ln(" << x << ") = " << R;
        }
    }
    else if (operation == "log") {
        if (x <= 0 || y <= 0) {
            cout << "Ошибка: логарифм определен только для положительных чисел!";
        }
        else if (y == 1) {
            cout << "Ошибка: основание логарифма не может быть равно 1!";
        }
        else {
            R = log(x) / log(y);
            cout << "log_" << y << "(" << x << ") = " << R;
        }
    }
    else if (operation == "sin") {
        R = sin(toRadians(x, useRadians));
        cout << "sin(" << x << getAngleUnit(useRadians) << ") = " << fixed << setprecision(4) << R;
    }
    else if (operation == "cos") {
        R = cos(toRadians(x, useRadians));
        cout << "cos(" << x << getAngleUnit(useRadians) << ") = " << fixed << setprecision(4) << R;
    }
    else if (operation == "tan") {
        double angle_rad = toRadians(x, useRadians);
        if (cos(angle_rad) == 0) {
            cout << "Ошибка: тангенс не определен для угла " << x << getAngleUnit(useRadians) << "!";
        }
        else {
            R = sin(angle_rad) / cos(angle_rad);
            cout << "tan(" << x << getAngleUnit(useRadians) << ") = " << fixed << setprecision(4) << R;
        }
    }
    else if (operation == "ctg") {
        double angle_rad = toRadians(x, useRadians);
        if (sin(angle_rad) == 0) {
            cout << "Ошибка: котангенс не определен для угла " << x << getAngleUnit(useRadians) << "!";
        }
        else {
            R = cos(angle_rad) / sin(angle_rad);
            cout << "ctg(" << x << getAngleUnit(useRadians) << ") = " << fixed << setprecision(4) << R;
        }
    }
    else {
        cout << "Ошибка: неизвестная операция!";
    }

    return 0;
}
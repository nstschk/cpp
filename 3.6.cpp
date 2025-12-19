#include <iostream>
#include <string>
using namespace std;

class BankAccount {
private:
    string accountNumber;
    string ownerName;
    double balance;
    static int totalAccounts;           // Общее количество счетов
    static double totalBankBalance;     // Общий баланс банка
    const double MIN_BALANCE = 10.0;    // Минимальный баланс

public:
    BankAccount(const string& a, const string& o, double b) : 
        accountNumber(a),
        ownerName(o),
        balance(b) {
        totalAccounts++;
        totalBankBalance += balance;
    }
    ~BankAccount() {
        totalAccounts--;
        totalBankBalance -= balance;
    }

    void deposit(double amount) {
    if (amount < 0) {
        cout << "Сумма пополнения должна быть больше 0" << endl;
        return;
    }
    balance += amount;
    totalBankBalance += amount;
}
    void withdraw(double amount) {
        if (amount < 0) {
            cout << "Сумма снятия должна быть больше 0" << endl;
            return;
        }
        if (balance < amount) {
            cout << "Нехватка средств, остаточный баланс: " << balance << endl;
            return;
        }
        if (totalBankBalance - amount < MIN_BALANCE) {
            cout << "Невозможно вывести срества, сумма меньше минимального баланса банка>: " << balance << endl;
            return;
        }
        balance -= amount;
        totalBankBalance -= amount;
    }
    void displayAccountInfo() const {cout << "Номер аккаунта: " << accountNumber << " Имя пользователя: " << ownerName <<" Баланс: " << balance << " руб." << endl;
    }
 
    static int getTotalAccounts() {
        return totalAccounts;
    }

    
    static double getTotalBankBalance() {
        return totalBankBalance;
    }

    
    static double getAverageBalance() {
        return totalBankBalance / totalAccounts;
    }
};

int BankAccount::totalAccounts = 0;
double BankAccount::totalBankBalance = 0.0;

int main() {
    setlocale(LC_ALL, "rus");
    BankAccount account1("1", "Иванов Иван", 1000.0);
    BankAccount account2("2", "Петров Петр", 500.0);
    cout << "Всего счетов: " << BankAccount::getTotalAccounts() << endl;
    cout << "Общий баланс банка: " << BankAccount::getTotalBankBalance() << " руб." << endl;
    cout << "Средний баланс: " << BankAccount::getAverageBalance() << " руб." << endl;
    cout << endl;
    string result = account1.getTotalAccounts() == account2.getTotalAccounts() ? "Статические переменные равны" : "Ошибка";
    cout << result << endl;

    return 0;
}
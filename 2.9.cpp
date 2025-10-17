#include <iostream>
#include <string>

using namespace std;

struct Account {
    int number;     
    double balance; 
};


void deposit(Account* acc, double amount) {
    if (acc == nullptr) {
        cout << "Передан нулевой указатель" << endl;
        return;
    }

    if (amount <= 0) {
        cout << "Сумма для пополнения должна быть положительной!" << endl;
        return;
    }

    acc->balance += amount;
    cout << "Счет №" << acc->number << " пополнен на " << amount << ". Новый баланс: " << acc->balance << endl;
}


void withdraw(Account& acc, double amount) {
    if (amount <= 0) {
        cout << "Сумма для снятия должна быть положительной" << endl;
        return;
    }

    if (acc.balance >= amount) {
        acc.balance -= amount;
        cout << "Со счета №" << acc.number << " снято " << amount << ". Новый баланс: " << acc.balance << endl;
    }
    else {
        cout << "На счете №" << acc.number
            << " недостаточно средств. Текущий баланс: " << acc.balance << endl;
    }
}


void printAccountInfo(const Account& acc) {
    cout << "Информация о счете" << endl;
    cout << "Номер счета: " << acc.number << endl;
    cout << "Баланс: " << acc.balance << endl;
}

int main() {
    setlocale(LC_ALL, "rus");
    Account myAccount;
    myAccount.number = 123456;
    myAccount.balance = 1000.0;

    

    printAccountInfo(myAccount);
    cout << endl;

    
    cout << "Пополняем" << endl;
    deposit(&myAccount, 500.0);  
    cout << endl;

    
    cout << "Снимаем" << endl;
    withdraw(myAccount, 200.0);  
    cout << endl;

   
    cout << "Пытаемся снять слишком большую сумму" << endl;
    withdraw(myAccount, 2000.0);
    cout << endl;


    cout << "Пополняем" << endl;
    deposit(&myAccount, 1000.0);
    cout << endl;


    cout << "Текущее состояние счета" << endl;
    printAccountInfo(myAccount);



    return 0;
}

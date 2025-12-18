#include <iostream>
#include <string>
using namespace std;

class Vehicle {
protected:
    string brand;
    string model;
    int year;
    double fuelLevel;

public:
    Vehicle(string b, string m, int y) : brand(b), model(m), year(y), fuelLevel(100) {}
    virtual ~Vehicle() {}
    virtual void startEngine() const = 0;
    virtual void stopEngine() const = 0;
    virtual void refuel(double amount) = 0;
    virtual void displayInfo() const {
        cout << brand << " " << model << " (" << year << ")" << endl;
        cout << "Уровень топлива: " << fuelLevel << "%" << endl;
    }

    double getFuelLevel() const { return fuelLevel; }
    void setFuelLevel(double level) {
        if (level >= 0 && level <= 100) fuelLevel = level;
    }
};

class Car : public Vehicle {
private:
    int doors;
    string transmission;

public:
    Car(string b, string m, int y, int d, string trans)
        : Vehicle(b, m, y), doors(d), transmission(trans) {}
    void startEngine() const override {
        cout << "Автомобиль " << brand <<" " << model<< " заводится с ключа" << endl;
    }

    void stopEngine() const override {
        cout << "Автомобиль " << brand << " " << model << " останавливается" << endl;
    }

    void refuel(double amount) override {
        cout << "Автомобиль " << brand << " с уровнем топлива "
            << fuelLevel << " %" << " Заправился на: " << amount << " руб." << endl;
        }
};

class Motorcycle : public Vehicle {
private:
    bool hasFairing;

public:
    Motorcycle(string b, string m, int y, bool fairing)
        : Vehicle(b, m, y), hasFairing(fairing) {}

    // TODO: Реализовать чисто виртуальные методы
    void startEngine() const override {
        cout << "Мотоцикл " << brand << " " << model << " заводится с ключа" << endl;
    }

    void stopEngine() const override {
        cout << "Мотоцикл " << brand << " " << model << " останавливается" << endl;
    }
    void wheelie() {
        cout << brand << " " << model << " делает вилли!" << endl;
    }
    void refuel(double amount) override {
        cout << "Мотоцикл " << brand << " с уровнем топлива "
            << fuelLevel << " %" << " Заправился на: " << amount << " руб." << endl;
    }
};

int main() {
    // TODO: Создать массив указателей на Vehicle
    // Добавить объекты Car и Motorcycle
    // Продемонстрировать полиморфное поведение
        setlocale(LC_ALL, "rus");
        const int numVehicles = 2;
        Vehicle* garage[numVehicles];


        garage[0] = new Car("Toyota", "Camry", 2022, 4, "автоматическая");
        garage[1] = new Motorcycle("Harley-Davidson", "Sportster", 2021, true);
        for (int i = 0; i < numVehicles; i++) {
            cout << "Транспортное средство " << i + 1 << endl;
            garage[i]->displayInfo();
            garage[i]->startEngine();
            garage[i]->refuel(1000);

            garage[i]->stopEngine();

        }

     
        for (int i = 0; i < numVehicles; i++) {
            delete garage[i];
        }

        return 0;

        
 
}
#include <iostream>
#include <string>
using namespace std;

class Person {
protected:
    string name;
    int age;
    string address;

public:
    Person(string n, int a, string addr) : name(n), age(a), address(addr) {}
    virtual void displayInfo() const {
        cout << "Имя: " << name << ", Возраст: " << age << ", Адрес: " << address << endl;
    }
    virtual ~Person() {}
};

class Student : public Person {
private:
    string studentId;
    double averageGrade;

public:
    Student(string n, int a, string addr, string id, double avg)
        :Person(n, a, addr), studentId(id), averageGrade(avg) {
            {
                SetAverageGrade(avg);
            }
    }
    void displayInfo() const override {
        Person::displayInfo();
        cout << "ID студента: " << studentId << ", Средний балл: " << averageGrade << endl;
    }
    double getAverageGrade() const {
        return averageGrade;
    }
    bool SetAverageGrade(double averageGrade) {
        if (averageGrade >= 1 && averageGrade <= 5) {
            cout << "Допустимый средний балл" << endl;
            return true;
        }
        else {
            cout << "Недопустимый средний балл" << endl;
            return false;
        }
        Student::averageGrade = averageGrade;
    }
};

class Professor : public Person {
private:
    string department;
    double salary;
    int yearsOfExperience;

public:
    Professor(string n, int a, string addr, string dep, double sum, int year)
        :Person(n, a, addr), department(dep), salary(sum), yearsOfExperience(year) {}
    void displayInfo() const override {
        Person::displayInfo();
        cout << "Кафедра: " << department << ", Зарплата: " << salary << ", Кол-во лет стажа: "<< yearsOfExperience << endl;
    }
    double nadbavka() {
        double sum = 0.0;
        if (yearsOfExperience > 20) {
            sum = salary * 0.25;
        }
        else if (yearsOfExperience > 15) {
            sum = salary * 0.20;
        }
        else if (yearsOfExperience > 10) {
            sum = salary * 0.15;
        }
        else if (yearsOfExperience > 5) {
            sum = salary * 0.10;
        }
        else if (yearsOfExperience > 2) {
            sum = salary * 0.05;
        }
        return sum;
    }
    double totalSalary() {
        return salary + nadbavka();
    }
};

int main() {
    setlocale(LC_ALL, "rus");
    // Тестирование иерархии классов
    Student student("Петр Петров", 20, "ул. Студенческая, 15", "S12345", 4.3);
    Professor prof("Доктор Иванов", 45, "ул. Академическая, 10", "Компьютерные науки", 50000, 15);

    student.displayInfo();
    prof.displayInfo();
    cout << "Надбавка за стаж: " << prof.nadbavka() << endl;
    cout << "Итоговая зарплата: " << prof.totalSalary() << endl;
    return 0;
}
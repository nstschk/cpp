#include <iostream>
#include <string>
using namespace std;

class Student {
private:
    string name;
    int age;
    double averageGrade;
    int grades[5];

public:
    Student(string StudentName, int StudentAge)
    {
        name = StudentName;
        age = StudentAge;
        averageGrade = 0.0;
        for (int i = 0; i < 5; i++) {
            grades[i] = 0;
        }
    }
    string getStudentName() const {
        return name;
    }
    int getStudentAge() const {
        return age;
    }

    double getavg() const {
        return averageGrade;
    }

    int getGrade(int index) const {
        if (index >= 0 && index < 5) {
            return grades[index];
        }
        return -1;
    }
    void setStudentName(string newName) {
        if (!newName.empty()) {
            name = newName;
        }
    }
    void setStudentAge(int newAge) {
        if (newAge >= 10 && newAge <= 100) {
            age = newAge;
        }
    }
    void setGrade(int index, int grade) {
        if (index >= 0 && index < 5 && grade >= 1 && grade <= 5) {
            grades[index] = grade;
            CalculateAverageGrade(); 
        }
    }
    void CalculateAverageGrade() {
        double sum = 0;
        for (int i = 0; i < 5; i++) {
            sum += grades[i];
        }
        averageGrade = sum / 5;
    }
    void displayInfo() const {
        cout << "Имя, Фамилия: " << name << endl;
        cout << "Возраст: " << age << endl;
        cout << "Средний балл: " << averageGrade << endl;
        cout << "Оценки: ";
        for (int i = 0; i < 5; i++) {
            cout << grades[i] << " ";
        }
        cout << endl;
    }
    bool hasScholarship() const {
        return averageGrade >= 4.5;
    }
    // TODO: Создать конструктор с параметрами
    // TODO: Реализовать геттеры и сеттеры с валидацией
    // TODO: Метод для расчета среднего балла
    // TODO: Метод для вывода информации о студенте
    // TODO: Метод для проверки стипендии (средний балл >= 4.5)
};

int main() {
    setlocale(LC_ALL, "rus");
    // Тестирование класса Student
    Student student("Иван Иванов", 20);

    // Установка оценок
    student.setGrade(0, 5);
    student.setGrade(1, 4);
    student.setGrade(2, 5);
    student.setGrade(3, 3);
    student.setGrade(4, 4);

    student.displayInfo();

    if (student.hasScholarship()) {
        cout << "Студент получает стипендию" << endl;
    }
    else {
        cout << "Стипендия не назначена" << endl;
    }

    return 0;
}
#include <iostream>
#include <string>

using namespace std;


struct Date {
    int day;
    int month;
    int year;
};


struct Student {
    string fio;
    Date birthdate;
    int grades[5];
};


void printStudent(const Student& s) {
    cout << "ФИО: " << s.fio << endl;
    cout << "Дата рождения: " << s.birthdate.day << "."
        << s.birthdate.month << "." << s.birthdate.year << endl;
    cout << "Оценки: ";
    for (int i = 0; i < 5; i++) {
        cout << s.grades[i] << " ";
    }
    cout << endl;
}


double getAverageRating(const Student& s) {
    double sum = 0;
    for (int i = 0; i < 5; i++) {
        sum += s.grades[i];
    }
    return sum / 5.0;
}

int main() {
    setlocale(LC_ALL, "rus");
    Student students[4] = {
        {"Петров Максим Иванович", {21, 7, 2003}, {3, 3, 3, 4, 3}},
        {"Сидорова Анна Сергеевна", {10, 11, 2003}, {5, 5, 5, 4, 5}},
        {"Пупырка Дмитрий Владимирович", {5, 1, 2003}, {5, 5, 5, 5, 5}},
        {"Анисова Екатерина Андреевна", {25, 12, 2002}, {5, 2, 3, 4, 3}}
    };


    for (int i = 0; i < 4; i++) {
        cout << "Студент №" << i + 1 << ":" << endl;
        printStudent(students[i]);
        double avg = getAverageRating(students[i]);
        cout << "Средний балл = " << avg << endl;
    }


    cout << "Студенты со средним баллом выше 4.0" << endl;
    int count = 0; 
    for (int i = 0; i < 4; i++) {
        double avg = getAverageRating(students[i]);
        if (avg > 4.0) {
            cout << "Студент №" << i + 1 << ":" << endl;
            printStudent(students[i]);
            cout << "Средний балл = " << avg << endl;
            count++; 
        }
    }

    
    if (count == 0) {
        cout << "Нет студентов со средним баллом выше 4.0" << endl;
    }


    return 0;
}
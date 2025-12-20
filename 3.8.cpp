#include <iostream>
#include <string>
using namespace std;

class Person {
protected:
    string name;
    int age;

public:
    Person(string n, int a) : name(n), age(a) {}
    virtual void display() const {
        cout << "Имя: " << name << ", Возраст: " << age << endl;
    }
};

class Employee {
protected:
    string position;
    double salary;

public:
    Employee(string pos, double sal) : position(pos), salary(sal) {}
    virtual void work() const {
        cout << "Выполняет работу как сотрудник" << endl;
    }
};

class Teacher : public Person, public Employee {
private:
    string subject;
    int experienceYears;

public:
    Teacher(
        const string& n,
        int a,
        const string& pos,
        double sal,
        const string& subject,
        int experienceYears
    ) : Person(n, a),
        Employee(pos, sal),
        subject(subject),
        experienceYears(experienceYears) {}
    void display() const override {
        cout << "Имя: " << name << " Возраст: " << age
            << " Предмет: " << subject
            << " Опыт: " << experienceYears << " лет" << endl;
    }

    void work() const override {
        cout << "Преподает предмет: " << subject << endl;
    }

    void prepareLesson() const {
        cout << "Готовит урок по предмету: " << subject << endl;
    }

    void gradeStudents() const {
        cout << "Выставляет оценки ученикам" << endl;
    }
};

class Researcher {
protected:
    string researchArea;
    int publicationsCount;

public:
    Researcher(string area, int publications)
        : researchArea(area), publicationsCount(publications) {}

    void conductResearch() const {
        cout << "Проводит исследования в области: " << researchArea << endl;
    }
};

class Professor : public Teacher, public Researcher {
public:
        Professor(
            const string& n,
            int a,
            const string& pos,
            double sal,
            const string& subject,
            int experienceYears,
            const string& area,
            int publications
        ) : Teacher(n, a, pos, sal, subject, experienceYears),
            Researcher(area, publications) {}

        void display() const override {
            Teacher::display();
            cout << "Имя: " << name << " Возраст: " << age << " Область исследования: " << researchArea << " Публикации: " << publicationsCount << endl;
        }

        void work() const override {
            cout << "Выполняет исследование в области" << researchArea<< endl;
        }
 };


int main() {
    setlocale(LC_ALL, "rus");
    Teacher teacher = Teacher("Агафья", 49, "учитель", 100000, "математика", 5);
    Professor professor = Professor("Василина", 55, "профессор", 1500000, "матемтика", 15, "ТФКП", 15);
    cout << "Вызов методов учителя: " << endl;
    teacher.display();
    teacher.work();
    teacher.prepareLesson();
    teacher.gradeStudents();
    cout << endl;

    cout << "Вызов методов профессора: " << endl;
    professor.display();
    professor.work();
    professor.prepareLesson();
    professor.gradeStudents();
    professor.conductResearch();

    return 0;

    return 0;
}
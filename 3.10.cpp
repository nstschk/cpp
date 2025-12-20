#include <iostream>
#include <string>
#include <vector>
#include <memory>
using namespace std;

class UniversityMember {
protected:
    string name;
    string id;
    string email;

public:
    UniversityMember(string n, string i, string e) : name(n), id(i), email(e) {}
    virtual ~UniversityMember() {}

    virtual void displayInfo() const = 0;

    virtual void work() const = 0;

    virtual string getRole() const = 0;

    virtual string getName() const = 0;

    virtual string getID() const = 0;
};

class Student : public UniversityMember {
private:
    string major;
    int year;
    double gpa;
    vector<string> courses;

public:
    Student(string n, string i, string e, string m, int y, double g)
        : UniversityMember(n, i, e), major(m), year(y), gpa(g) {}

    void displayInfo() const override {
        cout << "Студент: " << name << " ID: " << id << " Email: " << email << " Специальность: " << major << " Курс: " << year << " GPA: " << gpa << endl;
    }

    void work() const override {
        cout << "Студент " << name << " учится и посещает занятия" << endl;
    }

    string getRole() const override {
        return "Student";
    }

    string getName() const override {
        return name;
    }

    string getID() const override {
        return id;
    }

    void enrollCourse(const string& course) {
        courses.push_back(course);
    }

    void dropCourse(const string& course) {
        for (auto it = courses.begin(); it != courses.end(); ++it) {
            if (*it == course) {
                courses.erase(it);
                break;
            }
        }
    }

    double calculateGPA() const {
        return gpa;
    }
};

class Professor : public UniversityMember {
private:
    string department;
    string office;
    double salary;
    vector<string> teachingCourses;

public:
    Professor(string n, string i, string e, string d, string o, double s)
        : UniversityMember(n, i, e), department(d), office(o), salary(s) {}

    void displayInfo() const override {
        cout << "Профессор: " << name << " ID: " << id << " Email: " << email << " Кафедра: " << department << " Аудитория: " << office << " ЗП: " << salary << endl;
    }

    void work() const override {
        cout << "Профессор " << name << " преподает занятия" << endl;
    }

    string getRole() const override {
        return "Professor";
    }

    string getName() const override {
        return name;
    }

    string getID() const override {
        return id;
    }

    void assignCourse(const string& course) {
        teachingCourses.push_back(course);
    }

    void removeCourse(const string& course) {
        for (auto it = teachingCourses.begin(); it != teachingCourses.end(); ++it) {
            if (*it == course) {
                teachingCourses.erase(it);
                break;
            }
        }
    }

    double calculateSalary() const {
        return salary;
    }
};

class Course {
private:
    string courseCode;
    string courseName;
    int credits;
    Professor* instructor;
    vector<Student*> enrolledStudents;

public:
    Course(string code, string name, int cred)
        : courseCode(code), courseName(name), credits(cred), instructor(nullptr) {}

    string getCourseCode() {
        return courseCode;
    }

    void addStudent(Student* student) {
        enrolledStudents.push_back(student);
    }

    void removeStudent(const string& studentId) {
        for (auto it = enrolledStudents.begin(); it != enrolledStudents.end(); ++it) {
            if ((*it)->getID() == studentId) {
                enrolledStudents.erase(it);
                break;
            }
        }
    }

    void setInstructor(Professor* prof) {
        instructor = prof;
    }

    void displayCourseInfo() const {
        cout << "Курс: " << courseCode <<  courseName
            << " Кредиты: " << credits <<  "Преподаватель: " << instructor->getName() << "Зачисленные студенты: " << enrolledStudents.size() << endl;
    }
};

class University {
private:
    string name;
    vector<unique_ptr<UniversityMember>> members;
    vector<unique_ptr<Course>> courses;
    static int totalUniversities;

public:
    University(string n) : name(n) {
        totalUniversities++;
    }

    ~University() {
        totalUniversities--;
    }

    void addMember(unique_ptr<UniversityMember> member) {
        members.push_back(move(member));
    }

    void removeMember(const string& memberId) {
        for (auto it = members.begin(); it != members.end(); ++it) {
            if ((*it)->getID() == memberId) {
                members.erase(it);
                break;
            }
        }
    }

    void addCourse(unique_ptr<Course> course) {
        courses.push_back(move(course));
    }
 
    void removeCourse(const string& courseCode) {
        for (size_t i = 0; i < courses.size(); i++) {
            if (courses[i]->getCourseCode() == courseCode) {
                courses.erase(courses.begin() + i);
                return;
            }
        }
    }

    static int getTotalUniversities() {
        return totalUniversities;
    }

    void displayAllMembers() const {
        for (const auto& member : members) {
            member->displayInfo();
            cout << endl;
        }
    }
};
int University::totalUniversities = 0;

int main() {

    setlocale(LC_ALL, "rus");
    University uni("Технический Университет");

    // Создание студентов
    auto student1 = make_unique<Student>("Иван Петров", "S001", "ivan@uni.ru", "Информатика", 2, 4.5);
    auto student2 = make_unique<Student>("Мария Сидорова", "S002", "maria@uni.ru", "Математика", 3, 4.7);

    // Создание преподавателей
    auto prof1 = make_unique<Professor>("Дмитрий Иванов", "P001", "dmitry@uni.ru", "Кафедра информатики", "101", 80000);
    auto prof2 = make_unique<Professor>("Ольга Смирнова", "P002", "olga@uni.ru", "Кафедра математики", "202", 75000);

    // Добавление членов университета
    uni.addMember(move(student1));
    uni.addMember(move(student2));
    uni.addMember(move(prof1));
    uni.addMember(move(prof2));

    // Создание курсов
    auto course1 = make_unique<Course>("CS101", "Программирование", 5);
    auto course2 = make_unique<Course>("MATH201", "Алгебра", 4);

    // Добавление курсов
    uni.addCourse(move(course1));
    uni.addCourse(move(course2));

    // Демонстрация всех членов университета
    cout << "Все члены университета:" << endl;
    uni.displayAllMembers();

    // Удаление члена университета
    uni.removeMember("S001");

    // Удаление курса
    uni.removeCourse("MATH201");

    // Статистика университетов
    cout << "Всего университетов: " << University::getTotalUniversities() << endl;

    return 0;
}
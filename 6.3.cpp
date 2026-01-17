#include <stdio.h>
#include "sqlite3.h" 
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <map>

using namespace std;

struct Student {
    int id;
    string name;
    string email;
    string group;
};

struct Grade {
    string subject;
    int grade;
};

class DatabaseManager {
private:
    sqlite3* db;

public:
    DatabaseManager() : db(nullptr) {}

    ~DatabaseManager() {
        if (db) {
            sqlite3_close(db);
        }
    }

    bool initialize(const string& filename) {
        if (sqlite3_open(filename.c_str(), &db) != SQLITE_OK) {
            return false;
        }

        execute("PRAGMA foreign_keys = ON;");
        execute("PRAGMA journal_mode = WAL;");

        const char* sql = R"(
            CREATE TABLE IF NOT EXISTS students (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL,
                email TEXT UNIQUE NOT NULL,
                group_name TEXT NOT NULL
            );
            
            CREATE TABLE IF NOT EXISTS grades (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                student_id INTEGER NOT NULL,
                subject TEXT NOT NULL,
                grade INTEGER NOT NULL,
                FOREIGN KEY(student_id) REFERENCES students(id) ON DELETE CASCADE
            );
        )";

        return execute(sql);
    }

    bool execute(const string& sql) {
        return sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
    }

    sqlite3* getHandle() const { return db; }
};

class StudentRepository {
private:
    sqlite3* db;

    bool beginTransaction() {
        return sqlite3_exec(db, "BEGIN TRANSACTION;", nullptr, nullptr, nullptr) == SQLITE_OK;
    }

    bool commitTransaction() {
        return sqlite3_exec(db, "COMMIT;", nullptr, nullptr, nullptr) == SQLITE_OK;
    }

    bool rollbackTransaction() {
        return sqlite3_exec(db, "ROLLBACK;", nullptr, nullptr, nullptr) == SQLITE_OK;
    }

public:
    StudentRepository(DatabaseManager& dbManager) : db(dbManager.getHandle()) {}

    // 1. Добавление студента с оценками в одной транзакции
    bool addStudentWithGrades(const string& name, const string& email,
        const string& group, const vector<Grade>& grades) {
        // Начинаем транзакцию
        if (!beginTransaction()) {
            return false;
        }

        // Вставляем студента
        const string sqlStudent = "INSERT INTO students (name, email, group_name) VALUES (?, ?, ?)";
        sqlite3_stmt* stmtStudent;

        if (sqlite3_prepare_v2(db, sqlStudent.c_str(), -1, &stmtStudent, nullptr) != SQLITE_OK) {
            rollbackTransaction();
            return false;
        }

        sqlite3_bind_text(stmtStudent, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmtStudent, 2, email.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmtStudent, 3, group.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmtStudent) != SQLITE_DONE) {
            sqlite3_finalize(stmtStudent);
            rollbackTransaction();
            return false;
        }

        // Получаем ID нового студента
        int studentId = sqlite3_last_insert_rowid(db);
        sqlite3_finalize(stmtStudent);

        // Вставляем оценки
        const string sqlGrade = "INSERT INTO grades (student_id, subject, grade) VALUES (?, ?, ?)";

        for (const auto& grade : grades) {
            sqlite3_stmt* stmtGrade;

            if (sqlite3_prepare_v2(db, sqlGrade.c_str(), -1, &stmtGrade, nullptr) != SQLITE_OK) {
                rollbackTransaction();
                return false;
            }

            sqlite3_bind_int(stmtGrade, 1, studentId);
            sqlite3_bind_text(stmtGrade, 2, grade.subject.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_int(stmtGrade, 3, grade.grade);

            if (sqlite3_step(stmtGrade) != SQLITE_DONE) {
                sqlite3_finalize(stmtGrade);
                rollbackTransaction();
                return false;
            }

            sqlite3_finalize(stmtGrade);
        }

        // Коммитим транзакцию
        if (!commitTransaction()) {
            rollbackTransaction();
            return false;
        }

        return true;
    }

    // 2.1 Получение студентов по группе
    vector<Student> getStudentsByGroup(const string& groupName) {
        const string sql = "SELECT id, name, email, group_name FROM students WHERE group_name = ?";
        vector<Student> students;
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return students;
        }

        sqlite3_bind_text(stmt, 1, groupName.c_str(), -1, SQLITE_TRANSIENT);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Student student;
            student.id = sqlite3_column_int(stmt, 0);
            student.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            student.email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            student.group = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
            students.push_back(student);
        }

        sqlite3_finalize(stmt);
        return students;
    }

    // 2.2 Средняя оценка по предмету
    optional<double> getAverageGradeBySubject(const string& subject) {
        const string sql = "SELECT AVG(grade) FROM grades WHERE subject = ?";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return nullopt;
        }

        sqlite3_bind_text(stmt, 1, subject.c_str(), -1, SQLITE_TRANSIENT);

        optional<double> result = nullopt;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            result = sqlite3_column_double(stmt, 0);
        }

        sqlite3_finalize(stmt);
        return result;
    }

    // 2.3 Топ студентов по среднему баллу (JOIN запрос)
    vector<pair<Student, double>> getTopStudents(int limit) {
        const string sql = R"(
            SELECT s.id, s.name, s.email, s.group_name, AVG(g.grade) as avg_grade
            FROM students s
            JOIN grades g ON s.id = g.student_id
            GROUP BY s.id
            ORDER BY avg_grade DESC
            LIMIT ?
        )";

        vector<pair<Student, double>> topStudents;
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return topStudents;
        }

        sqlite3_bind_int(stmt, 1, limit);

        while (sqlite3_step(stmt) == SQLITE_ROW) {
            Student student;
            student.id = sqlite3_column_int(stmt, 0);
            student.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            student.email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            student.group = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

            double avgGrade = sqlite3_column_double(stmt, 4);

            topStudents.push_back({ student, avgGrade });
        }

        sqlite3_finalize(stmt);
        return topStudents;
    }
};

int main() {
    setlocale(LC_ALL, "Russian");
    DatabaseManager dbManager;
    if (!dbManager.initialize("test.db")) {
        cout << "Ошибка инициализации БД" << endl;
        system("pause");
        return 1;
    }

    StudentRepository repo(dbManager);
    vector<Grade> grades = { {"Math", 85}, {"Physics", 90}, {"Programming", 95} };
    repo.addStudentWithGrades("Петр Петров", "petr@university.ru", "CS-101", grades);
    vector<Grade> grades2 = { {"Math", 70}, {"Physics", 80}, {"Programming", 85} };
    repo.addStudentWithGrades("Мария Иванова", "maria@university.ru", "CS-101", grades2);

    vector<Grade> grades3 = { {"Math", 90}, {"Physics", 95}, {"Programming", 92} };
    repo.addStudentWithGrades("Алексей Сидоров", "alex@university.ru", "CS-102", grades3);

    cout << "\n Студенты группы CS-101:" << endl;
    auto cs101Students = repo.getStudentsByGroup("CS-101");
    for (const auto& student : cs101Students) {
        cout << "   - " << student.name << " (" << student.email << ")" << endl;
    }

    // 4. Средняя оценка по предмету
    cout << "\n Средние оценки по предметам:" << endl;
    auto avgMath = repo.getAverageGradeBySubject("Math");
    if (avgMath) {
        cout << "   Math: " << *avgMath << endl;
    }

    auto avgPhysics = repo.getAverageGradeBySubject("Physics");
    if (avgPhysics) {
        cout << "   Physics: " << *avgPhysics << endl;
    }

    // 5. Топ студентов
    cout << "\ Топ 2 студента группы:" << endl;
    auto topStudents = repo.getTopStudents(2);
    int rank = 1;
    for (const auto& [student, avgGrade] : topStudents) {
        cout << "   " << rank << ". " << student.name << " (" << student.group
            << ") - средний балл: " << avgGrade << endl;
        rank++;
    }
    system("pause");
    return 0;
}
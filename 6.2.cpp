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

        const char* sql = R"(
            CREATE TABLE IF NOT EXISTS students (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL,
                email TEXT UNIQUE NOT NULL,
                group_name TEXT NOT NULL
            );
        )";

        execute(sql);
        return true;
    }

    bool execute(const string& sql) {
        return sqlite3_exec(db, sql.c_str(), nullptr, nullptr, nullptr) == SQLITE_OK;
    }

    sqlite3* getHandle() const { return db; }
};

class StudentRepository {
private:
    sqlite3* db;

    bool isEmailUnique(const string& email, int excludeId = -1) {
        const string sql = "SELECT COUNT(*) FROM students WHERE email = ? AND id != ?";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return false;
        }

        sqlite3_bind_text(stmt, 1, email.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 2, excludeId);

        bool unique = false;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            unique = sqlite3_column_int(stmt, 0) == 0;
        }

        sqlite3_finalize(stmt);
        return unique;
    }

public:
    StudentRepository(DatabaseManager& dbManager) : db(dbManager.getHandle()) {}

    optional<Student> addStudent(const string& name, const string& email, const string& group) {
        if (!isEmailUnique(email)) {
            return nullopt;
        }

        const string sql = "INSERT INTO students (name, email, group_name) VALUES (?, ?, ?)";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return nullopt;
        }

        sqlite3_bind_text(stmt, 1, name.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, email.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, group.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmt) != SQLITE_DONE) {
            sqlite3_finalize(stmt);
            return nullopt;
        }

        int studentId = sqlite3_last_insert_rowid(db);
        sqlite3_finalize(stmt);

        return getStudent(studentId);
    }

    optional<Student> getStudent(int id) {
        const string sql = "SELECT id, name, email, group_name FROM students WHERE id = ?";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return nullopt;
        }

        sqlite3_bind_int(stmt, 1, id);

        if (sqlite3_step(stmt) == SQLITE_ROW) {
            Student student;
            student.id = sqlite3_column_int(stmt, 0);
            student.name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            student.email = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            student.group = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));

            sqlite3_finalize(stmt);
            return student;
        }

        sqlite3_finalize(stmt);
        return nullopt;
    }

    bool updateStudent(int id, const string& newName, const string& newEmail, const string& newGroup) {
        if (!isEmailUnique(newEmail, id)) {
            return false;
        }

        const string sql = "UPDATE students SET name = ?, email = ?, group_name = ? WHERE id = ?";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return false;
        }

        sqlite3_bind_text(stmt, 1, newName.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 2, newEmail.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmt, 3, newGroup.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmt, 4, id);

        bool success = sqlite3_step(stmt) == SQLITE_DONE;
        sqlite3_finalize(stmt);

        return success;
    }

    bool deleteStudent(int id) {
        const string sql = "DELETE FROM students WHERE id = ?";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return false;
        }

        sqlite3_bind_int(stmt, 1, id);

        bool success = sqlite3_step(stmt) == SQLITE_DONE;
        sqlite3_finalize(stmt);

        return success;
    }

    vector<Student> getAllStudents() {
        const string sql = "SELECT id, name, email, group_name FROM students";
        vector<Student> students;
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return students;
        }

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
};

int main() {

        setlocale(LC_ALL, "Russian");
        DatabaseManager dbManager;


        if (!dbManager.initialize("test.db")) {
            cout << "ОШИБКА: Не удалось подключиться к БД!" << endl;  
            system("pause");
            return 1;
        }

        cout << "БД подключена успешно!" << endl; 

        StudentRepository repo(dbManager);

        cout << "Добавляю студента..." << endl;  
        repo.addStudent("Иван Иванов", "ivan@university.ru", "CS-101");

        auto student = repo.getStudent(1);

        if (student) {
            cout << "\n=== НАЙДЕН СТУДЕНТ ===" << endl;
            cout << "  ID: " << student->id << endl;
            cout << "  Имя: " << student->name << endl;
            cout << "  Email: " << student->email << endl;
            cout << "  Группа: " << student->group << endl;
        }
        else {
            cout << "\nСтудент не найден" << endl;
        }

        system("pause");
        return 0;
    }

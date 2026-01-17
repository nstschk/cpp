#include <stdio.h>
#include "sqlite3.h" 
#include <iostream>
#include <string>
using namespace std;

class DatabaseManager {
private:
    sqlite3* db;
    string lastError;

public:
    DatabaseManager() : db(nullptr) {}

    bool connect(const string& filename) {
        int result = sqlite3_open(filename.c_str(), &db);
        if (result != SQLITE_OK) {
            lastError = sqlite3_errmsg(db);
            sqlite3_close(db);
            db = nullptr;
            return false;
        }

        // Включение поддержки внешних ключей
        execute("PRAGMA foreign_keys = ON;");

        // Включаем WAL (Write-Ahead Logging) для лучшей параллельности
        execute("PRAGMA journal_mode = WAL;");

        // Увеличиваем размер кэша
        execute("PRAGMA cache_size = -64000;"); // 64MB

        // Включаем синхронизацию только в критических секциях
        execute("PRAGMA synchronous = NORMAL;");

        // Увеличиваем таймаут для занятой базы
        sqlite3_busy_timeout(db, 5000); // 5 секунд
        return true;
    }

    ~DatabaseManager() {
        if (db) {
            sqlite3_close(db);
        }
    }

    bool createTables() {
        const char* sql = R"(
        CREATE TABLE IF NOT EXISTS students (
        id INTEGER PRIMARY KEY AUTOINCREMENT, 
        name TEXT NOT NULL, 
        email TEXT UNIQUE, 
        group_name TEXT
        );
        
        CREATE TABLE IF NOT EXISTS grades (
        id INTEGER PRIMARY KEY AUTOINCREMENT, 
        student_id INTEGER, 
        subject TEXT, 
        grade INTEGER, 
        FOREIGN KEY(student_id) REFERENCES students(id) ON DELETE CASCADE);
    )";

        char* errorMessage = nullptr;
        int result = sqlite3_exec(db, sql, nullptr, nullptr, &errorMessage);

        if (result != SQLITE_OK) {
            lastError = errorMessage;
            sqlite3_free(errorMessage);
            return false;
        }

        return true;
    
    }
    bool execute(const string& sql) {
        
            if (!db) {
                lastError = "Database not connected";
                return false;
            }
        char* errorMessage = nullptr;
        int result = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage);

        if (result != SQLITE_OK) {
            lastError = errorMessage;
            sqlite3_free(errorMessage);
            return false;
        }

        return true;
    }
    string getLastError() const {
        return lastError;
    }
};

// Пример использования
int main() {
    DatabaseManager databaseManager;

    if (!databaseManager.connect("newtest.db")) {
        cout << "Failed to initialize database" << endl;
        return 1;
    }

    if (!databaseManager.createTables()) {
        cout << "Failed to create tables" << endl;
        return 1;
    }

    // Создаем студента
    databaseManager.execute(
        "INSERT INTO students (name, email, group_name) "
        "VALUES ('Ivan P', 'ivan@example.com', 'CS-101');"
    );

    // Создаем оценку для студента
    databaseManager.execute(
        "INSERT INTO grades (student_id, subject, grade) "
        "VALUES (1, 'Math', 5);"
    );

}
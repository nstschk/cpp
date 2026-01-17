#include <stdio.h>
#include "sqlite3.h" 
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <map>
#include <regex>
#include <stdexcept>

using namespace std;

// Класс для валидации входных данных
class InputValidator {
private:
    // Регулярные выражения для проверки
    static const regex emailRegex;
    static const regex safeInputRegex;

public:
    // Проверка email
    static bool isValidEmail(const string& email) {
        return regex_match(email, emailRegex);
    }

    // Проверка на SQL-инъекции и опасные символы
    static bool isSafeInput(const string& input) {
        // Проверяем на опасные SQL-символы
        if (input.find(';') != string::npos ||
            input.find('\'') != string::npos ||
            input.find('\"') != string::npos ||
            input.find('\\') != string::npos ||
            input.find('*') != string::npos ||
            input.find('/') != string::npos ||
            input.find("DROP") != string::npos ||
            input.find("DELETE") != string::npos ||
            input.find("UPDATE") != string::npos ||
            input.find("INSERT") != string::npos ||
            input.find("SELECT") != string::npos) {
            return false;
        }
        return true;
    }

    // Проверка длины строки
    static bool isValidLength(const string& input, size_t minLen, size_t maxLen) {
        return input.length() >= minLen && input.length() <= maxLen;
    }

    // Проверка диапазона оценки
    static bool isValidGrade(int grade) {
        return grade >= 0 && grade <= 100;
    }

    // Проверка группы (формат XX-XXX)
    static bool isValidGroup(const string& group) {
        return regex_match(group, regex("^[A-Z]{2}-\\d{3}$"));
    }

    // Полная проверка студента
    static void validateStudentData(const string& name, const string& email, const string& group) {
        // Проверка имени
        if (!isSafeInput(name)) {
            throw invalid_argument("Имя содержит опасные символы или SQL-инъекцию");
        }
        if (!isValidLength(name, 2, 100)) {
            throw invalid_argument("Имя должно быть от 2 до 100 символов");
        }

        // Проверка email
        if (!isValidEmail(email)) {
            throw invalid_argument("Неверный формат email. Пример: user@example.com");
        }
        if (!isValidLength(email, 5, 100)) {
            throw invalid_argument("Email должен быть от 5 до 100 символов");
        }

        // Проверка группы
        if (!isValidGroup(group)) {
            throw invalid_argument("Неверный формат группы. Пример: CS-101");
        }
    }

    // Проверка оценки
    static void validateGrade(const string& subject, int grade) {
        if (!isSafeInput(subject)) {
            throw invalid_argument("Название предмета содержит опасные символы");
        }
        if (!isValidLength(subject, 2, 50)) {
            throw invalid_argument("Название предмета должно быть от 2 до 50 символов");
        }
        if (!isValidGrade(grade)) {
            throw invalid_argument("Оценка должна быть в диапазоне 0-100");
        }
    }
};

// Инициализация статических регулярных выражений
const regex InputValidator::emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");
const regex InputValidator::safeInputRegex(R"(^[a-zA-Z0-9а-яА-ЯёЁ\s\-_@\.]+$)");

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
                group_name TEXT NOT NULL,
                CHECK(length(name) >= 2 AND length(name) <= 100),
                CHECK(length(email) >= 5 AND length(email) <= 100),
                CHECK(group_name GLOB '[A-Z][A-Z]-[0-9][0-9][0-9]')
            );
            
            CREATE TABLE IF NOT EXISTS grades (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                student_id INTEGER NOT NULL,
                subject TEXT NOT NULL,
                grade INTEGER NOT NULL CHECK (grade >= 0 AND grade <= 100),
                CHECK(length(subject) >= 2 AND length(subject) <= 50),
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

    // 1. Добавление студента с оценками с валидацией
    bool addStudentWithGrades(const string& name, const string& email,
        const string& group, const vector<Grade>& grades) {
        try {
            // Валидация входных данных
            InputValidator::validateStudentData(name, email, group);

            // Валидация оценок
            for (const auto& grade : grades) {
                InputValidator::validateGrade(grade.subject, grade.grade);
            }

            // Начинаем транзакцию
            if (!beginTransaction()) {
                throw runtime_error("Ошибка начала транзакции");
            }

            // Вставляем студента с подготовленным выражением
            const string sqlStudent = "INSERT INTO students (name, email, group_name) VALUES (?, ?, ?)";
            sqlite3_stmt* stmtStudent;

            if (sqlite3_prepare_v2(db, sqlStudent.c_str(), -1, &stmtStudent, nullptr) != SQLITE_OK) {
                rollbackTransaction();
                throw runtime_error("Ошибка подготовки SQL-запроса для студента");
            }

            // Безопасная привязка параметров
            sqlite3_bind_text(stmtStudent, 1, name.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmtStudent, 2, email.c_str(), -1, SQLITE_TRANSIENT);
            sqlite3_bind_text(stmtStudent, 3, group.c_str(), -1, SQLITE_TRANSIENT);

            if (sqlite3_step(stmtStudent) != SQLITE_DONE) {
                sqlite3_finalize(stmtStudent);
                rollbackTransaction();
                throw runtime_error("Ошибка добавления студента (возможно email уже существует)");
            }

            // Получаем ID нового студента
            int studentId = sqlite3_last_insert_rowid(db);
            sqlite3_finalize(stmtStudent);

            // Вставляем оценки с подготовленными выражениями
            const string sqlGrade = "INSERT INTO grades (student_id, subject, grade) VALUES (?, ?, ?)";

            for (const auto& grade : grades) {
                sqlite3_stmt* stmtGrade;

                if (sqlite3_prepare_v2(db, sqlGrade.c_str(), -1, &stmtGrade, nullptr) != SQLITE_OK) {
                    rollbackTransaction();
                    throw runtime_error("Ошибка подготовки SQL-запроса для оценки");
                }

                // Безопасная привязка параметров
                sqlite3_bind_int(stmtGrade, 1, studentId);
                sqlite3_bind_text(stmtGrade, 2, grade.subject.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_int(stmtGrade, 3, grade.grade);

                if (sqlite3_step(stmtGrade) != SQLITE_DONE) {
                    sqlite3_finalize(stmtGrade);
                    rollbackTransaction();
                    throw runtime_error("Ошибка добавления оценки");
                }

                sqlite3_finalize(stmtGrade);
            }

            // Коммитим транзакцию
            if (!commitTransaction()) {
                rollbackTransaction();
                throw runtime_error("Ошибка коммита транзакции");
            }

            return true;

        }
        catch (const exception& e) {
            cerr << "Ошибка: " << e.what() << endl;
            return false;
        }
    }

    // 2.1 Получение студентов по группе с безопасными параметрами
    vector<Student> getStudentsByGroup(const string& groupName) {
        vector<Student> students;

        // Валидация входных данных
        if (!InputValidator::isValidGroup(groupName)) {
            cerr << "Неверный формат группы: " << groupName << endl;
            return students;
        }
        if (!InputValidator::isSafeInput(groupName)) {
            cerr << "Небезопасные символы в названии группы" << endl;
            return students;
        }

        const string sql = "SELECT id, name, email, group_name FROM students WHERE group_name = ?";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            cerr << "Ошибка подготовки SQL-запроса" << endl;
            return students;
        }

        // Безопасная привязка параметра
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

    // 2.2 Средняя оценка по предмету с безопасными параметрами
    optional<double> getAverageGradeBySubject(const string& subject) {
        // Валидация входных данных
        if (!InputValidator::isSafeInput(subject)) {
            cerr << "Небезопасные символы в названии предмета" << endl;
            return nullopt;
        }
        if (!InputValidator::isValidLength(subject, 2, 50)) {
            cerr << "Недопустимая длина названия предмета" << endl;
            return nullopt;
        }

        const string sql = "SELECT AVG(grade) FROM grades WHERE subject = ?";
        sqlite3_stmt* stmt;

        if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            return nullopt;
        }

        // Безопасная привязка параметра
        sqlite3_bind_text(stmt, 1, subject.c_str(), -1, SQLITE_TRANSIENT);

        optional<double> result = nullopt;
        if (sqlite3_step(stmt) == SQLITE_ROW) {
            result = sqlite3_column_double(stmt, 0);
        }

        sqlite3_finalize(stmt);
        return result;
    }

    // 2.3 Топ студентов по среднему баллу с безопасными параметрами
    vector<pair<Student, double>> getTopStudents(int limit) {
        // Валидация входных данных
        if (limit <= 0 || limit > 100) {
            cerr << "Недопустимое значение limit: " << limit << endl;
            return {};
        }

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

        // Безопасная привязка параметра
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


    // Тест 1: Корректные данные
    cout << "\n1. Тест с корректными данными..." << endl;
    vector<Grade> grades = { {"Math", 85}, {"Physics", 90}, {"Programming", 95} };
    if (repo.addStudentWithGrades("Петр Петров", "petr1@university.ru", "CS-101", grades)) {
        cout << "   ✓ Студент добавлен успешно" << endl;
    }

    // Тест 2: Неверный email
    cout << "\n2. Тест с неверным email..." << endl;
    vector<Grade> grades2 = { {"Math", 70} };
    if (!repo.addStudentWithGrades("Иван", "неправильный-email", "CS-102", grades2)) {
        cout << "   ✓ Валидация email сработала" << endl;
    }

    // Тест 3: SQL-инъекция в имени
    cout << "\n3. Тест с SQL-инъекцией..." << endl;
    vector<Grade> grades3 = { {"Math", 80} };
    if (!repo.addStudentWithGrades("Иван'; DROP TABLE students;--", "test@mail.ru", "CS-103", grades3)) {
        cout << "   ✓ Защита от SQL-инъекций сработала" << endl;
    }

    // Тест 4: Недопустимая оценка
    cout << "\n4. Тест с недопустимой оценкой..." << endl;
    vector<Grade> grades4 = { {"Math", 150} };
    if (!repo.addStudentWithGrades("Мария", "maria@mail.ru", "CS-104", grades4)) {
        cout << "   ✓ Валидация оценки сработала" << endl;
    }

    // Тест 5: Неверный формат группы
    cout << "\n5. Тест с неверным форматом группы..." << endl;
    vector<Grade> grades5 = { {"Math", 85} };
    if (!repo.addStudentWithGrades("Алексей", "alex@mail.ru", "НЕПРАВИЛЬНАЯ", grades5)) {
        cout << "   ✓ Валидация группы сработала" << endl;
    }

    // Тест 6: Получение студентов по группе
    cout << "\n6. Получение студентов группы CS-101:" << endl;
    auto cs101Students = repo.getStudentsByGroup("CS-101");
    cout << "   Найдено студентов: " << cs101Students.size() << endl;

    // Тест 7: Средняя оценка
    cout << "\n7. Средняя оценка по Math:" << endl;
    auto avgMath = repo.getAverageGradeBySubject("Math");
    if (avgMath) {
        cout << "   Средняя: " << *avgMath << endl;
    }

    system("pause");
    return 0;
}
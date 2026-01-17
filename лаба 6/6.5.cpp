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
#include <chrono>
#include <random>
#include <algorithm>
#include <cctype>

using namespace std;
using namespace std::chrono;

// ========== InputValidator ==========
class InputValidator {
private:
    static const regex emailRegex;

public:
    static bool isValidEmail(const string& email) {
        return regex_match(email, emailRegex);
    }

    static bool isSafeInput(const string& input) {
        if (input.find(';') != string::npos ||
            input.find('\'') != string::npos ||
            input.find('\"') != string::npos ||
            input.find('\\') != string::npos ||
            input.find('*') != string::npos ||
            input.find('/') != string::npos) {
            return false;
        }

        string upperInput = input;
        transform(upperInput.begin(), upperInput.end(), upperInput.begin(), ::toupper);

        if (upperInput.find("DROP") != string::npos ||
            upperInput.find("DELETE") != string::npos ||
            upperInput.find("UPDATE") != string::npos ||
            upperInput.find("INSERT") != string::npos ||
            upperInput.find("SELECT") != string::npos ||
            upperInput.find("--") != string::npos ||
            upperInput.find("/*") != string::npos ||
            upperInput.find("*/") != string::npos) {
            return false;
        }

        return true;
    }

    static bool isValidLength(const string& input, size_t minLen, size_t maxLen) {
        return input.length() >= minLen && input.length() <= maxLen;
    }

    static bool isValidGrade(int grade) {
        return grade >= 0 && grade <= 100;
    }

    static bool isValidGroup(const string& group) {
        return regex_match(group, regex("^[A-Z]{2}-\\d{3}$"));
    }
};

const regex InputValidator::emailRegex(R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)");

// ========== Структуры данных ==========
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

struct BatchStudent {
    string name;
    string email;
    string group;
    vector<Grade> grades;
};

// ========== DatabaseManager ==========
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
            cerr << "Ошибка открытия БД: " << sqlite3_errmsg(db) << endl;
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

        if (!execute(sql)) {
            cerr << "Ошибка создания таблиц" << endl;
            return false;
        }

        createIndexes();
        return true;
    }

    void createIndexes() {
        execute("CREATE INDEX IF NOT EXISTS idx_students_email ON students(email);");
        execute("CREATE INDEX IF NOT EXISTS idx_students_group ON students(group_name);");
        execute("CREATE INDEX IF NOT EXISTS idx_grades_student_id ON grades(student_id);");
        execute("CREATE INDEX IF NOT EXISTS idx_grades_subject ON grades(subject);");
    }

    bool execute(const string& sql) {
        char* errMsg = nullptr;
        if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK) {
            if (errMsg) {
                cerr << "SQL ошибка: " << errMsg << endl;
                sqlite3_free(errMsg);
            }
            return false;
        }
        return true;
    }

    sqlite3* getHandle() const { return db; }
};

// ========== StudentRepository ==========
class StudentRepository {
private:
    sqlite3* db;

    bool beginTransaction() {
        return execute("BEGIN TRANSACTION;");
    }

    bool commitTransaction() {
        return execute("COMMIT;");
    }

    bool rollbackTransaction() {
        return execute("ROLLBACK;");
    }

    bool execute(const string& sql) {
        char* errMsg = nullptr;
        return sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errMsg) == SQLITE_OK;
    }

public:
    StudentRepository(DatabaseManager& dbManager) : db(dbManager.getHandle()) {}

    // ========== Пакетная вставка ==========
    bool batchInsertStudents(const vector<BatchStudent>& batch) {
        if (batch.empty()) return true;

        cout << "Начало пакетной вставки " << batch.size() << " студентов..." << endl;

        if (!beginTransaction()) {
            cerr << "Ошибка начала транзакции" << endl;
            return false;
        }

        try {
            // Подготовленный запрос для студентов
            const string sqlStudent = "INSERT OR IGNORE INTO students (name, email, group_name) VALUES (?, ?, ?)";
            sqlite3_stmt* stmtStudent;

            if (sqlite3_prepare_v2(db, sqlStudent.c_str(), -1, &stmtStudent, nullptr) != SQLITE_OK) {
                cerr << "Ошибка подготовки запроса для студентов" << endl;
                rollbackTransaction();
                return false;
            }

            // Подготовленный запрос для оценок
            const string sqlGrade = "INSERT INTO grades (student_id, subject, grade) VALUES (?, ?, ?)";
            sqlite3_stmt* stmtGrade;

            if (sqlite3_prepare_v2(db, sqlGrade.c_str(), -1, &stmtGrade, nullptr) != SQLITE_OK) {
                cerr << "Ошибка подготовки запроса для оценок" << endl;
                sqlite3_finalize(stmtStudent);
                rollbackTransaction();
                return false;
            }

            int successCount = 0;
            for (size_t i = 0; i < batch.size(); i++) {
                const auto& student = batch[i];

                // Вставка студента
                sqlite3_bind_text(stmtStudent, 1, student.name.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmtStudent, 2, student.email.c_str(), -1, SQLITE_TRANSIENT);
                sqlite3_bind_text(stmtStudent, 3, student.group.c_str(), -1, SQLITE_TRANSIENT);

                if (sqlite3_step(stmtStudent) != SQLITE_DONE) {
                    cerr << "Ошибка вставки студента " << i << ": " << sqlite3_errmsg(db) << endl;
                    continue; // Пропускаем этого студента, продолжаем с остальными
                }

                int studentId = sqlite3_last_insert_rowid(db);
                sqlite3_reset(stmtStudent);

                // Вставка оценок
                for (const auto& grade : student.grades) {
                    sqlite3_bind_int(stmtGrade, 1, studentId);
                    sqlite3_bind_text(stmtGrade, 2, grade.subject.c_str(), -1, SQLITE_TRANSIENT);
                    sqlite3_bind_int(stmtGrade, 3, grade.grade);

                    if (sqlite3_step(stmtGrade) != SQLITE_DONE) {
                        cerr << "Ошибка вставки оценки для студента " << i << endl;
                    }
                    sqlite3_reset(stmtGrade);
                }

                successCount++;

                // Прогресс каждые 100 записей
                if (successCount % 100 == 0) {
                    cout << "  Добавлено " << successCount << " студентов..." << endl;
                }
            }

            sqlite3_finalize(stmtStudent);
            sqlite3_finalize(stmtGrade);

            if (!commitTransaction()) {
                cerr << "Ошибка коммита транзакции" << endl;
                rollbackTransaction();
                return false;
            }

            cout << "Успешно добавлено " << successCount << " из " << batch.size() << " студентов" << endl;
            return successCount > 0;

        }
        catch (const exception& e) {
            cerr << "Исключение при пакетной вставке: " << e.what() << endl;
            rollbackTransaction();
            return false;
        }
    }

    // ========== Генерация тестовых данных ==========
    static vector<BatchStudent> generateTestData(int count) {
        vector<BatchStudent> students;
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> groupDist(100, 999);
        uniform_int_distribution<> gradeDist(60, 100);

        vector<string> subjects = { "Math", "Physics", "Programming", "History", "Chemistry" };
        vector<string> firstNames = { "Ivan", "Petr", "Maria", "Anna", "Alex", "Olga", "Dmitry", "Elena" };
        vector<string> lastNames = { "Ivanov", "Petrov", "Sidorov", "Smirnov", "Kuznetsov", "Popov" };

        auto toLower = [](string str) {
            transform(str.begin(), str.end(), str.begin(), [](unsigned char c) { return tolower(c); });
            return str;
            };

        cout << "Генерация " << count << " тестовых студентов..." << endl;

        for (int i = 0; i < count; i++) {
            BatchStudent student;

            string firstName = firstNames[gen() % firstNames.size()];
            string lastName = lastNames[gen() % lastNames.size()];
            student.name = firstName + " " + lastName;
            student.email = toLower(firstName) + "." + toLower(lastName) + to_string(i) + "@university.ru"; // Добавляем индекс для уникальности
            student.group = "CS-" + to_string(groupDist(gen));

            // Генерация 3-5 оценок
            int gradeCount = 3 + gen() % 3;
            for (int j = 0; j < gradeCount; j++) {
                Grade grade;
                grade.subject = subjects[gen() % subjects.size()];
                grade.grade = gradeDist(gen);
                student.grades.push_back(grade);
            }

            students.push_back(student);
        }

        return students;
    }

    // ========== Другие методы ==========
    vector<Student> getStudentsByGroup(const string& groupName) {
        vector<Student> students;
        const string sql = "SELECT id, name, email, group_name FROM students WHERE group_name = ?";
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

    // ========== Тест производительности ==========
    void performanceTest() {
        cout << "\n=== ТЕСТ ПРОИЗВОДИТЕЛЬНОСТИ ===" << endl;

        // Тест 1: Пакетная вставка 100 студентов (вместо 1000 для быстрого теста)
        cout << "\n1. Пакетная вставка 100 студентов..." << endl;
        auto testData = generateTestData(100);

        auto start = high_resolution_clock::now();
        bool batchResult = batchInsertStudents(testData);
        auto end = high_resolution_clock::now();

        if (batchResult) {
            auto duration = duration_cast<milliseconds>(end - start).count();
            cout << "   ✓ Успешно за " << duration << " мс" << endl;
            cout << "   Скорость: " << (100.0 / duration * 1000) << " записей/сек" << endl;
        }

        // Тест 2: Поиск по группе
        cout << "\n2. Поиск студентов по группе..." << endl;
        start = high_resolution_clock::now();
        auto csStudents = getStudentsByGroup("CS-500");
        end = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(end - start).count();
        cout << "   Найдено: " << csStudents.size() << " студентов" << endl;
        cout << "   Время: " << duration << " мкс" << endl;

        // Тест 3: Топ студентов
        cout << "\n3. Получение топ-5 студентов..." << endl;
        start = high_resolution_clock::now();
        auto top5 = getTopStudents(5);
        end = high_resolution_clock::now();

        duration = duration_cast<milliseconds>(end - start).count();
        cout << "   Время: " << duration << " мс" << endl;
        if (!top5.empty()) {
            cout << "   Лучший студент: " << top5[0].first.name << " - " << top5[0].second << endl;
        }

        // Тест 4: Средняя оценка
        cout << "\n4. Расчет средней оценки по Math..." << endl;
        start = high_resolution_clock::now();
        auto avgMath = getAverageGradeBySubject("Math");
        end = high_resolution_clock::now();

        duration = duration_cast<microseconds>(end - start).count();
        if (avgMath) {
            cout << "   Средняя по Math: " << *avgMath << endl;
        }
        cout << "   Время: " << duration << " мкс" << endl;
    }
};

// ========== Unit-тесты ==========
void runTests() {
    cout << "\n=== UNIT-ТЕСТЫ ===" << endl;

    // Тест 1: Валидация email
    cout << "\n1. Тест валидации email:" << endl;
    cout << "   test@example.com: " << (InputValidator::isValidEmail("test@example.com") ? "✓" : "✗") << endl;
    cout << "   invalid-email: " << (InputValidator::isValidEmail("invalid-email") ? "✗" : "✓") << endl;

    // Тест 2: Проверка безопасности
    cout << "\n2. Тест безопасности:" << endl;
    cout << "   Normal input: " << (InputValidator::isSafeInput("Ivan Ivanov") ? "✓" : "✗") << endl;
    cout << "   SQL injection: " << (InputValidator::isSafeInput("test'; DROP TABLE students;--") ? "✗" : "✓") << endl;

    // Тест 3: Валидация оценок
    cout << "\n3. Тест валидации оценок:" << endl;
    cout << "   Grade 85: " << (InputValidator::isValidGrade(85) ? "✓" : "✗") << endl;
    cout << "   Grade 150: " << (InputValidator::isValidGrade(150) ? "✗" : "✓") << endl;

    // Тест 4: Валидация групп
    cout << "\n4. Тест валидации групп:" << endl;
    cout << "   CS-101: " << (InputValidator::isValidGroup("CS-101") ? "✓" : "✗") << endl;
    cout << "   INVALID: " << (InputValidator::isValidGroup("INVALID") ? "✗" : "✓") << endl;
}

// ========== Main функция ==========
// ========== Main функция ==========
int main() {
    setlocale(LC_ALL, "Russian");
    // Для корректного отображения символов в Windows
#ifdef _WIN32
    system("chcp 65001 > nul"); // UTF-8
#endif

    cout << "=== ОПТИМИЗАЦИЯ И ТЕСТИРОВАНИЕ ===" << endl;

    // Запуск unit-тестов
    cout << "\n=== UNIT-ТЕСТЫ ===" << endl;

    // Тест 1: Валидация email
    cout << "\n1. Тест валидации email:" << endl;
    cout << "   test@example.com: " << (InputValidator::isValidEmail("test@example.com") ? "OK" : "FAIL") << endl;
    cout << "   invalid-email: " << (InputValidator::isValidEmail("invalid-email") ? "FAIL" : "OK") << endl;

    // Тест 2: Проверка безопасности
    cout << "\n2. Тест безопасности:" << endl;
    cout << "   Normal input: " << (InputValidator::isSafeInput("Ivan Ivanov") ? "OK" : "FAIL") << endl;
    cout << "   SQL injection: " << (InputValidator::isSafeInput("test'; DROP TABLE students;--") ? "FAIL" : "OK") << endl;

    // Тест 3: Валидация оценок
    cout << "\n3. Тест валидации оценок:" << endl;
    cout << "   Grade 85: " << (InputValidator::isValidGrade(85) ? "OK" : "FAIL") << endl;
    cout << "   Grade 150: " << (InputValidator::isValidGrade(150) ? "FAIL" : "OK") << endl;

    // Тест 4: Валидация групп
    cout << "\n4. Тест валидации групп:" << endl;
    cout << "   CS-101: " << (InputValidator::isValidGroup("CS-101") ? "OK" : "FAIL") << endl;
    cout << "   INVALID: " << (InputValidator::isValidGroup("INVALID") ? "FAIL" : "OK") << endl;

    // Создание БД
    DatabaseManager dbManager;
    if (!dbManager.initialize("performance_test.db")) {
        cout << "\nОшибка инициализации БД" << endl;
        system("pause");
        return 1;
    }

    StudentRepository repo(dbManager);

    // Тест производительности
    cout << "\n=== ТЕСТ ПРОИЗВОДИТЕЛЬНОСТИ ===" << endl;

    // Тест 1: Пакетная вставка
    cout << "\n1. Пакетная вставка 100 студентов..." << endl;
    auto testData = StudentRepository::generateTestData(100);

    auto start = high_resolution_clock::now();
    bool batchResult = repo.batchInsertStudents(testData);
    auto end = high_resolution_clock::now();

    if (batchResult) {
        auto duration = duration_cast<milliseconds>(end - start).count();
        cout << "   Успешно за " << duration << " мс" << endl;
        cout << "   Скорость: " << (100.0 / duration * 1000) << " записей/сек" << endl;
    }

    // Тест 2: Поиск по реальной группе (берем группу из сгенерированных данных)
    cout << "\n2. Поиск студентов по группе..." << endl;
    if (!testData.empty()) {
        string sampleGroup = testData[0].group;
        cout << "   Поиск по группе: " << sampleGroup << endl;

        start = high_resolution_clock::now();
        auto groupStudents = repo.getStudentsByGroup(sampleGroup);
        end = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(end - start).count();
        cout << "   Найдено: " << groupStudents.size() << " студентов" << endl;
        cout << "   Время: " << duration << " мкс" << endl;

        if (!groupStudents.empty()) {
            cout << "   Пример: " << groupStudents[0].name << " (" << groupStudents[0].email << ")" << endl;
        }
    }

    // Тест 3: Топ студентов
    cout << "\n3. Получение топ-5 студентов..." << endl;
    start = high_resolution_clock::now();
    auto top5 = repo.getTopStudents(5);
    end = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(end - start).count();
    cout << "   Время: " << duration << " мс" << endl;
    if (!top5.empty()) {
        cout << "   Топ-5 студентов:" << endl;
        for (size_t i = 0; i < min(top5.size(), size_t(5)); i++) {
            cout << "   " << (i + 1) << ". " << top5[i].first.name
                << " (" << top5[i].first.group << ") - " << top5[i].second << endl;
        }
    }

    // Тест 4: Средняя оценка
    cout << "\n4. Расчет средней оценки по предметам..." << endl;
    vector<string> subjects = { "Math", "Physics", "Programming" };

    for (const auto& subject : subjects) {
        start = high_resolution_clock::now();
        auto avg = repo.getAverageGradeBySubject(subject);
        end = high_resolution_clock::now();

        duration = duration_cast<microseconds>(end - start).count();
        if (avg) {
            cout << "   " << subject << ": " << *avg << " (за " << duration << " мкс)" << endl;
        }
    }

    // Тест 5: Статистика
    cout << "\n5. Статистика базы данных:" << endl;
    cout << "   Всего студентов: " << testData.size() << endl;

    // Подсчет общего количества оценок
    int totalGrades = 0;
    for (const auto& student : testData) {
        totalGrades += student.grades.size();
    }
    cout << "   Всего оценок: " << totalGrades << endl;

    cout << "\n=== ТЕСТИРОВАНИЕ ЗАВЕРШЕНО ===" << endl;

    // Для паузы в Windows
#ifdef _WIN32
    system("pause");
#else
    cout << "\nНажмите Enter для выхода...";
    cin.get();
#endif

    return 0;
}
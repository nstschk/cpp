#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>
#include <random>
using namespace std;

class VectorSumCalculator {
private:
    vector<int> data;
    mutex sum_mutex;

public:
    VectorSumCalculator(size_t size) {
        // Используем random_device только для получения seed
        random_device rd;
        // Передаем seed в mt19937
        mt19937 gen(rd());

        uniform_int_distribution<> distrib(1, 100);
        data.reserve(size);

        // Заполнение вектора
        for (size_t i = 0; i < size; ++i) {
            data.push_back(distrib(gen));
        }
    }

    // Однопоточное вычисление суммы
    long long calculateSingleThreaded() {
        long long sum = 0;
        for (int num : data) {
            sum += num;
        }
        return sum;
    }

    // Многопоточное вычисление суммы
    long long calculateMultiThreaded(int num_threads) {
        // 1. Настройка количества потоков
        if (num_threads <= 0) num_threads = 4;

        // 2. Создаем общую сумму, защищенную мьютексом
        long long total_sum = 0;

        // 3. Создаем вектор потоков
        vector<thread> threads;

        // 4. Вычисляем размер части для каждого потока
        size_t chunk_size = data.size() / num_threads;

        // 5. Создаем и запускаем потоки
        for (int i = 0; i < num_threads; ++i) {
            size_t start = i * chunk_size;
            size_t end;

            // Последний поток берет остаток
            if (i == num_threads - 1) {
                end = data.size();
            }
            else {
                end = start + chunk_size;
            }

            // Создаем поток
            threads.emplace_back([this, start, end, &total_sum]() {
                // Локальная сумма для диапазона
                long long partial_sum = 0;

                // Считаем сумму в своем диапазоне
                for (size_t j = start; j < end; ++j) {
                    partial_sum += data[j];
                }

                // Блокируем мьютекс для безопасного добавления к общей сумме
                lock_guard<mutex> lock(sum_mutex);
                total_sum += partial_sum;
                });
        }

        // 6. Ждем завершения всех потоков
        for (auto& t : threads) {
            t.join();
        }

        return total_sum;
    }
};

int main() {
        setlocale(LC_ALL, "rus");
        // Создать объект
        VectorSumCalculator calculator(10000000);

        // Выполнить оба расчета
        cout << "Однопоточный расчет:" << endl;
        auto start = chrono::high_resolution_clock::now();
        long long single_sum = calculator.calculateSingleThreaded();
        auto end = chrono::high_resolution_clock::now();
        auto single_time = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "Сумма: " << single_sum << ", время: " << single_time.count() << " мс" << endl;

        cout << "\nМногопоточный расчет (4 потока):" << endl;
        start = chrono::high_resolution_clock::now();
        long long multi_sum = calculator.calculateMultiThreaded(4);
        end = chrono::high_resolution_clock::now();
        auto multi_time = chrono::duration_cast<chrono::milliseconds>(end - start);
        cout << "Сумма: " << multi_sum << ", время: " << multi_time.count() << " мс" << endl;

        // Сравнить результаты
        cout << "\nСравнение результатов:" << endl;
        if (single_sum == multi_sum) {
            cout << "Суммы совпадают!" << endl;
        }
        else {
            cout << "Ошибка: суммы не совпадают!" << endl;
        }

        // Сравнить время
        cout << "Ускорение: " << (double)single_time.count() / multi_time.count() << "x" << endl;
        cout << "\nНажмите Enter для выхода...";
        cin.get();  // Ждем нажатия Enter
        return 0;
    }
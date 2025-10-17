#include <iostream>
#include <cmath>
#include <algorithm>
using namespace std;

double calculateAverage(double temperatures[], int N) {
    double sum = 0;
    for (int i = 0; i < N; i++) {
        sum += temperatures[i];
    }
    double average = sum / N;
    cout << "Средняя температура за период " << N << " дней составляет " << average << endl;
    return average;
}

double calculateMax(double temperatures[], int N) {
    double max = -101;
    for (int i = 0; i < N; ++i) {
        if (temperatures[i] > max) {
            max = temperatures[i];
        }
    }
    cout << "Максимальная температура за период " << N << " дней составляет " << max << endl;
    return max;
}

double calculateMin(double temperatures[], int N) {
    double min = 101;
    for (int i = 0; i < N; ++i) {
        if (temperatures[i] < min) {
            min = temperatures[i];
        }
    }
    cout << "Минимальная температура за период " << N << " дней составляет " << min << endl;
    return min;
}

int main() {
    setlocale(LC_ALL, "rus");
    int N;

    cout << "Введите количество дней: " << endl;
    cout << "В случае, если введенное количество дней не целое число, то количество дней будет округлено" << endl;
    cin >> N;

    cin.ignore(10000, '\n');

    if (N <= 0) {
        cout << "Должно быть введено положительное число" << endl;
        return 0;
    }

    double* temperatures = new double[N];
    cout << "Вводимая температура должна лежать в диапозоне от -100 до +100 градусов" << endl;
    cout << "Введите температуры для " << N << " дней:" << endl;

    for (int i = 0; i < N; ++i) {
        while (true) {
            cout << "День " << (i + 1) << ": ";

            if (cin >> temperatures[i]) {
                if (temperatures[i] >= -100 && temperatures[i] <= 100) {
                    break;
                }
                else {
                    cout << "Температура должна быть от -100 до +100 градусов. Введите значение заново." << endl;
                }
            }
            else {
                cout << "Ошибка формата ввода. Введите значение заново." << endl;
                cin.clear();
                cin.ignore(10000, '\n');
            }
        }
    }

    calculateAverage(temperatures, N);
    calculateMax(temperatures, N);
    calculateMin(temperatures, N);

    delete[] temperatures;
    return 0;
}
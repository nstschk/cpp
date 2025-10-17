#include <iostream>
using namespace std;

void arrayInfo(int* arr, int size) {
    cout << "Размер массива: " << size << endl;
    int sum = 0;
    for (int i = 0; i < size; i++) {
        sum += arr[i];
    }
    cout << "Сумма элементов = " << sum << endl;

    cout << "Первый элемент = " << arr[0] << endl;
    cout << "Последний элемент = " << arr[size - 1] << endl;

}

int main() {
    setlocale(LC_ALL, "rus");
    int arr1[] = { 1, 2, 3, 4, 5 };
    int size1 = sizeof(arr1) / sizeof(arr1[0]);

    cout << "Массив 1:" << endl;
    arrayInfo(arr1, size1);

    int arr2[] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    int size2 = sizeof(arr2) / sizeof(arr2[0]);

    cout << "Массив 2:" << endl;
    arrayInfo(arr2, size2);

    return 0;
}
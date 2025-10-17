#include <iostream>
using namespace std;

int main() {
    setlocale(LC_ALL, "rus");
    int arr1[10]; 
    int arr2[10];  
    arr1[0] = 0;
    arr1[1] = 1;
    arr1[2] = 1;
    arr1[3] = 2;
    arr1[4] = 3;
    arr1[5] = 5;
    arr1[6] = 8;
    arr1[7] = 13;
    arr1[8] = 21;
    arr1[9] = 34;

    cout << "Массив: ";
    int* ptr = arr1;

    for (int i = 0; i < 10; i++) {
        cout << *ptr << " ";
        ptr++;
    }
    cout << endl;
    int sum = 0;
    ptr = arr1;

    for (int i = 0; i < 10; i++) {
        sum += *ptr;
        ptr++;
    }
    cout << "Сумма элементов = " << sum << endl;

    ptr = arr1;
    int min = *ptr;  

    for (int i = 1; i < 10; i++) {
        ptr++;
        if (*ptr < min) {
            min = *ptr;
        }
    }
    cout << "Минимальный элемент = " << min << endl;

    
    int* src_ptr = arr1 + 9;  
    int* dest_ptr = arr2;            

    for (int i = 0; i < 10; i++) {
        *dest_ptr = *src_ptr;  
        dest_ptr++;
        src_ptr--;
    }

  
    cout << "Массив в обратном порядке: ";
    ptr = arr2;  

    for (int i = 0; i < 10; i++) {
        cout << *ptr << " ";
        ptr++;
    }


    return 0;
}

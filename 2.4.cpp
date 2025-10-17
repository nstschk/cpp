#include <iostream>

using namespace std;

void swapValues1(int a, int b) {
    int temp = a;
    a = b;
    b = temp;
}
void swapValues2(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void swapValues3(int& a, int& b) {
    int temp = a;
    a = b;
    b = temp;
}

int main() {
    setlocale(LC_ALL, "rus");
    int x = 7;
    int y = 212;

    cout << "До вызова swapValues по значению: x = " << x << ", y = " << y << endl;
    swapValues1(x, y); 
    cout << "После вызова swapValues по значению: x = " << x << ", y = " << y << endl;
    
    x = 7;
    y = 212;
    cout << "До вызова swapValues по указателю: x = " << x << ", y = " << y << endl;
    swapValues2(&x, &y); 
    cout << "После вызова swapValues по указателю: x = " << x << ", y = " << y << endl;

 
    x = 7;
    y = 212;
    cout << "До вызова swapValues по ссылке: x = " << x << ", y = " << y << endl;
    swapValues3(x, y);
    cout << "После вызова swapValues по ссылке: x = " << x << ", y = " << y << endl;

    return 0;

}


